//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
      enum class Code;
      enum class Linkage;
      enum class ScriptType;

      class AddrSpace;
      class Arg;
      class Block;
      class Exp;
      class Function;
      class GlyphData;
      class Import;
      class Object;
      class Program;
      class Space;
      class StrEnt;
      class Type;
      struct Type_Empty;
      struct Type_Fixed;
      struct Type_Float;
      struct Type_Funct;
      struct Type_Multi;
      struct Type_Point;
      struct Type_StrEn;
      class Value;
      struct Value_Empty;
      struct Value_Fixed;
      struct Value_Float;
      struct Value_Funct;
      struct Value_Multi;
      struct Value_Point;
      struct Value_StrEn;
   }

   namespace AS
   {
      class MacroMap;

      //
      // ParserCtx
      //
      class ParserCtx
      {
      public:
         ParserCtx(Core::TokenStream &in_, MacroMap &macros_, IR::Program &prog_) :
            in    (in_),
            macros(macros_),
            prog  (prog_)
         {
         }

         Core::TokenStream &in;
         MacroMap          &macros;
         IR::Program       &prog;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      IR::AddrBase GetAddrBase(ParserCtx const &ctx);

      IR::AddrSpace GetAddrSpace(ParserCtx const &ctx);

      IR::Arg GetArg(ParserCtx const &ctx);

      IR::CallType GetCallType(ParserCtx const &ctx);

      IR::Code GetCode(ParserCtx const &ctx);

      Core::CounterRef<IR::Exp const> GetExp(ParserCtx const &ctx);

      Core::FastI GetFastI(ParserCtx const &ctx);
      Core::FastU GetFastU(ParserCtx const &ctx);

      Core::Integ GetInteg(ParserCtx const &ctx);

      IR::Linkage GetLinkage(ParserCtx const &ctx);

      IR::Value_Multi GetMulti(ParserCtx const &ctx);

      IR::Value_Multi GetMultiString(ParserCtx const &ctx);

      IR::Value GetNumber(Core::Token const &tok);

      IR::ScriptType GetScriptType(ParserCtx const &ctx);

      Core::String GetString(ParserCtx const &ctx);

      IR::Type GetType(ParserCtx const &ctx);

      IR::Type_Empty GetType_Empty(ParserCtx const &ctx);
      IR::Type_Fixed GetType_Fixed(ParserCtx const &ctx);
      IR::Type_Float GetType_Float(ParserCtx const &ctx);
      IR::Type_Funct GetType_Funct(ParserCtx const &ctx);
      IR::Type_Multi GetType_Multi(ParserCtx const &ctx);
      IR::Type_Point GetType_Point(ParserCtx const &ctx);
      IR::Type_StrEn GetType_StrEn(ParserCtx const &ctx);

      IR::Value GetValue(ParserCtx const &ctx);

      IR::Value_Empty GetValue_Empty(ParserCtx const &ctx);
      IR::Value_Fixed GetValue_Fixed(ParserCtx const &ctx);
      IR::Value_Float GetValue_Float(ParserCtx const &ctx);
      IR::Value_Funct GetValue_Funct(ParserCtx const &ctx);
      IR::Value_Multi GetValue_Multi(ParserCtx const &ctx);
      IR::Value_Point GetValue_Point(ParserCtx const &ctx);
      IR::Value_StrEn GetValue_StrEn(ParserCtx const &ctx);

      void ParseBlock(ParserCtx const &ctx, IR::Block &block, Core::TokenType end);

      void ParseDeclaration(ParserCtx const &ctx);

      void ParseFunction(ParserCtx const &ctx, IR::Function &func);

      void ParseGlyph(ParserCtx const &ctx, IR::GlyphData &glyph);

      void ParseImport(ParserCtx const &ctx, IR::Import &imp);

      void ParseObject(ParserCtx const &ctx, IR::Object &obj);

      void ParseSpace(ParserCtx const &ctx, IR::Space &space);

      void ParseStrEnt(ParserCtx const &ctx, IR::StrEnt &str);

      ParserCtx const &TokenDrop(ParserCtx const &ctx, Core::TokenType tt,
         char const *expect);

      ParserCtx const &TokenDropEq(ParserCtx const &ctx);

      ParserCtx const &TokenPeek(ParserCtx const &ctx, Core::TokenType tt,
         char const *expect);

      ParserCtx const &TokenPeekIdenti(ParserCtx const &ctx);
      ParserCtx const &TokenPeekString(ParserCtx const &ctx);
   }
}

#endif//GDCC__AS__Parse_H__

