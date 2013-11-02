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
      class Type;
      class Value;
      class Value_Multi;
   }

   class TokenStream;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   GDCC::TokenStream &ExpectToken(GDCC::TokenStream &in, GDCC::TokenType tt, char const *str);

   GDCC::IR::AddrBase ParseAddrBase(GDCC::Token const &tok);

   GDCC::IR::Arg ParseArg(GDCC::TokenStream &in);

   void ParseBlock(GDCC::TokenStream &in, GDCC::IR::Block &block, GDCC::TokenType end);

   GDCC::IR::CallType ParseCallType(GDCC::Token const &tok);

   GDCC::CounterRef<GDCC::IR::Exp> ParseExp(GDCC::TokenStream &in);

   GDCC::CounterRef<GDCC::IR::Exp> ParseExpMulti(GDCC::TokenStream &in);

   GDCC::FastI ParseFastI(GDCC::TokenStream &in);
   GDCC::FastU ParseFastU(GDCC::TokenStream &in);

   void ParseFunction(GDCC::TokenStream &in, GDCC::IR::Function &func);

   void ParseImport(GDCC::TokenStream &in, GDCC::IR::Import &imp);

   GDCC::Integ ParseInteg(GDCC::TokenStream &in);

   GDCC::IR::Linkage ParseLinkage(GDCC::Token const &tok);

   void ParseObject(GDCC::TokenStream &in, GDCC::IR::Object &obj);

   GDCC::IR::Value_Multi ParseMulti(GDCC::TokenStream &in);

   GDCC::IR::Value ParseNumber(GDCC::Token const &tok);

   GDCC::IR::ScriptType ParseScriptType(GDCC::Token const &tok);

   void ParseSpace(GDCC::TokenStream &in, GDCC::IR::Space &space);

   void ParseStrEnt(GDCC::TokenStream &in, GDCC::IR::StrEnt &str);

   GDCC::IR::Type ParseType(GDCC::TokenStream &in);

   GDCC::TokenStream &SkipToken(GDCC::TokenStream &in, GDCC::TokenType tt, char const *str);
}

#endif//Asm__Parse_H__

