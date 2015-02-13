//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS statement parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AS/LabelTBuf.hpp"
#include "AS/TStream.hpp"

#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Statement.hpp"

#include "CC/AsmGlyphTBuf.hpp"
#include "CC/Exp.hpp"
#include "CC/Scope/Case.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Statement.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/StringBuf.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetStatement_Compound
      //
      static AST::Statement::CRef GetStatement_Compound(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
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
            if(IsDecl(ctx, blockScope))
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
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
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
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return AST::StatementCreate_Exp(std::move(labels), pos, exp);
      }

      //
      // GetStatement_break
      //
      static AST::Statement::CRef GetStatement_break(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <break> ;

         // <break>
         auto pos = ctx.in.get().pos;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return CC::StatementCreate_Break(std::move(labels), pos, scope);
      }

      //
      // GetStatement_continue
      //
      static AST::Statement::CRef GetStatement_continue(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <continue> ;

         // <continue>
         auto pos = ctx.in.get().pos;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return CC::StatementCreate_Continue(std::move(labels), pos, scope);
      }

      //
      // GetStatement_do
      //
      static AST::Statement::CRef GetStatement_do(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <do> statement <while> ( expression ) ;
         // <do> statement <until> ( expression ) ;

         // <do>
         auto pos = ctx.in.get().pos;

         // statement
         auto body = GetStatement(ctx, loopScope);

         // <while>
         // <until>
         bool invert;
         if(!(invert = ctx.in.drop(Core::TOK_KeyWrd, Core::STR_until)) &&
            !ctx.in.drop(Core::TOK_KeyWrd, Core::STR_while))
            throw Core::ParseExceptExpect(ctx.in.peek(), "'while' or 'until'", false);

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // expression
         auto cond = GetExp(ctx, loopScope);
         if(invert)
            cond = CC::ExpCreate_Not(cond, cond->pos);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return CC::StatementCreate_Do(std::move(labels), pos, loopScope, body, cond);
      }

      //
      // GetStatement_for
      //
      static AST::Statement::CRef GetStatement_for(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <for> ( expression(opt) ; expression(opt) ; expression(opt) )
         // <for> ( declaration expression(opt) ; expression(opt) )

         // <for>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

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
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);
         }

         // expression(opt)
         AST::Exp::CPtr cond;
         if(ctx.in.peek().tok != Core::TOK_Semico)
            cond = GetExp(ctx, loopScope);
         else
            cond = CC::ExpCreate_LitInt(CC::TypeIntegPrS, 1, pos);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         // expression(opt)
         AST::Statement::CPtr iter;
         if(ctx.in.peek().tok != Core::TOK_ParenC)
            iter = AST::StatementCreate_Exp(GetExp(ctx, loopScope));
         else
            iter = AST::StatementCreate_Empty(pos);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // statement
         auto body = GetStatement(ctx, loopScope);

         return CC::StatementCreate_For(std::move(labels), pos, loopScope,
            init, cond, iter, body);
      }

      //
      // GetStatement_if
      //
      static AST::Statement::CRef GetStatement_if(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <if> ( expression ) statement
         // <if> ( expression ) statement <else> statement

         // <if>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // expression
         auto cond = GetExp(ctx, scope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // statement
         auto bodyT = GetStatement(ctx, scope);

         // <else> statement
         if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_else))
         {
            // statement
            auto bodyF = GetStatement(ctx, scope);

            return CC::StatementCreate_If(std::move(labels), pos, cond, bodyT, bodyF);
         }

         return CC::StatementCreate_If(std::move(labels), pos, cond, bodyT);
      }

      //
      // GetStatement_return
      //
      static AST::Statement::CRef GetStatement_return(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <return> expression(opt) ;

         // <return>
         auto pos = ctx.in.get().pos;

         // ;
         if(ctx.in.drop(Core::TOK_Semico))
            return CC::StatementCreate_Return(std::move(labels), pos, scope.fn);

         // expression
         auto exp = GetExp(ctx, scope);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return CC::StatementCreate_Return(std::move(labels), pos, scope.fn, exp);
      }

      //
      // GetStatement_switch
      //
      static AST::Statement::CRef GetStatement_switch(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &switchScope = scope.createScopeCase();

         // <switch> ( expression ) statement

         // <switch>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // expression
         auto cond = GetExp(ctx, switchScope);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // statement
         auto body = GetStatement(ctx, switchScope);

         return CC::StatementCreate_Switch(std::move(labels), pos, switchScope,
            cond, body);
      }

      //
      // GetStatement_while
      //
      static AST::Statement::CRef GetStatement_while(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <while> ( expression ) statement
         // <until> ( expression ) statement

         // <while>
         // <until>
         bool invert = ctx.in.peek().str == Core::STR_until;
         auto pos    = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // expression
         auto cond = GetExp(ctx, loopScope);
         if(invert)
            cond = CC::ExpCreate_Not(cond, cond->pos);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // statement
         auto body = GetStatement(ctx, loopScope);

         return CC::StatementCreate_While(std::move(labels), pos, loopScope,
            cond, body);
      }

      //
      // GetStatement_terminate
      //
      static AST::Statement::CRef GetStatement_terminate(ParserCtx const &ctx,
         CC::Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <terminate> ;

         // <terminate>
         auto pos = ctx.in.get().pos;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
               throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return CC::StatementCreate_Return(std::move(labels), pos, scope.fn);
      }

      //
      // GetStatementLabel
      //
      static Core::Array<Core::String> GetStatementLabel(ParserCtx const &ctx,
         CC::Scope_Local &scope)
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

                  auto val = CC::ExpToInteg(GetExp_Cond(ctx, scope));

                  if(!ctx.in.drop(Core::TOK_Colon))
                     throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

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
                     throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

                  auto label = scope.getLabelDefault(true);

                  if(!label)
                     throw Core::ExceptStr(tok.pos, "default redefined");

                  labels.emplace_back(label);
               }

               else
                  break;
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
   namespace ACC
   {
      //
      // GetStatement
      //
      AST::Statement::CRef GetStatement(ParserCtx const &ctx,
         CC::Scope_Local &scope)
      {
         auto labels = GetStatementLabel(ctx, scope);

         // compound-statement
         if(ctx.in.peek().tok == Core::TOK_BraceO)
            return GetStatement_Compound(ctx, scope, labels);

         if(ctx.in.peek(Core::TOK_KeyWrd))
            switch(ctx.in.peek().str)
         {
            // selection-statement
         case Core::STR_if:     return GetStatement_if    (ctx, scope, labels);
         case Core::STR_switch: return GetStatement_switch(ctx, scope, labels);

            // iteration-statement
         case Core::STR_while: return GetStatement_while(ctx, scope, labels);
         case Core::STR_until: return GetStatement_while(ctx, scope, labels);
         case Core::STR_do:    return GetStatement_do   (ctx, scope, labels);
         case Core::STR_for:   return GetStatement_for  (ctx, scope, labels);

            // jump-statement:
         case Core::STR_continue:  return GetStatement_continue (ctx, scope, labels);
         case Core::STR_break:     return GetStatement_break    (ctx, scope, labels);
         case Core::STR_return:    return GetStatement_return   (ctx, scope, labels);
       //case Core::STR_restart:   return GetStatement_restart  (ctx, scope, labels);
         case Core::STR_terminate: return GetStatement_terminate(ctx, scope, labels);

         default: break;
         }

         // expression-statement
         return GetStatement_Exp(ctx, scope, labels);
      }
   }
}

// EOF

