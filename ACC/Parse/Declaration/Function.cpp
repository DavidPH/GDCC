//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS function-declaration and script-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"
#include "ACC/Scope.hpp"

#include "CC/Exp/Assign.hpp"
#include "CC/Exp/Init.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Linkage.hpp"

#include "SR/Attribute.hpp"
#include "SR/Function.hpp"
#include "SR/Space.hpp"
#include "SR/Statement.hpp"
#include "SR/Storage.hpp"
#include "SR/Type.hpp"
#include "SR/Warning.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // GetDeclFunction
   //
   static SR::Function::Ref GetDeclFunction(Scope_Global &scope,
      SR::Attribute &attr)
   {
      auto fn = scope.getFunction(attr);

      // Set address, if one provided.
      if(attr.addrI)
         fn->valueInt = attr.addrI;
      else if(attr.addrS)
         fn->valueStr = attr.addrS;

      return fn;
   }

   //
   // ParseDeclFunction
   //
   static void ParseDeclFunction(Parser &ctx, Scope_Global &scope, SR::Attribute &attr)
   {
      // Check compatibility with existing symbol, if any.
      if(auto lookup = scope.find(attr.name))
      {
         if(lookup.res != CC::Lookup::Func)
            Core::Error(attr.namePos, "name redefined as different kind of symbol");

         if(lookup.resFunc->retrn != attr.type->getBaseType())
            Core::Error(attr.namePos, "function redeclared with different return type");
      }

      if(ctx.importing)
      {
         if(attr.linka != IR::Linkage::None)
            scope.add(attr.name, GetDeclFunction(scope, attr));

         ctx.skipBalancedToken();
         return;
      }

      auto fn = GetDeclFunction(scope, attr);

      scope.add(attr.name, fn);

      if(ctx.in.drop(Core::TOK_Semico))
         return;

      if(!fn->retrn->isTypeComplete() && !fn->retrn->isTypeVoid())
         Core::Error(attr.namePos, "incomplete return");

      auto &fnScope = scope.createScope(attr, fn);

      auto stmntPre  = ctx.fact.stCreate_FuncPre(ctx.in.peek().pos, fnScope);
      auto stmntBody = ctx.getStCompound(fnScope, {}, {});
      auto stmntPro  = ctx.fact.stCreate_FuncPro(ctx.in.reget().pos, fnScope);

      // Create statements for the function.
      Core::Array<SR::Statement::CRef> stmnts =
         {Core::Pack, stmntPre, stmntBody, stmntPro};

      fn->stmnt = ctx.fact.stCreate_Multi({}, attr.namePos, std::move(stmnts));
      fn->defin = true;

      fn->setAllocAut(attr.allocAut);

      if(!fn->retrn->isTypeVoid() && fn->stmnt->isNoReturn())
         Core::Error(attr.namePos, "no return in non-void function");
   }

   //
   // ParseScriptAddr
   //
   static void ParseScriptAddr(Parser &ctx, Scope_Global &scope,
      SR::Attribute &attr)
   {
      // script-address:
      //    integer
      //    string-literal
      attr.namePos = ctx.in.peek().pos;
      if(ctx.in.peek().isTokString())
      {
         attr.addrS = ctx.in.get().str;
         attr.callt = IR::CallType::ScriptS;
      }
      else if(ctx.in.peek(Core::TOK_NumInt))
      {
         attr.addrI = ctx.getExp_Prim(scope)->getIRExp();
         attr.callt = IR::CallType::ScriptI;
      }
      else
      {
         Core::ErrorExpect("script-address", ctx.in.peek());
      }
   }

   //
   // ParseScriptParameters
   //
   static void ParseScriptParameters(Parser &ctx, Scope_Global &scope,
      SR::Attribute &attr)
   {
      // script-parameters:
      //    ( parameter-type-list )

      // (
      ctx.expect(Core::TOK_ParenO);

      // parameter-type-list
      SR::TypeSet::CPtr types;
      std::tie(types, attr.param) = ctx.getTypeList(scope);
      attr.type = attr.type->getTypeFunction(types, attr.callt);

      // )
      ctx.expect(Core::TOK_ParenC);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getDecl_Function
   //
   SR::Statement::CRef Parser::getDecl_Function(Scope_Global &scope)
   {
      // function-declaration:
      //    <function> declaration-specifiers identifier (
      //       parameter-type-list ) identifier-sequence(opt)
      //       compound-statement
      //    <function> declaration-specifiers identifier (
      //       parameter-type-list ) identifier-sequence(opt) ;

      SR::Attribute attr;
      attr.callt = IR::CallType::LangACS;
      attr.linka = IR::Linkage::ExtACS;

      // <function>
      auto pos = expect(Core::TOK_KeyWrd, Core::STR_function).pos;

      // declaration-specifiers
      parseDeclSpec(scope, attr);

      // identifier
      attr.setName(expectIdenti());

      // ( parameter-type-list )
      ParseScriptParameters(*this, scope, attr);

      // identifier-sequence(opt)
      while(in.peek(Core::TOK_KeyWrd) || in.peek(Core::TOK_Identi))
         attr.stype.push_back(in.get().str);

      ParseDeclFunction(*this, scope, attr);

      return fact.stCreate_Empty({}, pos);
   }

   //
   // Parser::getDecl_Script
   //
   SR::Statement::CRef Parser::getDecl_Script(Scope_Global &scope)
   {
      // script-declaration:
      //    <script> script-address script-parameters(opt) script-type
      //       script-flag-sequence(opt) compound-statement
      //    <script> script-address script-parameters script-flag-sequence(opt)
      //       compound-statement
      //    <script> script-address script-parameters(opt)
      //       identifier-sequence(opt) compound-statement

      SR::Attribute attr;
      attr.type = SR::Type::Void;

      // <script>
      auto pos = expect(Core::TOK_KeyWrd, Core::STR_script).pos;

      // script-address
      ParseScriptAddr(*this, scope, attr);

      // script-parameters(opt)
      if(in.peek(Core::TOK_ParenO))
         ParseScriptParameters(*this, scope, attr);
      else
         attr.type = attr.type->getTypeFunction(SR::TypeSet::Get(false), attr.callt);

      // script-type(opt)
      // script-flag-list(opt)
      // identifier-sequence(opt)
      while(in.peek(Core::TOK_KeyWrd) || in.peek(Core::TOK_Identi))
         attr.stype.push_back(in.get().str);

      if(!in.peek(Core::TOK_BraceO))
         Core::ErrorExpect("{", in.peek(), true);

      ParseDeclFunction(*this, scope, attr);

      return fact.stCreate_Empty({}, pos);
   }
}

// EOF

