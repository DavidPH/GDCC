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

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"
#include "IR/Code.hpp"
#include "IR/Linkage.hpp"
#include "IR/ScriptType.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
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
      // getCode
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
         case Core::STR_ExtACS: return IR::Linkage::ExtACS;
         case Core::STR_ExtASM: return IR::Linkage::ExtASM;
         case Core::STR_ExtC:   return IR::Linkage::ExtC;
         case Core::STR_ExtCXX: return IR::Linkage::ExtCXX;
         case Core::STR_ExtDS:  return IR::Linkage::ExtDS;
         case Core::STR_IntC:   return IR::Linkage::IntC;
         case Core::STR_IntCXX: return IR::Linkage::IntCXX;

         default:
            throw Core::ParseExceptExpect(ctx.in.reget(), "Linkage", false);
         }
      }

      //
      // GetScriptType
      //
      IR::ScriptType GetScriptType(ParserCtx const &ctx)
      {
         switch(TokenPeekIdenti(ctx).in.get().str)
         {
         case Core::STR_None:       return IR::ScriptType::None;
         case Core::STR_Death:      return IR::ScriptType::Death;
         case Core::STR_Disconnect: return IR::ScriptType::Disconnect;
         case Core::STR_Enter:      return IR::ScriptType::Enter;
         case Core::STR_Lightning:  return IR::ScriptType::Lightning;
         case Core::STR_Open:       return IR::ScriptType::Open;
         case Core::STR_Respawn:    return IR::ScriptType::Respawn;
         case Core::STR_Return:     return IR::ScriptType::Return;
         case Core::STR_Unloading:  return IR::ScriptType::Unloading;

         default:
            throw Core::ParseExceptExpect(ctx.in.reget(), "ScriptType", false);
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
}

// EOF

