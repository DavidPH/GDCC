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

#ifndef GDCC__AS__Parse_H__
#define GDCC__AS__Parse_H__

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"
#include "../Core/Token.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class TokenStream;
   }

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
      class Program;
      class Space;
      class StrEnt;
      class Type;
      class Value;
      struct Value_Multi;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      Core::TokenStream &ExpectToken(Core::TokenStream &in, Core::TokenType tt,
         char const *str);

      IR::AddrBase ParseAddrBase(Core::Token const &tok);

      IR::Arg ParseArg(Core::TokenStream &in, IR::Program &prog);

      void ParseBlock(Core::TokenStream &in, IR::Program &prog,
         IR::Block &block, Core::TokenType end);

      IR::CallType ParseCallType(Core::Token const &tok);

      Core::CounterRef<IR::Exp const> ParseExp(Core::TokenStream &in,
         IR::Program &prog);

      Core::CounterRef<IR::Exp const> ParseExpMulti(Core::TokenStream &in,
         IR::Program &prog);

      Core::FastI ParseFastI(Core::TokenStream &in, IR::Program &prog);
      Core::FastU ParseFastU(Core::TokenStream &in, IR::Program &prog);

      void ParseFunction(Core::TokenStream &in, IR::Program &prog,
         IR::Function &func);

      void ParseImport(Core::TokenStream &in, IR::Program &prog,
         IR::Import &imp);

      Core::Integ ParseInteg(Core::TokenStream &in, IR::Program &prog);

      IR::Linkage ParseLinkage(Core::Token const &tok);

      void ParseObject(Core::TokenStream &in, IR::Program &prog,
         IR::Object &obj);

      IR::Value_Multi ParseMulti(Core::TokenStream &in, IR::Program &prog);

      IR::Value_Multi ParseMultiString(Core::TokenStream &in,
         IR::Program &prog);

      IR::Value ParseNumber(Core::Token const &tok);

      IR::ScriptType ParseScriptType(Core::Token const &tok);

      void ParseSpace(Core::TokenStream &in, IR::Program &prog,
         IR::Space &space);

      void ParseStrEnt(Core::TokenStream &in, IR::Program &prog,
         IR::StrEnt &str);

      IR::Type ParseType(Core::TokenStream &in, IR::Program &prog);

      Core::TokenStream &SkipToken(Core::TokenStream &in, Core::TokenType tt,
         char const *str);
   }
}

#endif//GDCC__AS__Parse_H__

