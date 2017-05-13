//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS function-declaration and script-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "CC/Exp.hpp"
#include "CC/Exp/Assign.hpp"
#include "CC/Exp/Init.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"
#include "CC/Statement.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"

#include "SR/Attribute.hpp"
#include "SR/Function.hpp"
#include "SR/Space.hpp"
#include "SR/Statement.hpp"
#include "SR/Storage.hpp"
#include "SR/Type.hpp"
#include "SR/Warning.hpp"


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
            throw Core::ExceptStr(attr.namePos,
               "name redefined as different kind of symbol");

         if(lookup.resFunc->retrn != attr.type->getBaseType())
            throw Core::ExceptStr(attr.namePos,
               "function redeclared with different return type");
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
         throw Core::ExceptStr(attr.namePos, "incomplete return");

      auto &fnScope = scope.createScope(attr, fn);

      auto stmntPre  = CC::StatementCreate_FuncPre(ctx.in.peek().pos, fnScope);
      auto stmntBody = ctx.getStatement(fnScope);
      auto stmntPro  = CC::StatementCreate_FuncPro(ctx.in.reget().pos, fnScope);

      // Create statements for the function.
      Core::Array<SR::Statement::CRef> stmnts =
         {Core::Pack, stmntPre, stmntBody, stmntPro};

      fn->stmnt = SR::StatementCreate_Multi(attr.namePos, std::move(stmnts));
      fn->defin = true;

      fn->setAllocAut(attr.allocAut);

      if(!fn->retrn->isTypeVoid() && fn->stmnt->isNoReturn())
         SR::WarnReturnType(attr.namePos, "no return in non-void function");
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
         throw Core::ParseExceptExpect(ctx.in.peek(), "script-address", false);
      }
   }

   //
   // ParseScriptFlagList
   //
   static void ParseScriptFlagList(Parser &ctx, SR::Attribute &attr)
   {
      while(ctx.in.peek(Core::TOK_KeyWrd)) switch(ctx.in.get().str)
      {
      case Core::STR_clientside: attr.stype.push_back(Core::STR_Clientside); break;
      case Core::STR_net:        attr.stype.push_back(Core::STR_Net);        break;
      default: ctx.in.unget(); return;
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
      if(!ctx.in.drop(Core::TOK_ParenO))
         throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

      // parameter-type-list
      SR::TypeSet::CPtr types;
      std::tie(types, attr.param) = ctx.getTypeList(scope);
      attr.type = attr.type->getTypeFunction(types, attr.callt);

      // )
      if(!ctx.in.drop(Core::TOK_ParenC))
         throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
   }

   //
   // ParseScriptType
   //
   static void ParseScriptType(Parser &ctx, SR::Attribute &attr)
   {
      if(ctx.in.peek(Core::TOK_KeyWrd)) switch(ctx.in.get().str)
      {
      case Core::STR_bluereturn:  attr.stype.push_back(Core::STR_BlueReturn);  break;
      case Core::STR_death:       attr.stype.push_back(Core::STR_Death);       break;
      case Core::STR_disconnect:  attr.stype.push_back(Core::STR_Disconnect);  break;
      case Core::STR_enter:       attr.stype.push_back(Core::STR_Enter);       break;
      case Core::STR_event:       attr.stype.push_back(Core::STR_Event);       break;
      case Core::STR_kill:        attr.stype.push_back(Core::STR_Kill);        break;
      case Core::STR_lightning:   attr.stype.push_back(Core::STR_Lightning);   break;
      case Core::STR_open:        attr.stype.push_back(Core::STR_Open);        break;
      case Core::STR_pickup:      attr.stype.push_back(Core::STR_Pickup);      break;
      case Core::STR_redreturn:   attr.stype.push_back(Core::STR_RedReturn);   break;
      case Core::STR_reopen:      attr.stype.push_back(Core::STR_Reopen);      break;
      case Core::STR_respawn:     attr.stype.push_back(Core::STR_Respawn);     break;
      case Core::STR_return:      attr.stype.push_back(Core::STR_Return);      break;
      case Core::STR_unloading:   attr.stype.push_back(Core::STR_Unloading);   break;
      case Core::STR_whitereturn: attr.stype.push_back(Core::STR_WhiteReturn); break;

      default: ctx.in.unget(); break;
      }
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
      //       parameter-type-list ) compound-statement
      //    <function> declaration-specifiers identifier (
      //       parameter-type-list ) ;

      SR::Attribute attr;
      attr.callt = IR::CallType::LangACS;
      attr.linka = IR::Linkage::ExtACS;

      // <function>
      if(!in.peek(Core::TOK_KeyWrd, Core::STR_function))
         throw Core::ParseExceptExpect(in.peek(), "function-declaration", false);

      auto pos = in.get().pos;

      // declaration-specifiers
      parseDeclSpec(scope, attr);

      // identifier
      if(!in.peek(Core::TOK_Identi))
         throw Core::ParseExceptExpect(in.peek(), "identifier", false);

      attr.setName(in.get());

      // ( parameter-type-list )
      ParseScriptParameters(*this, scope, attr);

      ParseDeclFunction(*this, scope, attr);

      return SR::StatementCreate_Empty(pos);
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

      SR::Attribute attr;
      attr.type = SR::Type::Void;

      // <script>
      if(!in.peek(Core::TOK_KeyWrd, Core::STR_script))
         throw Core::ParseExceptExpect(in.peek(), "script-declaration", false);

      auto pos = in.get().pos;

      // script-address
      ParseScriptAddr(*this, scope, attr);

      // script-parameters(opt)
      if(in.peek(Core::TOK_ParenO))
         ParseScriptParameters(*this, scope, attr);
      else
         attr.type = attr.type->getTypeFunction(SR::TypeSet::Get(false), attr.callt);

      // script-type(opt)
      ParseScriptType(*this, attr);

      // script-flag-list(opt)
      ParseScriptFlagList(*this, attr);

      if(!in.peek(Core::TOK_BraceO))
         throw Core::ParseExceptExpect(in.peek(), "{", true);

      ParseDeclFunction(*this, scope, attr);

      return SR::StatementCreate_Empty(pos);
   }
}

// EOF

