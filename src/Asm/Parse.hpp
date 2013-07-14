//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly parsing utilities.
//
//-----------------------------------------------------------------------------

#ifndef Asm__Parse_H__
#define Asm__Parse_H__

#include "GDCC/Counter.hpp"
#include "GDCC/Number.hpp"
#include "GDCC/Token.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Asm
{
   class IStream;
}

namespace GDCC
{
   namespace IR
   {
      class Arg;
      class Block;
      class Exp;
      class Function;
      class Value;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   GDCC::IR::Arg ParseArg(IStream &in);

   void ParseBlock(IStream &in, GDCC::IR::Block &block, GDCC::TokenType end);

   GDCC::CounterRef<GDCC::IR::Exp> ParseExp(IStream &in);

   GDCC::FastU ParseFastU(IStream &in);

   void ParseFunction(IStream &in, GDCC::IR::Function &func);

   GDCC::Integ ParseInteg(IStream &in);

   GDCC::IR::Value ParseNumber(GDCC::Token tok);
}

#endif//Asm__Parse_H__

