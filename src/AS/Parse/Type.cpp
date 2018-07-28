//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Type parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Type.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // GetType
   //
   IR::Type GetType(ParserCtx const &ctx)
   {
      switch(TokenPeekIdenti(ctx).in.peek().str)
      {
         #define GDCC_IR_TypeList(name) \
            case Core::STR_##name: return GetType_##name(ctx);
         #include "IR/TypeList.hpp"

      default:
         Core::ErrorExpect("Type", ctx.in.reget());
      }
   }

   //
   // GetType_Array
   //
   IR::Type_Array GetType_Array(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Array))
         Core::ErrorExpect("Type_Array", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto elemT = GetType(ctx);
      auto elemC = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {std::move(elemT), elemC};
   }

   //
   // GetType_Assoc
   //
   IR::Type_Assoc GetType_Assoc(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Assoc))
         Core::ErrorExpect("Type_Assoc", ctx.in.peek());

      std::vector<IR::TypeAssoc> assoc;

      TokenDrop(ctx, Core::TOK_ParenO, "'('");

      while(!ctx.in.drop(Core::TOK_ParenC))
      {
         auto type = GetType(ctx);
         auto name = GetString(ctx);
         auto addr = GetFastU(ctx);

         assoc.emplace_back(std::move(type), name, addr);
      }

      return {{Core::Move, assoc.begin(), assoc.end()}};
   }

   //
   // GetType_DJump
   //
   IR::Type_DJump GetType_DJump(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_DJump))
         Core::ErrorExpect("Type_DJump", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {};
   }

   //
   // GetType_Empty
   //
   IR::Type_Empty GetType_Empty(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Empty))
         Core::ErrorExpect("Type_Empty", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {};
   }

   //
   // GetType_Fixed
   //
   IR::Type_Fixed GetType_Fixed(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Fixed))
         Core::ErrorExpect("Type_Fixed", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto bitsI = GetFastU(ctx);
      auto bitsF = GetFastU(ctx);
      bool bitsS = GetFastU(ctx);
      bool satur = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {bitsI, bitsF, bitsS, satur};
   }

   //
   // GetType_Float
   //
   IR::Type_Float GetType_Float(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Float))
         Core::ErrorExpect("Type_Float", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto bitsI = GetFastU(ctx);
      auto bitsF = GetFastU(ctx);
      bool bitsS = GetFastU(ctx);
      bool satur = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {bitsI, bitsF, bitsS, satur};
   }

   //
   // GetType_Funct
   //
   IR::Type_Funct GetType_Funct(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Funct))
         Core::ErrorExpect("Type_Funct", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto callT = GetCallType(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {callT};
   }

   //
   // GetType_Point
   //
   IR::Type_Point GetType_Point(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Point))
         Core::ErrorExpect("Type_Point", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto addrB = GetAddrBase(ctx);
      auto addrN = GetString(ctx);
      auto addrS = GetFastU(ctx);
      auto addrW = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {addrB, addrN, addrS, addrW};
   }

   //
   // GetType_StrEn
   //
   IR::Type_StrEn GetType_StrEn(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_StrEn))
         Core::ErrorExpect("Type_StrEn", ctx.in.peek());

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return {};
   }

   //
   // GetType_Tuple
   //
   IR::Type_Tuple GetType_Tuple(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Tuple))
         Core::ErrorExpect("Type_Tuple", ctx.in.peek());

      std::vector<IR::Type> typev;

      TokenDrop(ctx, Core::TOK_ParenO, "'('");

      while(!ctx.in.drop(Core::TOK_ParenC))
         typev.emplace_back(GetType(ctx));

      return {{Core::Move, typev.begin(), typev.end()}};
   }

   //
   // GetType_Union
   //
   IR::Type_Union GetType_Union(ParserCtx const &ctx)
   {
      if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Union))
         Core::ErrorExpect("Type_Union", ctx.in.peek());

      std::vector<IR::Type> typev;

      TokenDrop(ctx, Core::TOK_ParenO, "'('");

      while(!ctx.in.drop(Core::TOK_ParenC))
         typev.emplace_back(GetType(ctx));

      return {{Core::Move, typev.begin(), typev.end()}};
   }
}

// EOF

