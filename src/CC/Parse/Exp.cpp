//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Exp.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define DeclExpCreate(getter1, getter2) \
   auto exp = getter1(scope); \
   \
   auto expCreate = [&](SR::Exp::CRef (*creator)(SR::Exp const *, \
      SR::Exp const *, Core::Origin)) \
   { \
      auto pos = in.get().pos; \
      return creator(exp, getter2(scope), pos); \
   }


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Parser::getExp_Mult
      //
      SR::Exp::CRef Parser::getExp_Mult(Scope &scope)
      {
         DeclExpCreate(getExp_Cast, getExp_Cast);

         for(;;) switch(in.peek().tok)
         {
         case Core::TOK_Mul: exp = expCreate(ExpCreate_Mul); break;
         case Core::TOK_Div: exp = expCreate(ExpCreate_Div); break;
         case Core::TOK_Mod: exp = expCreate(ExpCreate_Mod); break;

         default:
            return exp;
         }

      }

      //
      // Parser::getExp_Addi
      //
      SR::Exp::CRef Parser::getExp_Addi(Scope &scope)
      {
         DeclExpCreate(getExp_Mult, getExp_Mult);

         for(;;) switch(in.peek().tok)
         {
         case Core::TOK_Add: exp = expCreate(ExpCreate_Add); break;
         case Core::TOK_Sub: exp = expCreate(ExpCreate_Sub); break;

         default:
            return exp;
         }
      }

      //
      // Parser::getExp_Shft
      //
      SR::Exp::CRef Parser::getExp_Shft(Scope &scope)
      {
         DeclExpCreate(getExp_Addi, getExp_Addi);

         for(;;) switch(in.peek().tok)
         {
         case Core::TOK_ShL: exp = expCreate(ExpCreate_ShL); break;
         case Core::TOK_ShR: exp = expCreate(ExpCreate_ShR); break;

         default:
            return exp;
         }
      }

      //
      // Parser::getExp_Rela
      //
      SR::Exp::CRef Parser::getExp_Rela(Scope &scope)
      {
         DeclExpCreate(getExp_Shft, getExp_Shft);

         for(;;) switch(in.peek().tok)
         {
         case Core::TOK_CmpLT: exp = expCreate(ExpCreate_CmpLT); break;
         case Core::TOK_CmpGT: exp = expCreate(ExpCreate_CmpGT); break;
         case Core::TOK_CmpLE: exp = expCreate(ExpCreate_CmpLE); break;
         case Core::TOK_CmpGE: exp = expCreate(ExpCreate_CmpGE); break;

         default:
            return exp;
         }
      }

      //
      // Parser::getExp_Equa
      //
      SR::Exp::CRef Parser::getExp_Equa(Scope &scope)
      {
         DeclExpCreate(getExp_Rela, getExp_Rela);

         for(;;) switch(in.peek().tok)
         {
         case Core::TOK_CmpEQ: exp = expCreate(ExpCreate_CmpEQ); break;
         case Core::TOK_CmpNE: exp = expCreate(ExpCreate_CmpNE); break;

         default:
            return exp;
         }
      }

      //
      // Parser::getExp_BAnd
      //
      SR::Exp::CRef Parser::getExp_BAnd(Scope &scope)
      {
         DeclExpCreate(getExp_Equa, getExp_Equa);

         while(in.peek().tok == Core::TOK_And)
            exp = expCreate(ExpCreate_BitAnd);

         return exp;
      }

      //
      // Parser::getExp_BOrX
      //
      SR::Exp::CRef Parser::getExp_BOrX(Scope &scope)
      {
         DeclExpCreate(getExp_BAnd, getExp_BAnd);

         while(in.peek().tok == Core::TOK_OrX)
            exp = expCreate(ExpCreate_BitOrX);

         return exp;
      }

      //
      // Parser::getExp_BOrI
      //
      SR::Exp::CRef Parser::getExp_BOrI(Scope &scope)
      {
         DeclExpCreate(getExp_BOrX, getExp_BOrX);

         while(in.peek().tok == Core::TOK_OrI)
            exp = expCreate(ExpCreate_BitOrI);

         return exp;
      }

      //
      // Parser::getExp_LAnd
      //
      SR::Exp::CRef Parser::getExp_LAnd(Scope &scope)
      {
         DeclExpCreate(getExp_BOrI, getExp_BOrI);

         while(in.peek().tok == Core::TOK_And2)
            exp = expCreate(ExpCreate_LogAnd);

         return exp;
      }

      //
      // Parser::getExp_LOrI
      //
      SR::Exp::CRef Parser::getExp_LOrI(Scope &scope)
      {
         DeclExpCreate(getExp_LAnd, getExp_LAnd);

         while(in.peek().tok == Core::TOK_OrI2)
            exp = expCreate(ExpCreate_LogOrI);

         return exp;
      }

      //
      // Parser::getExp_Cond
      //
      SR::Exp::CRef Parser::getExp_Cond(Scope &scope)
      {
         auto exp = getExp_LOrI(scope);

         if(in.peek().tok == Core::TOK_Query)
         {
            auto pos = in.get().pos;

            auto expT = getExp(scope);

            if(!in.drop(Core::TOK_Colon))
               throw Core::ExceptStr(in.peek().pos, "expected ':'");

            return ExpCreate_Cnd(exp, expT, getExp_Cond(scope), pos);
         }

         return exp;
      }

      //
      // Parser::getExp_Assi
      //
      SR::Exp::CRef Parser::getExp_Assi(Scope &scope)
      {
         DeclExpCreate(getExp_Cond, getExp_Assi);

         switch(in.peek().tok)
         {
         case Core::TOK_Equal: return expCreate(ExpCreate_Assign);
         case Core::TOK_MulEq: return expCreate(ExpCreate_MulEq);
         case Core::TOK_DivEq: return expCreate(ExpCreate_DivEq);
         case Core::TOK_ModEq: return expCreate(ExpCreate_ModEq);
         case Core::TOK_AddEq: return expCreate(ExpCreate_AddEq);
         case Core::TOK_SubEq: return expCreate(ExpCreate_SubEq);
         case Core::TOK_ShLEq: return expCreate(ExpCreate_ShLEq);
         case Core::TOK_ShREq: return expCreate(ExpCreate_ShREq);
         case Core::TOK_AndEq: return expCreate(ExpCreate_BitAndEq);
         case Core::TOK_OrXEq: return expCreate(ExpCreate_BitOrXEq);
         case Core::TOK_OrIEq: return expCreate(ExpCreate_BitOrIEq);

         default:
            return exp;
         }
      }

      //
      // Parser::getExp
      //
      SR::Exp::CRef Parser::getExp(Scope &scope)
      {
         DeclExpCreate(getExp_Assi, getExp_Assi);

         while(in.peek().tok == Core::TOK_Comma)
            exp = expCreate(ExpCreate_Comma);

         return exp;
      }

      //
      // Parser::getExpList
      //
      Core::Array<SR::Exp::CRef> Parser::getExpList(CC::Scope &scope)
      {
         std::vector<SR::Exp::CRef> args;

         do
            args.emplace_back(getExp_Assi(scope));
         while(in.drop(Core::TOK_Comma));

         return {Core::Move, args.begin(), args.end()};
      }
   }
}

// EOF

