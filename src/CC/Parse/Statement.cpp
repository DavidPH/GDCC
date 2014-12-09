//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C statement parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Case.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Statement.hpp"
#include "CC/Type.hpp"

#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Statement.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetStatement_Compound
      //
      static AST::Statement::CRef GetStatement_Compound(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // compound-statement:
         //    { block-item-list(opt) }

         // {
         auto pos = ctx.in.get().pos;

         std::vector<AST::Statement::CRef> stmnts;
         auto &blockScope = scope.createScopeBlock();

         // block-item-list:
         //    block-item
         //    block-item-list block-item
         while(!ctx.in.drop(Core::TOK_BraceC))
         {
            // block-item:
            //    declaration
            //    statement

            // declaration
            if(IsDecl(ctx, scope))
               stmnts.emplace_back(GetDecl(ctx, blockScope));

            // statement
            else
               stmnts.emplace_back(GetStatement(ctx, blockScope));
         }

         // }

         return AST::StatementCreate_Multi(std::move(labels), pos,
            {stmnts.begin(), stmnts.end()});
      }

      //
      // GetStatement_Exp
      //
      static AST::Statement::CRef GetStatement_Exp(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // expression-statement:
         //    expression(opt) ;

         auto pos = ctx.in.peek().pos;

         // ;
         if(ctx.in.drop(Core::TOK_Semico))
            return AST::StatementCreate_Empty(std::move(labels), pos);

         // expression
         auto exp = GetExp(ctx, scope);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         return AST::StatementCreate_Exp(std::move(labels), pos, exp);
      }

      //
      // GetStatement_break
      //
      static AST::Statement::CRef GetStatement_break(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <break> ;

         // <break>
         auto pos = ctx.in.get().pos;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         return StatementCreate_Break(std::move(labels), pos, scope);
      }

      //
      // GetStatement_continue
      //
      static AST::Statement::CRef GetStatement_continue(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <continue> ;

         // <continue>
         auto pos = ctx.in.get().pos;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         return StatementCreate_Continue(std::move(labels), pos, scope);
      }

      //
      // GetStatement_do
      //
      static AST::Statement::CRef GetStatement_do(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <do> statement <while> ( expression ) ;

         // <do>
         auto pos = ctx.in.get().pos;

         // statement
         auto body = GetStatement(ctx, loopScope);

         // <while>
         if(!ctx.in.drop(Core::TOK_KeyWrd, Core::STR_while))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected 'while'");

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // expression
         auto cond = GetExp(ctx, loopScope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         return StatementCreate_Do(std::move(labels), pos, loopScope, body, cond);
      }

      //
      // GetStatement_for
      //
      static AST::Statement::CRef GetStatement_for(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <for> ( expression(opt) ; expression(opt) ; expression(opt) )
         // <for> ( declaration expression(opt) ; expression(opt) )

         // <for>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         AST::Statement::CPtr init;
         // declaration
         if(IsDecl(ctx, loopScope))
         {
            init = GetDecl(ctx, loopScope);
         }
         // expression(opt) ;
         else
         {
            // expression(opt)
            if(ctx.in.peek().tok != Core::TOK_Semico)
               init = AST::StatementCreate_Exp(GetExp(ctx, loopScope));
            else
               init = AST::StatementCreate_Empty(pos);

            // ;
            if(!ctx.in.drop(Core::TOK_Semico))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");
         }

         // expression(opt)
         AST::Exp::CPtr cond;
         if(ctx.in.peek().tok != Core::TOK_Semico)
            cond = GetExp(ctx, loopScope);
         else
            cond = ExpCreate_LitInt(TypeIntegPrS, 1, pos);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         // expression(opt)
         AST::Statement::CPtr iter;
         if(ctx.in.peek().tok != Core::TOK_ParenC)
            iter = AST::StatementCreate_Exp(GetExp(ctx, loopScope));
         else
            iter = AST::StatementCreate_Empty(pos);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         // statement
         auto body = GetStatement(ctx, loopScope);

         return StatementCreate_For(std::move(labels), pos, loopScope, init,
            cond, iter, body);
      }

      //
      // GetStatement_goto
      //
      static AST::Statement::CRef GetStatement_goto(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <goto> identifier ;

         // <goto>
         auto pos = ctx.in.get().pos;

         // identifier
         if(ctx.in.peek().tok != Core::TOK_Identi)
            throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");
         auto name = ctx.in.get().str;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         return StatementCreate_Goto(std::move(labels), pos, scope, name);
      }

      //
      // GetStatement_if
      //
      static AST::Statement::CRef GetStatement_if(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <if> ( expression ) statement
         // <if> ( expression ) statement <else> statement

         // <if>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // expression
         auto cond = GetExp(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         // statement
         auto bodyT = GetStatement(ctx, scope);

         // <else> statement
         if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_else))
         {
            // statement
            auto bodyF = GetStatement(ctx, scope);

            return StatementCreate_If(std::move(labels), pos, cond, bodyT, bodyF);
         }

         return StatementCreate_If(std::move(labels), pos, cond, bodyT);
      }

      //
      // GetStatement_return
      //
      static AST::Statement::CRef GetStatement_return(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <return> expression(opt) ;

         // <return>
         auto pos = ctx.in.get().pos;

         // ;
         if(ctx.in.drop(Core::TOK_Semico))
            return StatementCreate_Return(std::move(labels), pos, scope.fn);

         // expression
         auto exp = GetExp(ctx, scope);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

         return StatementCreate_Return(std::move(labels), pos, scope.fn, exp);
      }

      //
      // GetStatement_switch
      //
      static AST::Statement::CRef GetStatement_switch(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &switchScope = scope.createScopeCase();

         // <switch> ( expression ) statement

         // <switch>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // expression
         auto cond = GetExp(ctx, switchScope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         // statement
         auto body = GetStatement(ctx, switchScope);

         return StatementCreate_Switch(std::move(labels), pos, switchScope,
            cond, body);
      }

      //
      // GetStatement_while
      //
      static AST::Statement::CRef GetStatement_while(ParserCtx const &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <while> ( expression ) statement

         // <while>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // expression
         auto cond = GetExp(ctx, loopScope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         // statement
         auto body = GetStatement(ctx, loopScope);

         return StatementCreate_While(std::move(labels), pos, loopScope, cond, body);
      }

      //
      // GetStatementLabel
      //
      static Core::Array<Core::String> GetStatementLabel(ParserCtx const &ctx,
         Scope_Local &scope)
      {
         std::vector<Core::String> labels;

         for(;;)
         {
            auto tok = ctx.in.peek();

            // Keyword label?
            if(tok.tok == Core::TOK_KeyWrd)
            {
               // <case> constant-expression :
               if(tok.str == Core::STR_case)
               {
                  ctx.in.get();

                  auto val = ExpToInteg(GetExp_Cond(ctx, scope));

                  if(!ctx.in.drop(Core::TOK_Colon))
                     throw Core::ExceptStr(ctx.in.peek().pos, "expected ':'");

                  auto label = scope.getLabelCase(val, true);

                  if(!label)
                     throw Core::ExceptStr(tok.pos, "case redefined");

                  labels.emplace_back(label);
               }

               // <default> :
               else if(tok.str == Core::STR_default)
               {
                  ctx.in.get();

                  if(!ctx.in.drop(Core::TOK_Colon))
                     throw Core::ExceptStr(ctx.in.peek().pos, "expected ':'");

                  auto label = scope.getLabelDefault(true);

                  if(!label)
                     throw Core::ExceptStr(tok.pos, "default redefined");

                  labels.emplace_back(label);
               }

               else
                  break;
            }

            // identifier :
            else if(tok.tok == Core::TOK_Identi)
            {
               ctx.in.get();
               if(ctx.in.drop(Core::TOK_Colon))
               {
                  auto label = scope.getLabel(tok.str);

                  if(!label)
                     throw Core::ExceptStr(tok.pos, "label redefined");

                  labels.emplace_back(label);
               }
               else
               {
                  ctx.in.unget();
                  break;
               }
            }

            else
               break;
         }

         return Core::Array<Core::String>(labels.begin(), labels.end());
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetStatement
      //
      AST::Statement::CRef GetStatement(ParserCtx const &ctx, Scope_Local &scope)
      {
         auto labels = GetStatementLabel(ctx, scope);

         // compound-statement
         if(ctx.in.peek().tok == Core::TOK_BraceO)
            return GetStatement_Compound(ctx, scope, labels);

         if(ctx.in.peek().tok == Core::TOK_KeyWrd) switch(ctx.in.peek().str)
         {
            // selection-statement
         case Core::STR_if:     return GetStatement_if    (ctx, scope, labels);
         case Core::STR_switch: return GetStatement_switch(ctx, scope, labels);

            // iteration-statement
         case Core::STR_while: return GetStatement_while(ctx, scope, labels);
         case Core::STR_do:    return GetStatement_do   (ctx, scope, labels);
         case Core::STR_for:   return GetStatement_for  (ctx, scope, labels);

            // jump-statement:
         case Core::STR_goto:     return GetStatement_goto    (ctx, scope, labels);
         case Core::STR_continue: return GetStatement_continue(ctx, scope, labels);
         case Core::STR_break:    return GetStatement_break   (ctx, scope, labels);
         case Core::STR_return:   return GetStatement_return  (ctx, scope, labels);

         default: break;
         }

         // expression-statement
         return GetStatement_Exp(ctx, scope, labels);
      }
   }
}

// EOF

