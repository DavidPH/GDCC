//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
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

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Space.hpp"
#include "AST/Statement.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"
#include "AST/Warning.hpp"

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
#include "IR/ScriptType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetDeclFunction
      //
      static AST::Function::Ref GetDeclFunction(Scope_Global &scope,
         AST::Attribute &attr)
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
      static void ParseDeclFunction(ParserCtx const &ctx, Scope_Global &scope,
         AST::Attribute &attr)
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

            SkipBalancedToken(ctx);
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
         auto stmntBody = GetStatement(ctx, fnScope);
         auto stmntPro  = CC::StatementCreate_FuncPro(ctx.in.reget().pos, fnScope);

         // Create statements for the function.
         Core::Array<AST::Statement::CRef> stmnts =
            {Core::Pack, stmntPre, stmntBody, stmntPro};

         fn->stmnt = AST::StatementCreate_Multi(attr.namePos, std::move(stmnts));
         fn->defin = true;

         fn->setAllocLoc(attr.allocLoc);

         if(!fn->retrn->isTypeVoid() && fn->stmnt->isNoReturn())
            AST::WarnReturnType(attr.namePos, "no return in non-void function");
      }

      //
      // ParseScriptAddr
      //
      static void ParseScriptAddr(ParserCtx const &ctx, Scope_Global &scope,
         AST::Attribute &attr)
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
            attr.addrI = GetExp_Prim(ctx, scope)->getIRExp();
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
      static void ParseScriptFlagList(ParserCtx const &ctx, AST::Attribute &attr)
      {
         while(ctx.in.peek(Core::TOK_KeyWrd)) switch(ctx.in.get().str)
         {
         case Core::STR_clientside: attr.sflagClS = true; break;
         case Core::STR_net:        attr.sflagNet = true; break;
         default: ctx.in.unget(); return;
         }
      }

      //
      // ParseScriptParameters
      //
      static void ParseScriptParameters(ParserCtx const &ctx, Scope_Global &scope,
         AST::Attribute &attr)
      {
         // script-parameters:
         //    ( parameter-type-list )

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // parameter-type-list
         AST::TypeSet::CPtr types;
         std::tie(types, attr.param) = GetTypeList(ctx, scope);
         attr.type = attr.type->getTypeFunction(types, attr.callt);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
      }

      //
      // ParseScriptType
      //
      static void ParseScriptType(ParserCtx const &ctx, AST::Attribute &attr)
      {
         if(ctx.in.peek(Core::TOK_KeyWrd)) switch(ctx.in.get().str)
         {
         case Core::STR_bluereturn:  attr.stype = IR::ScriptType::BlueReturn;  break;
         case Core::STR_death:       attr.stype = IR::ScriptType::Death;       break;
         case Core::STR_disconnect:  attr.stype = IR::ScriptType::Disconnect;  break;
         case Core::STR_enter:       attr.stype = IR::ScriptType::Enter;       break;
         case Core::STR_event:       attr.stype = IR::ScriptType::Event;       break;
         case Core::STR_lightning:   attr.stype = IR::ScriptType::Lightning;   break;
         case Core::STR_open:        attr.stype = IR::ScriptType::Open;        break;
         case Core::STR_pickup:      attr.stype = IR::ScriptType::Pickup;      break;
         case Core::STR_redreturn:   attr.stype = IR::ScriptType::RedReturn;   break;
         case Core::STR_respawn:     attr.stype = IR::ScriptType::Respawn;     break;
         case Core::STR_return:      attr.stype = IR::ScriptType::Return;      break;
         case Core::STR_unloading:   attr.stype = IR::ScriptType::Unloading;   break;
         case Core::STR_whitereturn: attr.stype = IR::ScriptType::WhiteReturn; break;

         default: ctx.in.unget(); break;
         }
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetDecl_Function
      //
      AST::Statement::CRef GetDecl_Function(ParserCtx const &ctx,
         Scope_Global &scope)
      {
         // function-declaration:
         //    <function> declaration-specifiers identifier (
         //       parameter-type-list ) compound-statement
         //    <function> declaration-specifiers identifier (
         //       parameter-type-list ) ;

         AST::Attribute attr;
         attr.callt = IR::CallType::LangACS;
         attr.linka = IR::Linkage::ExtACS;

         // <function>
         if(!ctx.in.peek(Core::TOK_KeyWrd, Core::STR_function))
            throw Core::ParseExceptExpect(ctx.in.peek(), "function-declaration", false);

         auto pos = ctx.in.get().pos;

         // declaration-specifiers
         ParseDeclSpec(ctx, scope, attr);

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         attr.setName(ctx.in.get());

         // ( parameter-type-list )
         ParseScriptParameters(ctx, scope, attr);

         ParseDeclFunction(ctx, scope, attr);

         return AST::StatementCreate_Empty(pos);
      }

      //
      // GetDecl_Script
      //
      AST::Statement::CRef GetDecl_Script(ParserCtx const &ctx,
         Scope_Global &scope)
      {
         // script-declaration:
         //    <script> script-address script-parameters(opt) script-type
         //       script-flag-sequence(opt) compound-statement
         //    <script> script-address script-parameters script-flag-sequence(opt)
         //       compound-statement

         AST::Attribute attr;
         attr.type = AST::Type::Void;

         // <script>
         if(!ctx.in.peek(Core::TOK_KeyWrd, Core::STR_script))
            throw Core::ParseExceptExpect(ctx.in.peek(), "script-declaration", false);

         auto pos = ctx.in.get().pos;

         // script-address
         ParseScriptAddr(ctx, scope, attr);

         // script-parameters(opt)
         if(ctx.in.peek(Core::TOK_ParenO))
            ParseScriptParameters(ctx, scope, attr);
         else
            attr.type = attr.type->getTypeFunction(AST::TypeSet::Get(false), attr.callt);

         // script-type(opt)
         ParseScriptType(ctx, attr);

         // script-flag-list(opt)
         ParseScriptFlagList(ctx, attr);

         if(!ctx.in.peek(Core::TOK_BraceO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "{", true);

         ParseDeclFunction(ctx, scope, attr);

         return AST::StatementCreate_Empty(pos);
      }
   }
}

// EOF

