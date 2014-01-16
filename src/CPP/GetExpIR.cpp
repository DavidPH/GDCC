//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR expression parsing.
//
//-----------------------------------------------------------------------------

#include "CPP/GetExpIR.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// TypeFlt
//
static GDCC::IR::Type_Float TypeFlt()
{
   return GDCC::IR::Type_Float(23, 8, true, false);
}

//
// TypeFltL
//
static GDCC::IR::Type_Float TypeFltL()
{
   return GDCC::IR::Type_Float(52, 11, true, false);
}

//
// TypeFltLL
//
static GDCC::IR::Type_Float TypeFltLL()
{
   return GDCC::IR::Type_Float(52, 11, true, false);
}

//
// TypeIntMax
//
static GDCC::IR::Type_Fixed TypeIntMax()
{
   return GDCC::IR::Type_Fixed(63, 0, true, false);
}

//
// TypeUIntMax
//
static GDCC::IR::Type_Fixed TypeUIntMax()
{
   return GDCC::IR::Type_Fixed(64, 0, false, false);
}

//
// Promote
//
static std::pair<GDCC::IR::Exp::CRef, GDCC::IR::Exp::CRef>
Promote(GDCC::IR::Exp const *l_, GDCC::IR::Exp const *r_)
{
   GDCC::IR::Exp::CRef l{l_}, r{r_};

   auto lt = l->getType(), rt = r->getType();

   // There should only be Fixed and Float values passing through here.

   // Fixed <-> ???
   if(lt.t == GDCC::IR::TypeBase::Fixed)
   {
      // Fixed <-> Fixed
      if(rt.t == GDCC::IR::TypeBase::Fixed)
      {
         // Promote to accum/fract from int.
         if(!lt.tFixed.bitsF && rt.tFixed.bitsF)
            l = GDCC::IR::ExpCreate_UnaryCst(rt, l);
         else if(lt.tFixed.bitsF && !rt.tFixed.bitsF)
            r = GDCC::IR::ExpCreate_UnaryCst(lt, r);

         // Promote to the most integral bits.
         else if(lt.tFixed.bitsI < rt.tFixed.bitsI)
            l = GDCC::IR::ExpCreate_UnaryCst(rt, l);
         else
            r = GDCC::IR::ExpCreate_UnaryCst(lt, r);
      }

      // Fixed <-> Float
      else if(rt.t == GDCC::IR::TypeBase::Float)
      {
         // Fixed always promotes to Float.
         l = GDCC::IR::ExpCreate_UnaryCst(rt, l);
      }
   }

   // Float <-> ???
   else if(lt.t == GDCC::IR::TypeBase::Float)
   {
      // Float <-> Fixed
      if(rt.t == GDCC::IR::TypeBase::Fixed)
      {
         // Fixed always promotes to Float.
         r = GDCC::IR::ExpCreate_UnaryCst(lt, r);
      }

      // Float <-> Float
      else if(rt.t == GDCC::IR::TypeBase::Float)
      {
         // Promote to the most integral bits.
         if(lt.tFloat.bitsI < rt.tFloat.bitsI)
            l = GDCC::IR::ExpCreate_UnaryCst(rt, l);
         else
            r = GDCC::IR::ExpCreate_UnaryCst(lt, r);
      }
   }

   return {l, r};
}

