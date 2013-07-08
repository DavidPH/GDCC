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

#include <iostream>


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
      default:
         std::cerr << "ERROR: " << tok.pos << ": expected expression\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // ParseFastU
   //
   GDCC::IR::FastU ParseFastU(IStream &in)
   {
      auto i = ParseInteg(in);

      return static_cast<GDCC::IR::FastU>(i);
   }

   //
   // ParseInteg
   //
   GDCC::IR::Integ ParseInteg(IStream &in)
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
}

// EOF

