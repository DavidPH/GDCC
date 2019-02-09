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

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Linkage.hpp"
#include "IR/Code.hpp"

#include "Target/Addr.hpp"
#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // GetAddrBase
   //
   IR::AddrBase GetAddrBase(ParserCtx const &ctx)
   {
      switch(TokenPeekIdenti(ctx).in.get().str)
      {
         #define GDCC_Target_AddrList(name) \
            case Core::STR_##name: return IR::AddrBase::name;
         #include "Target/AddrList.hpp"

      default:
         Core::ErrorExpect("AddrBase", ctx.in.reget());
      }
   }

   //
   // GetAddrSpace
   //
   IR::AddrSpace GetAddrSpace(ParserCtx const &ctx)
   {
      IR::AddrSpace space;

      space.base = GetAddrBase(ctx);
      space.name = TokenPeekString(ctx).in.get().str;

      if(space.name == Core::STR_)
         space.name = Core::STRNULL;

      return space;
   }

   //
   // GetCallType
   //
   IR::CallType GetCallType(ParserCtx const &ctx)
   {
      switch(TokenPeekIdenti(ctx).in.get().str)
      {
         #define GDCC_Target_CallTypeList(name) \
            case Core::STR_##name: return IR::CallType::name;
         #include "Target/CallTypeList.hpp"

      default:
         Core::ErrorExpect("CallType", ctx.in.reget());
      }
   }

   //
   // GetCode
   //
   IR::Code GetCode(ParserCtx const &ctx)
   {
      auto base = GetCodeBase(ctx);
      if(ctx.in.drop(Core::TOK_Colon))
         return {base, GetCodeType(ctx)};
      else
         return base;
   }

   //
   // GetCodeBase
   //
   IR::CodeBase GetCodeBase(ParserCtx const &ctx)
   {
      switch(TokenPeekIdenti(ctx).in.get().str)
      {
         #define GDCC_IR_CodeList(c) \
            case Core::STR_##c: return IR::CodeBase::c; break;
         #include "IR/CodeList.hpp"

      default:
         Core::ErrorExpect("CodeBase", ctx.in.reget());
      }
   }

   //
   // GetCodeType
   //
   IR::CodeType GetCodeType(ParserCtx const &ctx)
   {
      auto tok = TokenPeekIdenti(ctx).in.get();
      switch(tok.str.size())
      {
      case 1: return {tok.str[0]};
      case 2: return {tok.str[0], tok.str[1]};
      case 3: return {tok.str[0], tok.str[1], tok.str[2]};
      case 4: return {tok.str[0], tok.str[1], tok.str[2], tok.str[3]};
      default:
         Core::ErrorExpect("CodeType", tok);
      }
   }

   //
   // GetLinkage
   //
   IR::Linkage GetLinkage(ParserCtx const &ctx)
   {
      switch(TokenPeekIdenti(ctx).in.get().str)
      {
         #define GDCC_IR_LinkageList(name) \
            case Core::STR_##name: return IR::Linkage::name;
         #include "IR/LinkageList.hpp"

      default:
         Core::ErrorExpect("Linkage", ctx.in.reget());
      }
   }

   //
   // GetString
   //
   Core::String GetString(ParserCtx const &ctx)
   {
      return TokenPeekString(ctx).in.get().str;
   }

   //
   // TokenDrop
   //
   ParserCtx const &TokenDrop(ParserCtx const &ctx, Core::TokenType tt,
      char const *expect)
   {
      if(!ctx.in.drop(tt))
         Core::ErrorExpect(expect, ctx.in.peek());

      return ctx;
   }

   //
   // TokenDropEq
   //
   ParserCtx const &TokenDropEq(ParserCtx const &ctx)
   {
      return TokenDrop(ctx, Core::TOK_Equal, "'='");
   }

   //
   // TokenPeek
   //
   ParserCtx const &TokenPeek(ParserCtx const &ctx, Core::TokenType tt,
      char const *expect)
   {
      if(!ctx.in.peek(tt))
         Core::ErrorExpect(expect, ctx.in.peek());

      return ctx;
   }

   //
   // TokenPeekIdenti
   //
   ParserCtx const &TokenPeekIdenti(ParserCtx const &ctx)
   {
      return TokenPeek(ctx, Core::TOK_Identi, "identifier");
   }

   //
   // TokenPeekString
   //
   ParserCtx const &TokenPeekString(ParserCtx const &ctx)
   {
      return TokenPeek(ctx, Core::TOK_String, "string");
   }
}

// EOF

