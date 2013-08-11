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
      enum class AddrBase;
      enum class CallType;
      enum class Linkage;
      enum class ScriptType;

      class Arg;
      class Block;
      class Exp;
      class Function;
      class Import;
      class Object;
      class Space;
      class StrEnt;
      class Value;
      class Value_Multi;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   GDCC::IR::AddrBase ParseAddrBase(GDCC::Token const &tok);

   GDCC::IR::Arg ParseArg(IStream &in);

   void ParseBlock(IStream &in, GDCC::IR::Block &block, GDCC::TokenType end);

   GDCC::IR::CallType ParseCallType(GDCC::Token const &tok);

   GDCC::CounterRef<GDCC::IR::Exp> ParseExp(IStream &in);

   GDCC::FastI ParseFastI(IStream &in);
   GDCC::FastU ParseFastU(IStream &in);

   void ParseFunction(IStream &in, GDCC::IR::Function &func);

   void ParseImport(IStream &in, GDCC::IR::Import &imp);

   GDCC::Integ ParseInteg(IStream &in);

   GDCC::IR::Linkage ParseLinkage(GDCC::Token const &tok);

   void ParseObject(IStream &in, GDCC::IR::Object &obj);

   GDCC::IR::Value_Multi ParseMulti(IStream &in);

   GDCC::IR::Value ParseNumber(GDCC::Token const &tok);

   GDCC::IR::ScriptType ParseScriptType(GDCC::Token const &tok);

   void ParseSpace(IStream &in, GDCC::IR::Space &space);

   void ParseStrEnt(IStream &in, GDCC::IR::StrEnt &str);

   IStream &SkipEqual(IStream &in);
}

#endif//Asm__Parse_H__

