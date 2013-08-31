//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR expression parsing.
//
//-----------------------------------------------------------------------------

#include "GetExpIR.hpp"

#include "GDCC/Parse.hpp"
#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Exp.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

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
static std::pair<GDCC::IR::Exp::Ref, GDCC::IR::Exp::Ref>
Promote(GDCC::IR::Exp *l_, GDCC::IR::Exp *r_)
{
   GDCC::IR::Exp::Ref l{l_}, r{r_};

   auto lt = l->getType(), rt = r->getType();

   // There should only be Fixed and Float values passing through here.

   // Fixed <-> ???
   if(lt.t == GDCC::IR::TypeBase::Fixed)
   {
      // Fixed <-> Fixed
      if(rt.t == GDCC::IR::TypeBase::Fixed)
      {
         // Promote to the most integral bits.
         if(lt.tFixed.bitsI < rt.tFixed.bitsI)
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
static GDCC::IR::Exp::Ref GetExpIR_Prim_NumInt(GDCC::Token const &tok)
{
   char const *itr = tok.str.begin();
   unsigned base;
   GDCC::Integ val;
   bool u = false;

   std::tie(itr, base) = GDCC::ParseNumberBaseC(itr);
   std::tie(itr, val, std::ignore) = GDCC::ParseNumberInteg(itr, base);

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

namespace C
{
   //
   // GetExpIR_Prim
   //
   GDCC::IR::Exp::Ref GetExpIR_Prim(GDCC::TokenStream &in)
   {
      auto tok = in.get(); switch(tok.tok)
      {
      case GDCC::TOK_NumInt:
         return GetExpIR_Prim_NumInt(tok);

      case GDCC::TOK_String:
         std::cerr << "ERROR: " << tok.pos << ": string-literal in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_ParenO:
         if(auto exp = GetExpIR(in)) if((tok = in.get()).tok == GDCC::TOK_ParenC)
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
   GDCC::IR::Exp::Ref GetExpIR_Post(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref e = GetExpIR_Prim(in);

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_Add2:
         std::cerr << "ERROR: " << tok.pos << ": increment in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_Dot:
         std::cerr << "ERROR: " << tok.pos << ": member in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_Mem:
         std::cerr << "ERROR: " << tok.pos << ": member in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_Sub2:
         std::cerr << "ERROR: " << tok.pos << ": decrement in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_BrackO:
         std::cerr << "ERROR: " << tok.pos << ": subscript in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_ParenO:
         std::cerr << "ERROR: " << tok.pos << ": call in constant-expression\n";
         throw EXIT_FAILURE;

      default: in.unget(); return e;
      }
   }

   //
   // GetExpIR_Unar
   //
   GDCC::IR::Exp::Ref GetExpIR_Unar(GDCC::TokenStream &in)
   {
      auto tok = in.get(); switch(tok.tok)
      {
      case GDCC::TOK_Add:
         return GDCC::IR::ExpCreate_UnaryAdd(GetExpIR_Unar(in), tok.pos);

      case GDCC::TOK_Add2:
         std::cerr << "ERROR: " << tok.pos << ": increment in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_And:
         std::cerr << "ERROR: " << tok.pos << ": address-of in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_Inv:
         return GDCC::IR::ExpCreate_UnaryNot(GetExpIR_Unar(in), tok.pos);

      case GDCC::TOK_Mul:
         std::cerr << "ERROR: " << tok.pos << ": indirection in constant-expression\n";
         throw EXIT_FAILURE;

      case GDCC::TOK_Not:
         return GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchNot(GetExpIR_Unar(in), tok.pos));

      case GDCC::TOK_Sub:
         return GDCC::IR::ExpCreate_UnarySub(GetExpIR_Unar(in), tok.pos);

      case GDCC::TOK_Sub2:
         std::cerr << "ERROR: " << tok.pos << ": decrement in constant-expression\n";
         throw EXIT_FAILURE;

      default: in.unget(); return GetExpIR_Post(in);
      }
   }

   //
   // GetExpIR_Mult
   //
   GDCC::IR::Exp::Ref GetExpIR_Mult(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_Unar(in); GDCC::IR::Exp::Ptr r;

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_Div:
         std::tie(l, r) = Promote(l, GetExpIR_Unar(in));
         l = GDCC::IR::ExpCreate_BinaryDiv(l, r, tok.pos);
         break;

      case GDCC::TOK_Mod:
         std::tie(l, r) = Promote(l, GetExpIR_Unar(in));
         l = GDCC::IR::ExpCreate_BinaryMod(l, r, tok.pos);
         break;

      case GDCC::TOK_Mul:
         std::tie(l, r) = Promote(l, GetExpIR_Unar(in));
         l = GDCC::IR::ExpCreate_BinaryMul(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_Addi
   //
   GDCC::IR::Exp::Ref GetExpIR_Addi(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_Mult(in); GDCC::IR::Exp::Ptr r;

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_Add:
         std::tie(l, r) = Promote(l, GetExpIR_Mult(in));
         l = GDCC::IR::ExpCreate_BinaryAdd(l, r, tok.pos);
         break;

      case GDCC::TOK_Sub:
         std::tie(l, r) = Promote(l, GetExpIR_Mult(in));
         l = GDCC::IR::ExpCreate_BinarySub(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_Shft
   //
   GDCC::IR::Exp::Ref GetExpIR_Shft(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_Addi(in);

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_ShL:
         l = GDCC::IR::ExpCreate_BinaryShL(l, GetExpIR_Addi(in), tok.pos);
         break;

      case GDCC::TOK_ShR:
         l = GDCC::IR::ExpCreate_BinaryShR(l, GetExpIR_Addi(in), tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_Rela
   //
   GDCC::IR::Exp::Ref GetExpIR_Rela(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_Shft(in); GDCC::IR::Exp::Ptr r;

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_CmpGE:
         std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchCmpGE(l, r, tok.pos));
         break;

      case GDCC::TOK_CmpGT:
         std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchCmpGT(l, r, tok.pos));
         break;

      case GDCC::TOK_CmpLE:
         std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchCmpLE(l, r, tok.pos));
         break;

      case GDCC::TOK_CmpLT:
         std::tie(l, r) = Promote(l, GetExpIR_Shft(in));
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchCmpLT(l, r, tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_Equa
   //
   GDCC::IR::Exp::Ref GetExpIR_Equa(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_Rela(in); GDCC::IR::Exp::Ptr r;

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_CmpEQ:
         std::tie(l, r) = Promote(l, GetExpIR_Rela(in));
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchCmpEQ(l, r, tok.pos));
         break;

      case GDCC::TOK_CmpNE:
         std::tie(l, r) = Promote(l, GetExpIR_Rela(in));
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchCmpNE(l, r, tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_BAnd
   //
   GDCC::IR::Exp::Ref GetExpIR_BAnd(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_Equa(in); GDCC::IR::Exp::Ptr r;

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_And:
         std::tie(l, r) = Promote(l, GetExpIR_Equa(in));
         l = GDCC::IR::ExpCreate_BinaryAnd(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_BOrX
   //
   GDCC::IR::Exp::Ref GetExpIR_BOrX(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_BAnd(in); GDCC::IR::Exp::Ptr r;

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_OrX:
         std::tie(l, r) = Promote(l, GetExpIR_BAnd(in));
         l = GDCC::IR::ExpCreate_BinaryOrX(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_BOrI
   //
   GDCC::IR::Exp::Ref GetExpIR_BOrI(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_BOrX(in); GDCC::IR::Exp::Ptr r;

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_OrI:
         std::tie(l, r) = Promote(l, GetExpIR_BOrX(in));
         l = GDCC::IR::ExpCreate_BinaryOrI(l, r, tok.pos);
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_LAnd
   //
   GDCC::IR::Exp::Ref GetExpIR_LAnd(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_BOrI(in);

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_And2:
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchAnd(l, GetExpIR_BOrI(in), tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_LOrI
   //
   GDCC::IR::Exp::Ref GetExpIR_LOrI(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_LAnd(in);

      for(GDCC::Token tok;;) switch((tok = in.get()).tok)
      {
      case GDCC::TOK_OrI2:
         l = GDCC::IR::ExpCreate_UnaryCst(TypeIntMax(),
            GDCC::IR::ExpCreate_BranchOrI(l, GetExpIR_LAnd(in), tok.pos));
         break;

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR_Cond
   //
   GDCC::IR::Exp::Ref GetExpIR_Cond(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref c = GetExpIR_LOrI(in); GDCC::IR::Exp::Ptr l, r;

      auto tok = in.get(); switch(tok.tok)
      {
      case GDCC::TOK_Query:
         l = GetExpIR(in);

         if(!in.drop(GDCC::TOK_Colon))
         {
            std::cerr << "ERROR: " << in.peek().pos << ": expected :\n";
            throw EXIT_FAILURE;
         }

         std::tie(l, r) = Promote(l, GetExpIR_Cond(in));
         return GDCC::IR::ExpCreate_BranchCnd(c, l, r, tok.pos);

      default: in.unget(); return c;
      }
   }

   //
   // GetExpIR_Assi
   //
   GDCC::IR::Exp::Ref GetExpIR_Assi(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref l = GetExpIR_Cond(in);

      auto tok = in.get(); switch(tok.tok)
      {
      case GDCC::TOK_AddEq:
      case GDCC::TOK_AndEq:
      case GDCC::TOK_DivEq:
      case GDCC::TOK_Equal:
      case GDCC::TOK_ModEq:
      case GDCC::TOK_MulEq:
      case GDCC::TOK_OrXEq:
      case GDCC::TOK_OrIEq:
      case GDCC::TOK_ShLEq:
      case GDCC::TOK_ShREq:
      case GDCC::TOK_SubEq:
         std::cerr << "ERROR: " << tok.pos << ": assignment in constant-expression\n";

      default: in.unget(); return l;
      }
   }

   //
   // GetExpIR
   //
   GDCC::IR::Exp::Ref GetExpIR(GDCC::TokenStream &in)
   {
      GDCC::IR::Exp::Ref e = GetExpIR_Assi(in);

      while(in.peek().tok == GDCC::TOK_Comma)
         in.get(), e = GetExpIR_Assi(in);

      return e;
   }
}

// EOF

