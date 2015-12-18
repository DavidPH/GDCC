//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C statement parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/AsmGlyphTBuf.hpp"
#include "CC/Exp/Assign.hpp"
#include "CC/Scope/Case.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Statement.hpp"
#include "CC/Type.hpp"

#include "AS/LabelTBuf.hpp"
#include "AS/TStream.hpp"

#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Statement.hpp"
#include "AST/Warning.hpp"

#include "Core/Exception.hpp"
#include "Core/StringBuf.hpp"
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
      static AST::Statement::CRef GetStatement_Compound(Parser &ctx,
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
            if(ctx.isDecl(blockScope))
               stmnts.emplace_back(ctx.getDecl(blockScope));

            // statement
            else
               stmnts.emplace_back(ctx.getStatement(blockScope));
         }

         // }

         return AST::StatementCreate_Multi(std::move(labels), pos,
            {stmnts.begin(), stmnts.end()});
      }

      //
      // GetStatement_Exp
      //
      static AST::Statement::CRef GetStatement_Exp(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // expression-statement:
         //    expression(opt) ;

         auto pos = ctx.in.peek().pos;

         // ;
         if(ctx.in.drop(Core::TOK_Semico))
            return AST::StatementCreate_Empty(std::move(labels), pos);

         // expression
         auto exp = ctx.getExp(scope);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return AST::StatementCreate_Exp(std::move(labels), pos, exp);
      }

      //
      // GetStatement_asm
      //
      static AST::Statement::CRef GetStatement_asm(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <__asm> ( string-literal ) ;

         // <__asm>
         auto pos = ctx.in.get().pos;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // string-literal
         if(!ctx.in.peek().isTokString())
            throw Core::ExceptStr(ctx.in.peek().pos, "expected string-literal");

         auto tok = ctx.in.get();

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         // Convert string to a series of assembly tokens.
         Core::StringBuf sbuf{tok.str.data(), tok.str.size()};
         AS::TStream     in  {sbuf, tok.pos.file, tok.pos.line};
         AS::LabelTBuf   ltb {*in.tkbuf(), scope.fn.fn->glyph};
         AsmGlyphTBuf    gtb {ltb, scope};
         in.tkbuf(&gtb);

         std::vector<Core::Token> tokens;
         while(in >> tok) tokens.push_back(tok);
         tokens.emplace_back(tok.pos, nullptr, Core::TOK_EOF);

         return StatementCreate_Asm(std::move(labels), pos,
            {Core::Move, tokens.begin(), tokens.end()});
      }

      //
      // GetStatement_break
      //
      static AST::Statement::CRef GetStatement_break(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <break> ;

         // <break>
         auto pos = ctx.in.get().pos;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return StatementCreate_Break(std::move(labels), pos, scope);
      }

      //
      // GetStatement_continue
      //
      static AST::Statement::CRef GetStatement_continue(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <continue> ;

         // <continue>
         auto pos = ctx.in.get().pos;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return StatementCreate_Continue(std::move(labels), pos, scope);
      }

      //
      // GetStatement_do
      //
      static AST::Statement::CRef GetStatement_do(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <do> statement <while> ( expression ) ;

         // <do>
         auto pos = ctx.in.get().pos;

         // statement
         auto body = ctx.getStatement(loopScope);

         // <while>
         if(!ctx.in.drop(Core::TOK_KeyWrd, Core::STR_while))
            throw Core::ParseExceptExpect(ctx.in.peek(), "while", true);

         // ( expression )
         auto cond = ctx.getStatementCond(loopScope);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return StatementCreate_Do(std::move(labels), pos, loopScope, body, cond);
      }

      //
      // GetStatement_for
      //
      static AST::Statement::CRef GetStatement_for(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
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
         if(ctx.isDecl(loopScope))
         {
            init = ctx.getDecl(loopScope);
         }
         // expression(opt) ;
         else
         {
            // expression(opt)
            if(ctx.in.peek().tok != Core::TOK_Semico)
               init = AST::StatementCreate_Exp(ctx.getExp(loopScope));
            else
               init = AST::StatementCreate_Empty(pos);

            // ;
            if(!ctx.in.drop(Core::TOK_Semico))
               throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);
         }

         // expression(opt)
         AST::Exp::CPtr cond;
         if(ctx.in.peek().tok != Core::TOK_Semico)
            cond = ctx.getStatementCondExp(loopScope);
         else
            cond = ExpCreate_LitInt(TypeIntegPrS, 1, pos);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         // expression(opt)
         AST::Statement::CPtr iter;
         if(ctx.in.peek().tok != Core::TOK_ParenC)
            iter = AST::StatementCreate_Exp(ctx.getExp(loopScope));
         else
            iter = AST::StatementCreate_Empty(pos);

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // statement
         auto body = ctx.getStatement(loopScope);

         return StatementCreate_For(std::move(labels), pos, loopScope, init,
            cond, iter, body);
      }

      //
      // GetStatement_goto
      //
      static AST::Statement::CRef GetStatement_goto(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <goto> identifier ;
         // <goto> * cast-expression ;

         // <goto>
         auto pos = ctx.in.get().pos;

         // * cast-expression ;
         if(ctx.in.drop(Core::TOK_Mul))
         {
            auto exp = ctx.getExp_Cast(scope);

            // ;
            if(!ctx.in.drop(Core::TOK_Semico))
               throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

            return StatementCreate_Goto(std::move(labels), pos, exp);
         }

         // identifier
         if(ctx.in.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);
         auto name = ctx.in.get().str;

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return StatementCreate_Goto(std::move(labels), pos, scope, name);
      }

      //
      // GetStatement_if
      //
      static AST::Statement::CRef GetStatement_if(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <if> ( expression ) statement
         // <if> ( expression ) statement <else> statement

         // <if>
         auto pos = ctx.in.get().pos;

         // ( expression )
         auto cond = ctx.getStatementCond(scope);

         // statement
         auto bodyT = ctx.getStatement(scope);

         // <else> statement
         if(ctx.in.drop(Core::TOK_KeyWrd, Core::STR_else))
         {
            // statement
            auto bodyF = ctx.getStatement(scope);

            return StatementCreate_If(std::move(labels), pos, cond, bodyT, bodyF);
         }

         return StatementCreate_If(std::move(labels), pos, cond, bodyT);
      }

      //
      // GetStatement_return
      //
      static AST::Statement::CRef GetStatement_return(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         // <return> expression(opt) ;

         // <return>
         auto pos = ctx.in.get().pos;

         // ;
         if(ctx.in.drop(Core::TOK_Semico))
            return StatementCreate_Return(std::move(labels), pos, scope.fn);

         // expression
         auto exp = ctx.getExp(scope);

         // ;
         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return StatementCreate_Return(std::move(labels), pos, scope.fn, exp);
      }

      //
      // GetStatement_switch
      //
      static AST::Statement::CRef GetStatement_switch(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &switchScope = scope.createScopeCase();

         // <switch> ( expression ) statement

         // <switch>
         auto pos = ctx.in.get().pos;

         // ( expression )
         auto cond = ctx.getStatementCond(switchScope);

         // statement
         auto body = ctx.getStatement(switchScope);

         return StatementCreate_Switch(std::move(labels), pos, switchScope,
            cond, body);
      }

      //
      // GetStatement_while
      //
      static AST::Statement::CRef GetStatement_while(Parser &ctx,
         Scope_Local &scope, Core::Array<Core::String> &labels)
      {
         auto &loopScope = scope.createScopeLoop();

         // <while> ( expression ) statement

         // <while>
         auto pos = ctx.in.get().pos;

         // ( expression )
         auto cond = ctx.getStatementCond(loopScope);

         // statement
         auto body = ctx.getStatement(loopScope);

         return StatementCreate_While(std::move(labels), pos, loopScope, cond, body);
      }

      //
      // GetStatementLabel
      //
      static Core::Array<Core::String> GetStatementLabel(Parser &ctx,
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

                  auto val = ExpToInteg(ctx.getExp_Cond(scope));

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
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Parser::getStatement
      //
      AST::Statement::CRef Parser::getStatement(Scope_Local &scope)
      {
         auto labels = GetStatementLabel(*this, scope);

         // compound-statement
         if(in.peek().tok == Core::TOK_BraceO)
            return GetStatement_Compound(*this, scope, labels);

         if(in.peek(Core::TOK_KeyWrd) || in.peek(Core::TOK_Identi))
            switch(in.peek().str)
         {
            // selection-statement
         case Core::STR_if:     return GetStatement_if    (*this, scope, labels);
         case Core::STR_switch: return GetStatement_switch(*this, scope, labels);

            // iteration-statement
         case Core::STR_while: return GetStatement_while(*this, scope, labels);
         case Core::STR_do:    return GetStatement_do   (*this, scope, labels);
         case Core::STR_for:   return GetStatement_for  (*this, scope, labels);

            // jump-statement:
         case Core::STR_goto:     return GetStatement_goto    (*this, scope, labels);
         case Core::STR_continue: return GetStatement_continue(*this, scope, labels);
         case Core::STR_break:    return GetStatement_break   (*this, scope, labels);
         case Core::STR_return:   return GetStatement_return  (*this, scope, labels);

            // asm-statement:
         case Core::STR___asm: return GetStatement_asm(*this, scope, labels);

         default: break;
         }

         // expression-statement
         return GetStatement_Exp(*this, scope, labels);
      }

      //
      // Parse::getStatementCond
      //
      AST::Exp::CRef Parser::getStatementCond(Scope &scope)
      {
         // (
         if(!in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(in.peek(), "(", true);

         // expression
         auto cond = getStatementCondExp(scope);

         // )
         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         return cond;
      }

      //
      // Parse::getStatementCondExp
      //
      AST::Exp::CRef Parser::getStatementCondExp(Scope &scope)
      {
         bool parenPre = in.peek(Core::TOK_ParenO);

         // expression
         auto cond = getExp(scope);

         bool parenPro = in.reget().tok == Core::TOK_ParenC;

         // This check could be more elegant with a more direct check for a
         // parenthesized expression. This will fail to warn for: if((x) = (y))
         if(!(parenPre && parenPro) && dynamic_cast<Exp_Assign const *>(&*cond))
         {
            AST::WarnParentheses(cond->pos,
               "assignment as a condition without parentheses");
         }

         return cond;
      }
   }
}

// EOF

