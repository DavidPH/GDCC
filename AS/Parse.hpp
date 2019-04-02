//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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

#include "../AS/Types.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"
#include "../Core/Token.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::AS
{
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

      ParserCtx(ParserCtx const &ctx, Core::TokenStream &in_) :
         in    (in_),
         macros(ctx.macros),
         prog  (ctx.prog)
      {
      }

      Core::TokenStream &in;
      MacroMap          &macros;
      IR::Program       &prog;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   IR::AddrBase GetAddrBase(ParserCtx const &ctx);

   IR::AddrSpace GetAddrSpace(ParserCtx const &ctx);

   IR::Arg GetArg(ParserCtx const &ctx, Core::FastU w, Core::FastU n);

   IR::Value_Array GetArrayString(ParserCtx const &ctx);

   IR::CallType GetCallType(ParserCtx const &ctx);

   IR::Code GetCode(ParserCtx const &ctx);
   IR::CodeBase GetCodeBase(ParserCtx const &ctx);
   IR::CodeType GetCodeType(ParserCtx const &ctx);

   Core::CounterRef<IR::Exp const> GetExp(ParserCtx const &ctx);

   Core::FastI GetFastI(ParserCtx const &ctx);
   Core::FastU GetFastU(ParserCtx const &ctx);

   Core::Integ GetInteg(ParserCtx const &ctx);

   IR::Linkage GetLinkage(ParserCtx const &ctx);

   IR::Value GetNumber(Core::Token const &tok);

   Core::String GetString(ParserCtx const &ctx);

   IR::Value_Tuple GetTuple(ParserCtx const &ctx);

   IR::Type GetType(ParserCtx const &ctx);

   IR::Value GetValue(ParserCtx const &ctx);

   void ParseBlock(ParserCtx const &ctx, IR::Block &block, Core::TokenType end);

   void ParseDJump(ParserCtx const &ctx, IR::DJump &jump);

   void ParseDeclaration(ParserCtx const &ctx);

   void ParseFile(char const *inName, IR::Program &prog);

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

   #define GDCC_IR_TypeList(t) \
      IR::Type_##t GetType_##t(ParserCtx const &ctx); \
      IR::Value_##t GetValue_##t(ParserCtx const &ctx);
   #include "../IR/TypeList.hpp"
}

#endif//GDCC__AS__Parse_H__

