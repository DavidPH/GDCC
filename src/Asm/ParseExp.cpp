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

#include "IStream.hpp"

#include "GDCC/Parse.hpp"

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
ParseSuffix(char const *s, GDCC::Token tok)
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
   GDCC::IR::Exp::Ref ParseExp(IStream &in)
   {
      GDCC::Token tok;
      switch((in >> tok, tok).tok)
      {
      case GDCC::TOK_Number:
         return GDCC::IR::ExpCreate_ValueRoot(ParseNumber(tok), tok.pos);

      case GDCC::TOK_String:
         return GDCC::IR::ExpCreate_ValueGlyph(static_cast<GDCC::IR::Glyph>(tok.str), tok.pos);

      default:
         std::cerr << "ERROR: " << tok.pos << ": expected expression\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // ParseFastI
   //
   GDCC::FastI ParseFastI(IStream &in)
   {
      auto i = ParseInteg(in);

      return number_cast<GDCC::FastI>(i);
   }

   //
   // ParseFastU
   //
   GDCC::FastU ParseFastU(IStream &in)
   {
      auto i = ParseInteg(in);

      return number_cast<GDCC::FastU>(i);
   }

   //
   // ParseInteg
   //
   GDCC::Integ ParseInteg(IStream &in)
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
   // ParseNumber
   //
   GDCC::IR::Value ParseNumber(GDCC::Token tok)
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

