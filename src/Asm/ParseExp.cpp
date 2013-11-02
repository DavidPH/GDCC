//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Expression parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "GDCC/Parse.hpp"
#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Exp.hpp"
#include "GDCC/IR/Glyph.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ParseSuffix
//
static std::tuple<char const */*end*/, bool /*fixed*/, bool /*bitsS*/,
   GDCC::FastU /*bitsI*/, GDCC::FastU /*bitsF*/, bool /*satur*/>
ParseSuffix(char const *s, GDCC::Token const &tok)
{
   GDCC::FastU bitsF;
   GDCC::FastU bitsI;
   bool        bitsS;
   bool        fixed;
   bool        satur;

   fixed = *s == 'F' || *s == 'f' ? ++s, false : true;
   bitsS = *s == 'S' || *s == 's' ? ++s, true : false;
   std::tie(s, bitsI, std::ignore) = GDCC::ParseNumberFastU(s, 10);

   if(*s != '.')
   {
      std::cerr << "ERROR: " << tok.pos << ": bad number: '" << tok.str << "'\n";
      throw EXIT_FAILURE;
   }

   std::tie(s, bitsF, std::ignore) = GDCC::ParseNumberFastU(++s, 10);
   satur = *s == 'S' || *s == 's' ? ++s, true : false;

   return std::make_tuple(s, fixed, bitsS, bitsI, bitsF, satur);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseExp
   //
   GDCC::IR::Exp::Ref ParseExp(GDCC::TokenStream &in)
   {
      #define doE1(name, token) case GDCC::token: \
         return GDCC::IR::ExpCreate_##name(ParseExp(in), tok.pos)

      #define doE2(name, token) case GDCC::token: \
         l = ParseExp(in); r = ParseExp(in); \
         return GDCC::IR::ExpCreate_##name(l, r, tok.pos)

      #define doE3(name, token) case GDCC::token: \
         c = ParseExp(in); l = ParseExp(in); r = ParseExp(in); \
         return GDCC::IR::ExpCreate_##name(c, l, r, tok.pos)

      GDCC::IR::Exp::Ptr c, l, r;
      GDCC::IR::Type t;

      GDCC::Token tok;
      switch((in >> tok, tok).tok)
      {
      case GDCC::TOK_Identi:
         switch(static_cast<GDCC::StringIndex>(tok.str))
         {
         case GDCC::STR_cast:
            t = ParseType(in);
            return GDCC::IR::ExpCreate_UnaryCst(t, ParseExp(in), tok.pos);

         default:
            std::cerr << "ERROR: " << tok.pos << ": expected expression\n";
            throw EXIT_FAILURE;
         }

      case GDCC::TOK_Number:
         return GDCC::IR::ExpCreate_ValueRoot(ParseNumber(tok), tok.pos);

      case GDCC::TOK_String:
         return GDCC::IR::ExpCreate_ValueGlyph(static_cast<GDCC::IR::Glyph>(tok.str), tok.pos);

         doE2(BinaryAdd, TOK_Add);
         doE2(BinaryAnd, TOK_And);
         doE2(BinaryDiv, TOK_Div);
         doE2(BinaryMod, TOK_Mod);
         doE2(BinaryMul, TOK_Mul);
         doE2(BinaryOrI, TOK_OrI);
         doE2(BinaryOrX, TOK_OrX);
         doE2(BinaryShL, TOK_ShL);
         doE2(BinaryShR, TOK_ShR);
         doE2(BinarySub, TOK_Sub);

         doE2(BranchAnd, TOK_And2);
         doE2(BranchCmpEQ, TOK_CmpEQ);
         doE2(BranchCmpGE, TOK_CmpGE);
         doE2(BranchCmpGT, TOK_CmpGT);
         doE2(BranchCmpLE, TOK_CmpLE);
         doE2(BranchCmpLT, TOK_CmpLT);
         doE2(BranchCmpNE, TOK_CmpNE);
         doE3(BranchCnd, TOK_Query);
         doE1(BranchNot, TOK_Not);
         doE2(BranchOrI, TOK_OrI2);
         doE2(BranchOrX, TOK_OrX2);

         doE1(UnaryAdd, TOK_Add2);
         doE1(UnaryNot, TOK_Inv);
         doE1(UnarySub, TOK_Sub2);

      case GDCC::TOK_BraceO:
         in.unget();
         return ParseExpMulti(in);

      case GDCC::TOK_BrackO:
         in.unget();
         return GDCC::IR::ExpCreate_ValueRoot(ParseMulti(in), tok.pos);

      default:
         std::cerr << "ERROR: " << tok.pos << ": expected expression\n";
         throw EXIT_FAILURE;
      }

      #undef doE3
      #undef doE2
      #undef doE1
   }

   //
   // ParseExpMulti
   //
   GDCC::IR::Exp::Ref ParseExpMulti(GDCC::TokenStream &in)
   {
      std::vector<GDCC::IR::Exp::Ref> val;
      auto pos = in.peek().pos;

      SkipToken(in, GDCC::TOK_BraceO, "{");

      if(in.peek().tok != GDCC::TOK_BrackC) do
         val.emplace_back(ParseExp(in));
      while(in.drop(GDCC::TOK_Comma));

      SkipToken(in, GDCC::TOK_BraceC, "}");

      return GDCC::IR::ExpCreate_ValueMulti(val.data(), val.size(), pos);
   }

   //
   // ParseFastI
   //
   GDCC::FastI ParseFastI(GDCC::TokenStream &in)
   {
      auto i = ParseInteg(in);

      return number_cast<GDCC::FastI>(i);
   }

   //
   // ParseFastU
   //
   GDCC::FastU ParseFastU(GDCC::TokenStream &in)
   {
      auto i = ParseInteg(in);

      return number_cast<GDCC::FastU>(i);
   }

   //
   // ParseInteg
   //
   GDCC::Integ ParseInteg(GDCC::TokenStream &in)
   {
      auto exp = ParseExp(in);
      auto val = exp->getValue();

      switch(val.v)
      {
      case GDCC::IR::ValueBase::Fixed:
         return val.vFixed.value >> val.vFixed.vtype.bitsF;

      default:
         std::cerr << "stub\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // ParseMulti
   //
   GDCC::IR::Value_Multi ParseMulti(GDCC::TokenStream &in)
   {
      std::vector<GDCC::IR::Value> val;

      SkipToken(in, GDCC::TOK_BrackO, "[");

      if(in.peek().tok != GDCC::TOK_BrackC) do
         val.emplace_back(ParseExp(in)->getValue());
      while(in.drop(GDCC::TOK_Comma));

      SkipToken(in, GDCC::TOK_BrackC, "]");

      GDCC::Array<GDCC::IR::Value> vals{GDCC::Move, val.begin(), val.end()};
      GDCC::Array<GDCC::IR::Type> types{vals.size()};

      for(std::size_t i = 0, e = vals.size(); i != e; ++i)
         types[i] = vals[i].getType();

      return GDCC::IR::Value_Multi(std::move(vals),
         GDCC::IR::Type_Multi(std::move(types)));
   }

   //
   // ParseNumber
   //
   GDCC::IR::Value ParseNumber(GDCC::Token const &tok)
   {
      // Read prefix.
      char const *s;
      int         base;
      std::tie(s, base) = GDCC::ParseNumberBaseC(tok.str.getData().str);

      // Read integral part.
      GDCC::Integ valI;
      std::tie(s, valI, std::ignore) = GDCC::ParseNumberInteg(s, base);

      // Read fractional part.
      bool        hasF;
      std::size_t digF;
      GDCC::Integ valF;

      if((hasF = *s == '.'))
         std::tie(s, valF, digF) = GDCC::ParseNumberInteg(++s, base);
      else
         valF = 0, digF = 0;

      // Read exponent.
      GDCC::FastI valE;

      if(*s == 'E' || *s == 'e' || *s == 'P' || *s == 'p')
         std::tie(s, valE) = GDCC::ParseNumberExpC(s);
      else
         valE = 0;

      // Read suffix.
      GDCC::FastU bitsF;
      GDCC::FastU bitsI;
      bool        bitsS;
      bool        fixed;
      bool        satur;

      if(*s == '_')
      {
         std::tie(s, fixed, bitsS, bitsI, bitsF, satur) = ParseSuffix(++s, tok);
      }
      else
      {
         // TODO: Target-based determination of default type.
         if(hasF)
         {
            bitsF = 15;
            bitsI = 16;
            bitsS = true;
            fixed = true;
            satur = false;
         }
         else
         {
            bitsF = 0;
            bitsI = 32;
            bitsS = false;
            fixed = true;
            satur = false;
         }
      }

      // Check that end of string was reached.
      if(*s)
      {
         std::cerr << "ERROR: " << tok.pos << ": bad number: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }

      if(fixed)
      {
         auto val = GDCC::MergeNumberFixedC(valI <<= bitsF, valF <<= bitsF, valE, digF, base);

         return GDCC::IR::Value_Fixed(val, GDCC::IR::Type_Fixed(bitsI, bitsF, bitsS, satur));
      }
      else
      {
         std::cerr << "STUB:" __FILE__ << ':' << __LINE__ << '\n';
         throw EXIT_FAILURE;
      }
   }
}

// EOF

