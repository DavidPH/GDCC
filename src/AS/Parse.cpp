//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2017 David Hill
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

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"
#include "IR/Code.hpp"


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
         #define GDCC_IR_AddrList(name) \
            case Core::STR_##name: return IR::AddrBase::name;
         #include "IR/AddrList.hpp"

      default:
         throw Core::ParseExceptExpect(ctx.in.reget(), "AddrBase", false);
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
         #define GDCC_IR_CallTypeList(name) \
            case Core::STR_##name: return IR::CallType::name;
         #include "IR/CallTypeList.hpp"

      default:
         throw Core::ParseExceptExpect(ctx.in.reget(), "CallType", false);
      }
   }

   //
   // GetCode
   //
   IR::Code GetCode(ParserCtx const &ctx)
   {
      switch(TokenPeekIdenti(ctx).in.get().str)
      {
         #define GDCC_IR_CodeList(c) \
            case Core::STR_##c: return IR::Code::c; break;
         #include "IR/CodeList.hpp"

      default:
         throw Core::ParseExceptExpect(ctx.in.reget(), "Code", false);
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
         throw Core::ParseExceptExpect(ctx.in.reget(), "Linkage", false);
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
         throw Core::ParseExceptExpect(ctx.in.peek(), expect, false);

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
         throw Core::ParseExceptExpect(ctx.in.peek(), expect, false);

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