//
// GetExpIR_Prim_NumInt
//
static GDCC::IR::Exp::CRef GetExpIR_Prim_NumInt(GDCC::Core::Token const &tok)
{
   char const *itr = tok.str.begin();
   unsigned base;
   GDCC::Core::Integ val;
   bool u = false;

   std::tie(itr, base) = GDCC::Core::ParseNumberBaseC(itr);
   std::tie(itr, val, std::ignore) = GDCC::Core::ParseNumberInteg(itr, base);

   // Detect unsigned.
   for(auto end = tok.str.end(); itr != end; ++itr)
      if(*itr == 'U' || *itr == 'u') {u = true; break;}

   // TODO: Check for oversized hex/octal literal.

   return GDCC::IR::ExpCreate_ValueRoot(
      GDCC::IR::Value_Fixed(val, u ? TypeUIntMax() : TypeIntMax()), tok.pos);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // GetExpIR_Prim_NumFix
      //
      IR::Exp::CRef GetExpIR_Prim_NumFix(Core::Token const &tok)
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
            throw Core::ExceptStr(tok.pos, "expected k or r suffix");
         }

         t.bitsS = !u;
         t.satur = false;

         val <<= t.bitsF;
         Core::Integ v{std::move(val)};
         return IR::ExpCreate_ValueRoot(
            IR::Value_Fixed(std::move(v), t), tok.pos);
      }

      //
      // GetExpIR_Prim_NumFlt
      //
      IR::Exp::CRef GetExpIR_Prim_NumFlt(Core::Token const &tok)
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
         return IR::ExpCreate_ValueRoot(
            IR::Value_Float(std::move(v), t), tok.pos);
      }

      //
      // GetExpIR_Prim
      //
      IR::Exp::CRef GetExpIR_Prim(Core::TokenStream &in)
      {
         auto tok = in.get(); switch(tok.tok)
         {
         case Core::TOK_Charac:
            return IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(*tok.str.begin(), TypeIntMax()), tok.pos);

         case Core::TOK_NumFix:
            return GetExpIR_Prim_NumFix(tok);

         case Core::TOK_NumFlt:
            return GetExpIR_Prim_NumFlt(tok);

         case Core::TOK_NumInt:
            return GetExpIR_Prim_NumInt(tok);

         case Core::TOK_String:
            std::cerr << "ERROR: " << tok.pos << ": string-literal in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_ParenO:
            if(auto exp = GetExpIR(in)) if((tok = in.get()).tok == Core::TOK_ParenC)
               return exp;

            std::cerr << "ERROR: " << tok.pos << ": expected )\n";
            throw EXIT_FAILURE;

         default:
            std::cerr << "ERROR: " << tok.pos << ": expected primary-expression\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // GetExpIR_Post
      //
      IR::Exp::CRef GetExpIR_Post(Core::TokenStream &in)
      {
         IR::Exp::CRef e = GetExpIR_Prim(in);

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_Add2:
            std::cerr << "ERROR: " << tok.pos << ": increment in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_Dot:
            std::cerr << "ERROR: " << tok.pos << ": member in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_Mem:
            std::cerr << "ERROR: " << tok.pos << ": member in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_Sub2:
            std::cerr << "ERROR: " << tok.pos << ": decrement in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_BrackO:
            std::cerr << "ERROR: " << tok.pos << ": subscript in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_ParenO:
            std::cerr << "ERROR: " << tok.pos << ": call in constant-expression\n";
            throw EXIT_FAILURE;

         default: in.unget(); return e;
         }
      }

      //
      // GetExpIR_Unar
      //
      IR::Exp::CRef GetExpIR_Unar(Core::TokenStream &in)
      {
         auto tok = in.get(); switch(tok.tok)
         {
         case Core::TOK_Add:
            return IR::ExpCreate_UnaryAdd(GetExpIR_Unar(in), tok.pos);

         case Core::TOK_Add2:
            std::cerr << "ERROR: " << tok.pos << ": increment in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_And:
            std::cerr << "ERROR: " << tok.pos << ": address-of in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_Inv:
            return IR::ExpCreate_UnaryNot(GetExpIR_Unar(in), tok.pos);

         case Core::TOK_Mul:
            std::cerr << "ERROR: " << tok.pos << ": indirection in constant-expression\n";
            throw EXIT_FAILURE;

         case Core::TOK_Not:
            return IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchNot(GetExpIR_Unar(in), tok.pos));

         case Core::TOK_Sub:
            return IR::ExpCreate_UnarySub(GetExpIR_Unar(in), tok.pos);

         case Core::TOK_Sub2:
            std::cerr << "ERROR: " << tok.pos << ": decrement in constant-expression\n";
            throw EXIT_FAILURE;

         default: in.unget(); return GetExpIR_Post(in);
         }
      }

      //
      // GetExpIR_Mult
      //
      IR::Exp::CRef GetExpIR_Mult(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_Unar(in); IR::Exp::CPtr r;

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_Div:
            std::tie(l, r) = Promote(l, GetExpIR_Unar(in));
            l = IR::ExpCreate_BinaryDiv(l, r, tok.pos);
            break;

         case Core::TOK_Mod:
            std::tie(l, r) = Promote(l, GetExpIR_Unar(in));
            l = IR::ExpCreate_BinaryMod(l, r, tok.pos);
            break;

         case Core::TOK_Mul:
            std::tie(l, r) = Promote(l, GetExpIR_Unar(in));
            l = IR::ExpCreate_BinaryMul(l, r, tok.pos);
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_Addi
      //
      IR::Exp::CRef GetExpIR_Addi(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_Mult(in); IR::Exp::CPtr r;

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_Add:
            std::tie(l, r) = Promote(l, GetExpIR_Mult(in));
            l = IR::ExpCreate_BinaryAdd(l, r, tok.pos);
            break;

         case Core::TOK_Sub:
            std::tie(l, r) = Promote(l, GetExpIR_Mult(in));
            l = IR::ExpCreate_BinarySub(l, r, tok.pos);
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_Shft
      //
      IR::Exp::CRef GetExpIR_Shft(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_Addi(in);

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_ShL:
            l = IR::ExpCreate_BinaryShL(l, GetExpIR_Addi(in), tok.pos);
            break;

         case Core::TOK_ShR:
            l = IR::ExpCreate_BinaryShR(l, GetExpIR_Addi(in), tok.pos);
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_Rela
      //
      IR::Exp::CRef GetExpIR_Rela(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_Shft(in); IR::Exp::CPtr r;

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_CmpGE:
            std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchCmpGE(l, r, tok.pos));
            break;

         case Core::TOK_CmpGT:
            std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchCmpGT(l, r, tok.pos));
            break;

         case Core::TOK_CmpLE:
            std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchCmpLE(l, r, tok.pos));
            break;

         case Core::TOK_CmpLT:
            std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchCmpLT(l, r, tok.pos));
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_Equa
      //
      IR::Exp::CRef GetExpIR_Equa(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_Rela(in); IR::Exp::CPtr r;

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_CmpEQ:
            std::tie(l, r) = Promote(l, GetExpIR_Rela(in));
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchCmpEQ(l, r, tok.pos));
            break;

         case Core::TOK_CmpNE:
            std::tie(l, r) = Promote(l, GetExpIR_Rela(in));
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchCmpNE(l, r, tok.pos));
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_BAnd
      //
      IR::Exp::CRef GetExpIR_BAnd(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_Equa(in); IR::Exp::CPtr r;

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_And:
            std::tie(l, r) = Promote(l, GetExpIR_Equa(in));
            l = IR::ExpCreate_BinaryAnd(l, r, tok.pos);
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_BOrX
      //
      IR::Exp::CRef GetExpIR_BOrX(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_BAnd(in); IR::Exp::CPtr r;

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_OrX:
            std::tie(l, r) = Promote(l, GetExpIR_BAnd(in));
            l = IR::ExpCreate_BinaryOrX(l, r, tok.pos);
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_BOrI
      //
      IR::Exp::CRef GetExpIR_BOrI(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_BOrX(in); IR::Exp::CPtr r;

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_OrI:
            std::tie(l, r) = Promote(l, GetExpIR_BOrX(in));
            l = IR::ExpCreate_BinaryOrI(l, r, tok.pos);
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_LAnd
      //
      IR::Exp::CRef GetExpIR_LAnd(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_BOrI(in);

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_And2:
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchAnd(l, GetExpIR_BOrI(in), tok.pos));
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_LOrI
      //
      IR::Exp::CRef GetExpIR_LOrI(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_LAnd(in);

         for(Core::Token tok;;) switch((tok = in.get()).tok)
         {
         case Core::TOK_OrI2:
            l = IR::ExpCreate_UnaryCst(TypeIntMax(),
               IR::ExpCreate_BranchOrI(l, GetExpIR_LAnd(in), tok.pos));
            break;

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR_Cond
      //
      IR::Exp::CRef GetExpIR_Cond(Core::TokenStream &in)
      {
         IR::Exp::CRef c = GetExpIR_LOrI(in); IR::Exp::CPtr l, r;

         auto tok = in.get(); switch(tok.tok)
         {
         case Core::TOK_Query:
            l = GetExpIR(in);

            if(!in.drop(Core::TOK_Colon))
            {
               std::cerr << "ERROR: " << in.peek().pos << ": expected :\n";
               throw EXIT_FAILURE;
            }

            std::tie(l, r) = Promote(l, GetExpIR_Cond(in));
            return IR::ExpCreate_BranchCnd(c, l, r, tok.pos);

         default: in.unget(); return c;
         }
      }

      //
      // GetExpIR_Assi
      //
      IR::Exp::CRef GetExpIR_Assi(Core::TokenStream &in)
      {
         IR::Exp::CRef l = GetExpIR_Cond(in);

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
            std::cerr << "ERROR: " << tok.pos << ": assignment in constant-expression\n";

         default: in.unget(); return l;
         }
      }

      //
      // GetExpIR
      //
      IR::Exp::CRef GetExpIR(Core::TokenStream &in)
      {
         IR::Exp::CRef e = GetExpIR_Assi(in);

         while(in.peek().tok == Core::TOK_Comma)
            in.get(), e = GetExpIR_Assi(in);

         return e;
      }
   }
}

// EOF

