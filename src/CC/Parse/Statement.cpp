//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
#include "CC/Factory.hpp"
#include "CC/Scope/Case.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Type.hpp"

#include "AS/LabelTBuf.hpp"
#include "AS/TStream.hpp"

#include "Core/Exception.hpp"
#include "Core/StringBuf.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Exp.hpp"
#include "SR/Function.hpp"
#include "SR/Statement.hpp"
#include "SR/Type.hpp"
#include "SR/Warning.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::getLabels
   //
   Parser::Labels Parser::getLabels(Scope_Local &scope)
   {
      std::vector<Core::String> labels;

      for(;;)
      {
         auto tok = in.peek();

         // Keyword label?
         if(tok.tok == Core::TOK_KeyWrd)
         {
            // <case> constant-expression :
            if(tok.str == Core::STR_case)
            {
               in.get();

               auto val = ExpToInteg(getExp_Cond(scope));

               expect(Core::TOK_Colon);

               auto label = scope.getLabelCase(val, true);

               if(!label)
                  Core::Error(tok.pos, "case redefined");

               labels.emplace_back(label);
            }

            // <default> :
            else if(tok.str == Core::STR_default)
            {
               in.get();

               expect(Core::TOK_Colon);

               auto label = scope.getLabelDefault(true);

               if(!label)
                  Core::Error(tok.pos, "default redefined");

               labels.emplace_back(label);
            }

            else
               break;
         }

         // identifier :
         else if(tok.tok == Core::TOK_Identi)
         {
            in.get();
            if(in.drop(Core::TOK_Colon))
            {
               auto label = scope.getLabel(tok.str);

               if(!label)
                  Core::Error(tok.pos, "label redefined");

               labels.emplace_back(label);
            }
            else
            {
               in.unget();
               break;
            }
         }

         else
            break;
      }

      return {labels.begin(), labels.end()};
   }

   //
   // Parser::getStatement
   //
   SR::Statement::CRef Parser::getStatement(Scope_Local &scope)
   {
      return getStatement(scope, getLabels(scope));
   }

   //
   // Parser::getStatement
   //
   SR::Statement::CRef Parser::getStatement(Scope_Local &scope, Labels &&labels)
   {
      // compound-statement
      if(in.peek().tok == Core::TOK_BraceO)
         return getStatementCompound(scope, std::move(labels));

      if(in.peek(Core::TOK_KeyWrd) || in.peek(Core::TOK_Identi))
         switch(in.peek().str)
      {
         // selection-statement
      case Core::STR_if:     return getStatement_if    (scope, std::move(labels));
      case Core::STR_switch: return getStatement_switch(scope, std::move(labels));

         // iteration-statement
      case Core::STR_while: return getStatement_while(scope, std::move(labels));
      case Core::STR_do:    return getStatement_do   (scope, std::move(labels));
      case Core::STR_for:   return getStatement_for  (scope, std::move(labels));

         // jump-statement
      case Core::STR_goto:     return getStatement_goto    (scope, std::move(labels));
      case Core::STR_continue: return getStatement_continue(scope, std::move(labels));
      case Core::STR_break:    return getStatement_break   (scope, std::move(labels));
      case Core::STR_return:   return getStatement_return  (scope, std::move(labels));

         // asm-statement
      case Core::STR___asm: return getStatement_asm(scope, std::move(labels));

         // with-statement
      case Core::STR___with: return getStatement_with(scope, std::move(labels));

      default: break;
      }

      // expression-statement
      return getStatementExp(scope, std::move(labels));
   }

   //
   // Parser::getStatement_asm
   //
   SR::Statement::CRef Parser::getStatement_asm(Scope_Local &scope, Labels &&labels)
   {
      // <__asm> ( string-literal ) ;
      auto pos = in.get().pos;
      expect(Core::TOK_ParenO);
      auto tok = expectString();
      expect(Core::TOK_ParenC);
      expect(Core::TOK_Semico);

      // Convert string to a series of assembly tokens.
      Core::StringBuf sbuf{tok.str.data(), tok.str.size()};
      AS::TStream     tstr{sbuf, tok.pos.file, tok.pos.line};
      AS::LabelTBuf   ltb {*tstr.tkbuf(), scope.fn.fn->glyph};
      AsmGlyphTBuf    gtb {ltb, scope};
      tstr.tkbuf(&gtb);

      std::vector<Core::Token> tokens;
      while(tstr >> tok) tokens.push_back(tok);
      tokens.emplace_back(tok.pos, nullptr, Core::TOK_EOF);

      return fact.stCreate_Asm(std::move(labels), pos,
         {Core::Move, tokens.begin(), tokens.end()});
   }

   //
   // Parser::getStatement_break
   //
   SR::Statement::CRef Parser::getStatement_break(Scope_Local &scope, Labels &&labels)
   {
      // <break> ;
      auto pos = in.get().pos;
      expect(Core::TOK_Semico);

      return fact.stCreate_Break(std::move(labels), pos, scope);
   }

   //
   // Parser::getStatement_continue
   //
   SR::Statement::CRef Parser::getStatement_continue(
      Scope_Local &scope, Labels &&labels)
   {
      // <continue> ;
      auto pos = in.get().pos;
      expect(Core::TOK_Semico);

      return fact.stCreate_Continue(std::move(labels), pos, scope);
   }

   //
   // Parser::getStatement_do
   //
   SR::Statement::CRef Parser::getStatement_do(Scope_Local &scope, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <do> statement <while> ( expression ) ;

      // <do>
      auto pos = in.get().pos;

      // statement
      auto body = getStatement(loopScope);

      // <while>
      expect(Core::TOK_KeyWrd, Core::STR_while);

      // ( expression )
      auto cond = getStatementCond(loopScope);

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Do(std::move(labels), pos, loopScope, body, cond);
   }

   //
   // Parser::getStatement_for
   //
   SR::Statement::CRef Parser::getStatement_for(Scope_Local &scope, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <for> ( expression(opt) ; expression(opt) ; expression(opt) )
      // <for> ( declaration expression(opt) ; expression(opt) )

      // <for>
      auto pos = in.get().pos;

      // (
      expect(Core::TOK_ParenO);

      SR::Statement::CPtr init;
      // declaration
      if(isDecl(loopScope))
      {
         init = getDecl(loopScope);
      }
      // expression(opt) ;
      else
      {
         // expression(opt)
         if(in.peek().tok != Core::TOK_Semico)
            init = fact.stCreate_Exp({}, getExp(loopScope));
         else
            init = fact.stCreate_Empty({}, pos);

         // ;
         expect(Core::TOK_Semico);
      }

      // expression(opt)
      SR::Exp::CPtr cond;
      if(in.peek().tok != Core::TOK_Semico)
         cond = getStatementCondExp(loopScope);
      else
         cond = fact.expCreate_LitInt(TypeIntegPrS, 1, pos);

      // ;
      expect(Core::TOK_Semico);

      // expression(opt)
      SR::Statement::CPtr iter;
      if(in.peek().tok != Core::TOK_ParenC)
         iter = fact.stCreate_Exp({}, getExp(loopScope));
      else
         iter = fact.stCreate_Empty({}, pos);

      // )
      expect(Core::TOK_ParenC);

      // statement
      auto body = getStatement(loopScope);

      return fact.stCreate_For(std::move(labels), pos, loopScope, init,
         cond, iter, body);
   }

   //
   // Parser::getStatement_goto
   //
   SR::Statement::CRef Parser::getStatement_goto(Scope_Local &scope, Labels &&labels)
   {
      // <goto> identifier ;
      // <goto> * cast-expression ;

      // <goto>
      auto pos = in.get().pos;

      // * cast-expression ;
      if(in.drop(Core::TOK_Mul))
      {
         auto exp = getExp_Cast(scope);
         expect(Core::TOK_Semico);

         return fact.stCreate_Goto(std::move(labels), pos, exp);
      }

      // identifier
      auto name = expectIdenti().str;

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Goto(std::move(labels), pos, scope, name);
   }

   //
   // Parser::getStatement_if
   //
   SR::Statement::CRef Parser::getStatement_if(Scope_Local &scope, Labels &&labels)
   {
      // <if> ( expression ) statement
      // <if> ( expression ) statement <else> statement

      // <if>
      auto pos = in.get().pos;

      // ( expression )
      auto cond = getStatementCond(scope);

      // statement
      auto bodyT = getStatement(scope);

      // <else> statement
      if(in.drop(Core::TOK_KeyWrd, Core::STR_else))
      {
         // statement
         auto bodyF = getStatement(scope);

         return fact.stCreate_If(std::move(labels), pos, cond, bodyT, bodyF);
      }

      return fact.stCreate_If(std::move(labels), pos, cond, bodyT);
   }

   //
   // Parser::getStatement_return
   //
   SR::Statement::CRef Parser::getStatement_return(Scope_Local &scope, Labels &&labels)
   {
      // <return> expression(opt) ;

      // <return>
      auto pos = in.get().pos;

      // ;
      if(in.drop(Core::TOK_Semico))
         return fact.stCreate_Return(std::move(labels), pos, scope.fn);

      // expression
      auto exp = getExp(scope);

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Return(std::move(labels), pos, scope.fn, exp);
   }

   //
   // Parser::getStatement_switch
   //
   SR::Statement::CRef Parser::getStatement_switch(Scope_Local &scope, Labels &&labels)
   {
      auto &switchScope = scope.createScopeCase();

      // <switch> ( expression ) statement

      // <switch>
      auto pos = in.get().pos;

      // ( expression )
      auto cond = getStatementCond(switchScope);

      // statement
      auto body = getStatement(switchScope);

      return fact.stCreate_Switch(std::move(labels), pos, switchScope, cond, body);
   }

   //
   // Parser::getStatement_while
   //
   SR::Statement::CRef Parser::getStatement_while(Scope_Local &scope, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <while> ( expression ) statement

      // <while>
      auto pos = in.get().pos;

      // ( expression )
      auto cond = getStatementCond(loopScope);

      // statement
      auto body = getStatement(loopScope);

      return fact.stCreate_While(std::move(labels), pos, loopScope, cond, body);
   }

   //
   // Parser::getStatement_with
   //
   SR::Statement::CRef Parser::getStatement_with(Scope_Local &scope, Labels &&labels)
   {
      auto &withScope = scope.createScopeBlock();

      // with-statement:
      //    <__with> ( with-declaration-sequence(opt) ) statement

      // <__with>
      auto pos = in.get().pos;

      // (
      expect(Core::TOK_ParenO);

      // with-declaration-sequence:
      //    with-declaration
      //    with-declaration-sequence with-declaration
      std::vector<SR::Statement::CRef> stmnts;
      while(!in.peek(Core::TOK_ParenC))
      {
         // with-declaration:
         //    declaration
         //    expression ;

         // declaration
         if(isDecl(withScope))
         {
            stmnts.push_back(getDecl(withScope));
         }
         // expression ;
         else
         {
            // expression
            stmnts.push_back(fact.stCreate_Exp({}, getExp(withScope)));

            // ;
            expect(Core::TOK_Semico);
         }
      }

      // )
      expect(Core::TOK_ParenC);

      // statement
      stmnts.push_back(getStatement(withScope));

      return fact.stCreate_Multi(std::move(labels), pos,
         {stmnts.begin(), stmnts.end()});
   }

   //
   // Parser::getStatementCompound
   //
   SR::Statement::CRef Parser::getStatementCompound(Scope_Local &scope, Labels &&labels)
   {
      // compound-statement:
      //    { block-item-list(opt) }

      // {
      auto pos = in.get().pos;

      std::vector<SR::Statement::CRef> stmnts;
      auto &blockScope = scope.createScopeBlock();

      // block-item-list:
      //    block-item
      //    block-item-list block-item
      while(!in.drop(Core::TOK_BraceC))
      {
         // block-item:
         //    declaration
         //    statement

         // declaration
         if(isDecl(blockScope))
            stmnts.emplace_back(getDecl(blockScope));

         // statement
         else
            stmnts.emplace_back(getStatement(blockScope));
      }

      // }

      return fact.stCreate_Multi(std::move(labels), pos,
         {stmnts.begin(), stmnts.end()});
   }

   //
   // Parse::getStatementCond
   //
   SR::Exp::CRef Parser::getStatementCond(Scope &scope)
   {
      // (
      expect(Core::TOK_ParenO);

      // expression
      auto cond = getStatementCondExp(scope);

      // )
      expect(Core::TOK_ParenC);

      return cond;
   }

   //
   // Parse::getStatementCondExp
   //
   SR::Exp::CRef Parser::getStatementCondExp(Scope &scope)
   {
      bool parenPre = in.peek(Core::TOK_ParenO);

      // expression
      auto cond = getExp(scope);

      bool parenPro = in.reget().tok == Core::TOK_ParenC;

      // This check could be more elegant with a more direct check for a
      // parenthesized expression. This will fail to warn for: if((x) = (y))
      if(!(parenPre && parenPro) && dynamic_cast<Exp_Assign const *>(&*cond))
      {
         SR::WarnParentheses(cond->pos,
            "assignment as a condition without parentheses");
      }

      return cond;
   }

   //
   // Parser::GetStatementExp
   //
   SR::Statement::CRef Parser::getStatementExp(Scope_Local &scope, Labels &&labels)
   {
      // expression-statement:
      //    expression(opt) ;

      auto pos = in.peek().pos;

      // ;
      if(in.drop(Core::TOK_Semico))
         return fact.stCreate_Empty(std::move(labels), pos);

      // expression
      auto exp = getExp(scope);

      // ;
      expect(Core::TOK_Semico);

      if(!exp->isEffect() && !exp->getType()->isTypeVoid())
         SR::WarnUnusedValue(exp->pos, "expression result unused");

      return fact.stCreate_Exp(std::move(labels), pos, exp);
   }
}

// EOF

