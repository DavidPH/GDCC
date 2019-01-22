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

#include "SR/Attribute.hpp"
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
   Parser::Labels Parser::getLabels(Scope_Local &scope, SR::Attribute &attr)
   {
      std::vector<Core::String> labels;

      for(;;)
      {
         if(isAttrSpec(scope))
            parseAttrSpecList(scope, attr);

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

               attr.clear();
               attr.linka = defLinkage();
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

               attr.clear();
               attr.linka = defLinkage();
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

               attr.clear();
               attr.linka = defLinkage();
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
   // Parser::getSt
   //
   SR::Statement::CRef Parser::getSt(Scope_Local &scope)
   {
      SR::Attribute attr;
      attr.linka = defLinkage();
      auto labels = getLabels(scope, attr);
      return getSt(scope, std::move(attr), std::move(labels));
   }

   //
   // Parser::getSt
   //
   SR::Statement::CRef Parser::getSt(Scope_Local &scope,
      SR::Attribute &&attr, Labels &&labels)
   {
      // compound-statement
      if(in.peek().tok == Core::TOK_BraceO)
         return getStCompound(scope, std::move(attr), std::move(labels));

      if(in.peek(Core::TOK_KeyWrd) || in.peek(Core::TOK_Identi))
         switch(in.peek().str)
      {
         // selection-statement
      case Core::STR_if:     return getSt_if    (scope, std::move(attr), std::move(labels));
      case Core::STR_switch: return getSt_switch(scope, std::move(attr), std::move(labels));

         // iteration-statement
      case Core::STR_while: return getSt_while(scope, std::move(attr), std::move(labels));
      case Core::STR_do:    return getSt_do   (scope, std::move(attr), std::move(labels));
      case Core::STR_for:   return getSt_for  (scope, std::move(attr), std::move(labels));

         // jump-statement
      case Core::STR_goto:     return getSt_goto    (scope, std::move(attr), std::move(labels));
      case Core::STR_continue: return getSt_continue(scope, std::move(attr), std::move(labels));
      case Core::STR_break:    return getSt_break   (scope, std::move(attr), std::move(labels));
      case Core::STR_return:   return getSt_return  (scope, std::move(attr), std::move(labels));

         // asm-statement
      case Core::STR___asm: return getSt_asm(scope, std::move(attr), std::move(labels));

         // with-statement
      case Core::STR___with: return getSt_with(scope, std::move(attr), std::move(labels));

      default: break;
      }

      // expression-statement
      return getStExp(scope, std::move(attr), std::move(labels));
   }

   //
   // Parser::getSt_asm
   //
   SR::Statement::CRef Parser::getSt_asm(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
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
   // Parser::getSt_break
   //
   SR::Statement::CRef Parser::getSt_break(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      // <break> ;
      auto pos = in.get().pos;
      expect(Core::TOK_Semico);

      return fact.stCreate_Break(std::move(labels), pos, scope);
   }

   //
   // Parser::getSt_continue
   //
   SR::Statement::CRef Parser::getSt_continue(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      // <continue> ;
      auto pos = in.get().pos;
      expect(Core::TOK_Semico);

      return fact.stCreate_Continue(std::move(labels), pos, scope);
   }

   //
   // Parser::getSt_do
   //
   SR::Statement::CRef Parser::getSt_do(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <do> statement <while> ( expression ) ;

      // <do>
      auto pos = in.get().pos;

      // statement
      auto body = getSt(loopScope);

      // <while>
      expect(Core::TOK_KeyWrd, Core::STR_while);

      // ( expression )
      auto cond = getStCond(loopScope);

      // ;
      expect(Core::TOK_Semico);

      return fact.stCreate_Do(std::move(labels), pos, loopScope, body, cond);
   }

   //
   // Parser::getSt_for
   //
   SR::Statement::CRef Parser::getSt_for(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
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
      if(isAttrSpec(loopScope) || isDecl(loopScope))
      {
         SR::Attribute attr;
         attr.linka = defLinkage();
         if(isAttrSpec(scope))
            parseAttrSpecList(loopScope, attr);
         init = getDecl(loopScope, std::move(attr), {});
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
         cond = getStCondExp(loopScope);
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
      auto body = getSt(loopScope);

      return fact.stCreate_For(std::move(labels), pos, loopScope, init,
         cond, iter, body);
   }

   //
   // Parser::getSt_goto
   //
   SR::Statement::CRef Parser::getSt_goto(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
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
   // Parser::getSt_if
   //
   SR::Statement::CRef Parser::getSt_if(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      // <if> ( expression ) statement
      // <if> ( expression ) statement <else> statement

      // <if>
      auto pos = in.get().pos;

      // ( expression )
      auto cond = getStCond(scope);

      // statement
      auto bodyT = getSt(scope);

      // <else> statement
      if(in.drop(Core::TOK_KeyWrd, Core::STR_else))
      {
         // statement
         auto bodyF = getSt(scope);

         return fact.stCreate_If(std::move(labels), pos, cond, bodyT, bodyF);
      }

      return fact.stCreate_If(std::move(labels), pos, cond, bodyT);
   }

   //
   // Parser::getSt_return
   //
   SR::Statement::CRef Parser::getSt_return(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
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
   // Parser::getSt_switch
   //
   SR::Statement::CRef Parser::getSt_switch(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      auto &switchScope = scope.createScopeCase();

      // <switch> ( expression ) statement

      // <switch>
      auto pos = in.get().pos;

      // ( expression )
      auto cond = getStCond(switchScope);

      // statement
      auto body = getSt(switchScope);

      return fact.stCreate_Switch(std::move(labels), pos, switchScope, cond, body);
   }

   //
   // Parser::getSt_while
   //
   SR::Statement::CRef Parser::getSt_while(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      auto &loopScope = scope.createScopeLoop();

      // <while> ( expression ) statement

      // <while>
      auto pos = in.get().pos;

      // ( expression )
      auto cond = getStCond(loopScope);

      // statement
      auto body = getSt(loopScope);

      return fact.stCreate_While(std::move(labels), pos, loopScope, cond, body);
   }

   //
   // Parser::getSt_with
   //
   SR::Statement::CRef Parser::getSt_with(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
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
         if(isAttrSpec(withScope) || isDecl(withScope))
         {
            SR::Attribute attr;
            attr.linka = defLinkage();
            if(isAttrSpec(scope))
               parseAttrSpecList(withScope, attr);
            stmnts.push_back(getDecl(withScope, std::move(attr), {}));
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
      stmnts.push_back(getSt(withScope));

      return fact.stCreate_Multi(std::move(labels), pos,
         {stmnts.begin(), stmnts.end()});
   }

   //
   // Parser::getStCompound
   //
   SR::Statement::CRef Parser::getStCompound(Scope_Local &scope,
      SR::Attribute &&, Labels &&blockLabels)
   {
      // compound-statement:
      //    { block-item-list(opt) }

      // {
      if(!in.peek(Core::TOK_BraceO))
         Core::ErrorExpect("compound statement", in.peek());
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

         SR::Attribute attr;
         attr.linka = defLinkage();
         auto labels = getLabels(blockScope, attr);

         // declaration
         if(isDecl(blockScope))
            stmnts.emplace_back(getDecl(blockScope, std::move(attr), std::move(labels)));

         // statement
         else
            stmnts.emplace_back(getSt(blockScope, std::move(attr), std::move(labels)));
      }

      // }

      return fact.stCreate_Multi(std::move(blockLabels), pos,
         {stmnts.begin(), stmnts.end()});
   }

   //
   // Parse::getStCond
   //
   SR::Exp::CRef Parser::getStCond(Scope &scope)
   {
      // (
      expect(Core::TOK_ParenO);

      // expression
      auto cond = getStCondExp(scope);

      // )
      expect(Core::TOK_ParenC);

      return cond;
   }

   //
   // Parse::getStCondExp
   //
   SR::Exp::CRef Parser::getStCondExp(Scope &scope)
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
   // Parser::getStExp
   //
   SR::Statement::CRef Parser::getStExp(Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
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

