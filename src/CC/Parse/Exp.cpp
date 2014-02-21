//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "AST/Exp.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define DeclExpCreate(getter1, getter2) \
   auto exp = getter1(in, ctx); \
   \
   auto expCreate = [&](AST::Exp::CRef (*creator)(AST::Exp const *, \
      AST::Exp const *, Core::Origin)) \
   { \
      auto pos = in.in.get().pos; \
      return creator(exp, getter2(in, ctx), pos); \
   }


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_Mult
      //
      AST::Exp::CRef GetExp_Mult(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Cast, GetExp_Cast);

         for(;;) switch(in.in.peek().tok)
         {
         case Core::TOK_Mul: exp = expCreate(ExpCreate_Mul); break;
         case Core::TOK_Div: exp = expCreate(ExpCreate_Div); break;
         case Core::TOK_Mod: exp = expCreate(ExpCreate_Mod); break;

         default:
            return exp;
         }

      }

      //
      // GetExp_Addi
      //
      AST::Exp::CRef GetExp_Addi(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Mult, GetExp_Mult);

         for(;;) switch(in.in.peek().tok)
         {
         case Core::TOK_Add: exp = expCreate(ExpCreate_Add); break;
         case Core::TOK_Sub: exp = expCreate(ExpCreate_Sub); break;

         default:
            return exp;
         }

      }

      //
      // GetExp_Shft
      //
      AST::Exp::CRef GetExp_Shft(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Addi, GetExp_Addi);

         for(;;) switch(in.in.peek().tok)
         {
         case Core::TOK_ShL: exp = expCreate(ExpCreate_ShL); break;
         case Core::TOK_ShR: exp = expCreate(ExpCreate_ShR); break;

         default:
            return exp;
         }

      }

      //
      // GetExp_Rela
      //
      AST::Exp::CRef GetExp_Rela(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Shft, GetExp_Shft);

         for(;;) switch(in.in.peek().tok)
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
      // GetExp_Equa
      //
      AST::Exp::CRef GetExp_Equa(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Rela, GetExp_Rela);

         for(;;) switch(in.in.peek().tok)
         {
         case Core::TOK_CmpEQ: exp = expCreate(ExpCreate_CmpEQ); break;
         case Core::TOK_CmpNE: exp = expCreate(ExpCreate_CmpNE); break;

         default:
            return exp;
         }
      }

      //
      // GetExp_BAnd
      //
      AST::Exp::CRef GetExp_BAnd(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Equa, GetExp_Equa);

         while(in.in.peek().tok == Core::TOK_And)
            exp = expCreate(ExpCreate_BitAnd);

         return exp;
      }

      //
      // GetExp_BOrX
      //
      AST::Exp::CRef GetExp_BOrX(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_BAnd, GetExp_BAnd);

         while(in.in.peek().tok == Core::TOK_OrX)
            exp = expCreate(ExpCreate_BitOrX);

         return exp;
      }

      //
      // GetExp_BOrI
      //
      AST::Exp::CRef GetExp_BOrI(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_BOrX, GetExp_BOrX);

         while(in.in.peek().tok == Core::TOK_OrI)
            exp = expCreate(ExpCreate_BitOrI);

         return exp;
      }

      //
      // GetExp_LAnd
      //
      AST::Exp::CRef GetExp_LAnd(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_BOrI, GetExp_BOrI);

         while(in.in.peek().tok == Core::TOK_And2)
            exp = expCreate(ExpCreate_LogAnd);

         return exp;
      }

      //
      // GetExp_LOrI
      //
      AST::Exp::CRef GetExp_LOrI(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_LAnd, GetExp_LAnd);

         while(in.in.peek().tok == Core::TOK_OrI2)
            exp = expCreate(ExpCreate_LogOrI);

         return exp;
      }

      //
      // GetExp_Cond
      //
      AST::Exp::CRef GetExp_Cond(ParserData &in, Scope *ctx)
      {
         auto exp = GetExp_LOrI(in, ctx);

         if(in.in.peek().tok == Core::TOK_Query)
         {
            auto pos = in.in.get().pos;

            auto expT = GetExp(in, ctx);

            if(!in.in.drop(Core::TOK_Colon))
               throw Core::ExceptStr(in.in.peek().pos, "expected ':'");

            return ExpCreate_Cnd(exp, expT, GetExp_Cond(in, ctx), pos);
         }

         return exp;
      }

      //
      // GetExp_Assi
      //
      AST::Exp::CRef GetExp_Assi(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Cond, GetExp_Assi);

         switch(in.in.peek().tok)
         {
         case Core::TOK_Equal: return expCreate(ExpCreate_Equal);
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
      // GetExp
      //
      AST::Exp::CRef GetExp(ParserData &in, Scope *ctx)
      {
         DeclExpCreate(GetExp_Assi, GetExp_Assi);

         while(in.in.peek().tok == Core::TOK_Comma)
            exp = expCreate(ExpCreate_Comma);

         return exp;
      }
   }
}

// EOF

