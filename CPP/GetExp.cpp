//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR expression parsing.
//
//-----------------------------------------------------------------------------

#include "CPP/GetExp.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // TypeFlt
   //
   static IR::Type_Float TypeFlt()
   {
      return IR::Type_Float(23, 8, true, false);
   }

   //
   // TypeFltL
   //
   static IR::Type_Float TypeFltL()
   {
      return IR::Type_Float(52, 11, true, false);
   }

   //
   // TypeFltLL
   //
   static IR::Type_Float TypeFltLL()
   {
      return IR::Type_Float(52, 11, true, false);
   }

   //
   // TypeIntMax
   //
   static IR::Type_Fixed TypeIntMax()
   {
      return IR::Type_Fixed(63, 0, true, false);
   }

   //
   // TypeUIntMax
   //
   static IR::Type_Fixed TypeUIntMax()
   {
      return IR::Type_Fixed(64, 0, false, false);
   }

   //
   // Promote
   //
   static std::pair<IR::Exp::CRef, IR::Exp::CRef>
   Promote(IR::Exp const *l_, IR::Exp const *r_)
   {
      IR::Exp::CRef l{l_}, r{r_};

      auto lt = l->getType(), rt = r->getType();

      // There should only be Fixed and Float values passing through here.

      // Fixed <-> ???
      if(lt.t == IR::TypeBase::Fixed)
      {
         // Fixed <-> Fixed
         if(rt.t == IR::TypeBase::Fixed)
         {
            // Promote to accum/fract from int.
            if(!lt.tFixed.bitsF && rt.tFixed.bitsF)
               l = IR::ExpCreate_Cst(rt, l);
            else if(lt.tFixed.bitsF && !rt.tFixed.bitsF)
               r = IR::ExpCreate_Cst(lt, r);

            // Promote to the most integral bits.
            else if(lt.tFixed.bitsI < rt.tFixed.bitsI)
               l = IR::ExpCreate_Cst(rt, l);
            else
               r = IR::ExpCreate_Cst(lt, r);
         }

         // Fixed <-> Float
         else if(rt.t == IR::TypeBase::Float)
         {
            // Fixed always promotes to Float.
            l = IR::ExpCreate_Cst(rt, l);
         }
      }

      // Float <-> ???
      else if(lt.t == IR::TypeBase::Float)
      {
         // Float <-> Fixed
         if(rt.t == IR::TypeBase::Fixed)
         {
            // Fixed always promotes to Float.
            r = IR::ExpCreate_Cst(lt, r);
         }

         // Float <-> Float
         else if(rt.t == IR::TypeBase::Float)
         {
            // Promote to the most integral bits.
            if(lt.tFloat.bitsI < rt.tFloat.bitsI)
               l = IR::ExpCreate_Cst(rt, l);
            else
               r = IR::ExpCreate_Cst(lt, r);
         }
      }

      return {l, r};
   }

   //
   // GetExp_Prim_NumInt
   //
   static IR::Exp::CRef GetExp_Prim_NumInt(Core::Token const &tok)
   {
      char const *itr = tok.str.begin();
      unsigned base;
      Core::Integ val;
      bool u = false;

      std::tie(itr, base) = Core::ParseNumberBaseC(itr);
      std::tie(itr, val, std::ignore) = Core::ParseNumberInteg(itr, base);

      // Detect unsigned.
      for(auto end = tok.str.end(); itr != end; ++itr)
         if(*itr == 'U' || *itr == 'u') {u = true; break;}

      // TODO: Check for oversized hex/octal literal.

      return IR::ExpCreate_Value(
         IR::Value_Fixed(val, u ? TypeUIntMax() : TypeIntMax()), tok.pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // GetExp_Prim_NumFix
   //
   IR::Exp::CRef GetExp_Prim_NumFix(Core::Token const &tok)
   {
      char const *itr = tok.str.begin();
      unsigned base;
      Core::Ratio val;
      IR::Type_Fixed t;

      bool u = false, h = false, l = false, k = false, r = false;

      std::tie(itr, base) = Core::ParseNumberBaseC(itr);
      std::tie(itr, val, std::ignore) = Core::ParseNumberRatioC(itr, base);

           if(*itr == 'U' || *itr == 'u') u = true, ++itr;
           if(*itr == 'H' || *itr == 'h') h = true, ++itr;
      else if(*itr == 'L' || *itr == 'l') l = true, ++itr;
           if(*itr == 'K' || *itr == 'k') k = true, ++itr;
      else if(*itr == 'R' || *itr == 'r') r = true, ++itr;

      if(r)
      {
         t.bitsI = 0;

              if(h) {t.bitsF = 15 + u;}
         else if(l) {t.bitsF = 31 + u;}
         else       {t.bitsF = 15 + u;}
      }
      else if(k)
      {
              if(h) {t.bitsI = 15 + u; t.bitsF = 16;}
         else if(l) {t.bitsI = 31 + u; t.bitsF = 32;}
         else       {t.bitsI = 15 + u; t.bitsF = 16;}
      }
      else
      {
         // Shouldn't get here, but just in case.
         Core::Error(tok.pos, "expected k or r suffix");
      }

      t.bitsS = !u;
      t.satur = false;

      val <<= t.bitsF;
      Core::Integ v{std::move(val)};
      return IR::ExpCreate_Value(IR::Value_Fixed(std::move(v), t), tok.pos);
   }

   //
   // GetExp_Prim_NumFlt
   //
   IR::Exp::CRef GetExp_Prim_NumFlt(Core::Token const &tok)
   {
      char const *itr = tok.str.begin();
      unsigned base;
      Core::Ratio val;
      IR::Type_Float t;

      std::tie(itr, base) = Core::ParseNumberBaseC(itr);
      std::tie(itr, val, std::ignore) = Core::ParseNumberRatioC(itr, base);

           if(*itr == 'F' || *itr == 'f') t = TypeFlt();
      else if(*itr == 'L' || *itr == 'l') t = TypeFltLL();
      else                                t = TypeFltL();

      Core::Float v{std::move(val)};
      return IR::ExpCreate_Value(IR::Value_Float(std::move(v), t), tok.pos);
   }

   //
   // GetExp_Prim
   //
   IR::Exp::CRef GetExp_Prim(Core::TokenStream &in)
   {
      auto tok = in.get(); switch(tok.tok)
      {
      case Core::TOK_Charac:
         return IR::ExpCreate_Value(
            IR::Value_Fixed(*tok.str.begin(), TypeIntMax()), tok.pos);

      case Core::TOK_NumFix:
         return GetExp_Prim_NumFix(tok);

      case Core::TOK_NumFlt:
         return GetExp_Prim_NumFlt(tok);

      case Core::TOK_NumInt:
         return GetExp_Prim_NumInt(tok);

      case Core::TOK_String:
         Core::Error(tok.pos, "string-literal in constant-expression");

      case Core::TOK_ParenO:
         if(auto exp = GetExp(in); (tok = in.get()).tok == Core::TOK_ParenC)
            return exp;

         Core::ErrorExpect(")", tok, true);

      default:
         Core::ErrorExpect("primary-expression", tok);
      }
   }

   //
   // GetExp_Post
   //
   IR::Exp::CRef GetExp_Post(Core::TokenStream &in)
   {
      IR::Exp::CRef e = GetExp_Prim(in);

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_Add2:
         Core::Error(tok.pos, "increment in constant-expression");

      case Core::TOK_Dot:
         Core::Error(tok.pos, "member in constant-expression");

      case Core::TOK_Mem:
         Core::Error(tok.pos, "member in constant-expression");

      case Core::TOK_Sub2:
         Core::Error(tok.pos, "decrement in constant-expression");

      case Core::TOK_BrackO:
         Core::Error(tok.pos, "subscript in constant-expression");

      case Core::TOK_ParenO:
         Core::Error(tok.pos, "call in constant-expression");

      default: in.unget(); return e;
      }
   }

   //
   // GetExp_Unar
   //
   IR::Exp::CRef GetExp_Unar(Core::TokenStream &in)
   {
      auto tok = in.get(); switch(tok.tok)
      {
      case Core::TOK_Add:
         return GetExp_Unar(in);

      case Core::TOK_Add2:
         Core::Error(tok.pos, "increment in constant-expression");

      case Core::TOK_And:
         Core::Error(tok.pos, "address-of in constant-expression");

      case Core::TOK_Inv:
         return IR::ExpCreate_Inv(GetExp_Unar(in), tok.pos);

      case Core::TOK_Mul:
         Core::Error(tok.pos, "indirection in constant-expression");

      case Core::TOK_Not:
         return IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_Not(GetExp_Unar(in), tok.pos));

      case Core::TOK_Sub:
         return IR::ExpCreate_Neg(GetExp_Unar(in), tok.pos);

      case Core::TOK_Sub2:
         Core::Error(tok.pos, "decrement in constant-expression");

      default: in.unget(); return GetExp_Post(in);
      }
   }

   //
   // GetExp_Mult
   //
   IR::Exp::CRef GetExp_Mult(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_Unar(in); IR::Exp::CPtr r;

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_Div:
         std::tie(l, r) = Promote(l, GetExp_Unar(in));
         l = IR::ExpCreate_Div(l, r, tok.pos);
         break;

      case Core::TOK_Mod:
         std::tie(l, r) = Promote(l, GetExp_Unar(in));
         l = IR::ExpCreate_Mod(l, r, tok.pos);
         break;

      case Core::TOK_Mul:
         std::tie(l, r) = Promote(l, GetExp_Unar(in));
         l = IR::ExpCreate_Mul(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_Addi
   //
   IR::Exp::CRef GetExp_Addi(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_Mult(in); IR::Exp::CPtr r;

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_Add:
         std::tie(l, r) = Promote(l, GetExp_Mult(in));
         l = IR::ExpCreate_Add(l, r, tok.pos);
         break;

      case Core::TOK_Sub:
         std::tie(l, r) = Promote(l, GetExp_Mult(in));
         l = IR::ExpCreate_Sub(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_Shft
   //
   IR::Exp::CRef GetExp_Shft(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_Addi(in);

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_ShL:
         l = IR::ExpCreate_ShL(l, GetExp_Addi(in), tok.pos);
         break;

      case Core::TOK_ShR:
         l = IR::ExpCreate_ShR(l, GetExp_Addi(in), tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_Rela
   //
   IR::Exp::CRef GetExp_Rela(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_Shft(in); IR::Exp::CPtr r;

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_CmpGE:
         std::tie(l, r) = Promote(l, GetExp_Shft(in));
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_CmpGE(l, r, tok.pos));
         break;

      case Core::TOK_CmpGT:
         std::tie(l, r) = Promote(l, GetExp_Shft(in));
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_CmpGT(l, r, tok.pos));
         break;

      case Core::TOK_CmpLE:
         std::tie(l, r) = Promote(l, GetExp_Shft(in));
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_CmpLE(l, r, tok.pos));
         break;

      case Core::TOK_CmpLT:
         std::tie(l, r) = Promote(l, GetExp_Shft(in));
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_CmpLT(l, r, tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_Equa
   //
   IR::Exp::CRef GetExp_Equa(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_Rela(in); IR::Exp::CPtr r;

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_CmpEQ:
         std::tie(l, r) = Promote(l, GetExp_Rela(in));
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_CmpEQ(l, r, tok.pos));
         break;

      case Core::TOK_CmpNE:
         std::tie(l, r) = Promote(l, GetExp_Rela(in));
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_CmpNE(l, r, tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_BAnd
   //
   IR::Exp::CRef GetExp_BAnd(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_Equa(in); IR::Exp::CPtr r;

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_And:
         std::tie(l, r) = Promote(l, GetExp_Equa(in));
         l = IR::ExpCreate_BitAnd(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_BOrX
   //
   IR::Exp::CRef GetExp_BOrX(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_BAnd(in); IR::Exp::CPtr r;

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_OrX:
         std::tie(l, r) = Promote(l, GetExp_BAnd(in));
         l = IR::ExpCreate_BitOrX(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_BOrI
   //
   IR::Exp::CRef GetExp_BOrI(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_BOrX(in); IR::Exp::CPtr r;

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_OrI:
         std::tie(l, r) = Promote(l, GetExp_BOrX(in));
         l = IR::ExpCreate_BitOrI(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_LAnd
   //
   IR::Exp::CRef GetExp_LAnd(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_BOrI(in);

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_And2:
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_LogAnd(l, GetExp_BOrI(in), tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_LOrI
   //
   IR::Exp::CRef GetExp_LOrI(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_LAnd(in);

      for(Core::Token tok;;) switch((tok = in.get()).tok)
      {
      case Core::TOK_OrI2:
         l = IR::ExpCreate_Cst(TypeIntMax(),
            IR::ExpCreate_LogOrI(l, GetExp_LAnd(in), tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExp_Cond
   //
   IR::Exp::CRef GetExp_Cond(Core::TokenStream &in)
   {
      IR::Exp::CRef c = GetExp_LOrI(in); IR::Exp::CPtr l, r;

      auto tok = in.get(); switch(tok.tok)
      {
      case Core::TOK_Query:
         l = GetExp(in);

         if(!in.drop(Core::TOK_Colon))
            Core::ErrorExpect(":", in.peek(), true);

         std::tie(l, r) = Promote(l, GetExp_Cond(in));
         return IR::ExpCreate_Cnd(c, l, r, tok.pos);

      default: in.unget(); return c;
      }
   }

   //
   // GetExp_Assi
   //
   IR::Exp::CRef GetExp_Assi(Core::TokenStream &in)
   {
      IR::Exp::CRef l = GetExp_Cond(in);

      auto tok = in.get(); switch(tok.tok)
      {
      case Core::TOK_AddEq:
      case Core::TOK_AndEq:
      case Core::TOK_DivEq:
      case Core::TOK_Equal:
      case Core::TOK_ModEq:
      case Core::TOK_MulEq:
      case Core::TOK_OrXEq:
      case Core::TOK_OrIEq:
      case Core::TOK_ShLEq:
      case Core::TOK_ShREq:
      case Core::TOK_SubEq:
         Core::Error(tok.pos, "assignment in constant-expression");

      default: in.unget(); return l;
      }
   }

   //
   // GetExp
   //
   IR::Exp::CRef GetExp(Core::TokenStream &in)
   {
      IR::Exp::CRef e = GetExp_Assi(in);

      while(in.peek().tok == Core::TOK_Comma)
         in.get(), e = GetExp_Assi(in);

      return e;
   }
}

// EOF

