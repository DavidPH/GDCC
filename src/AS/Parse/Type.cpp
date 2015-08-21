//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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

namespace GDCC
{
   namespace AS
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
            throw Core::ParseExceptExpect(ctx.in.reget(), "Type", false);
         }
      }

      //
      // GetType_Array
      //
      IR::Type_Array GetType_Array(ParserCtx const &ctx)
      {
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Array))
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Array", false);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto elemT = GetType(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Assoc", false);

         std::vector<IR::TypeAssoc> assoc;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         if(!ctx.in.peek(Core::TOK_ParenC)) do
         {
            auto type = GetType(ctx);
            auto name = GetString(ctx);
            auto addr = GetFastU(ctx);

            assoc.emplace_back(std::move(type), name, addr);
         }
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {{Core::Move, assoc.begin(), assoc.end()}};
      }

      //
      // GetType_DJump
      //
      IR::Type_DJump GetType_DJump(ParserCtx const &ctx)
      {
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR_DJump))
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_DJump", false);

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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Empty", false);

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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Fixed", false);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto bitsI = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
         auto bitsF = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
         bool bitsS = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Float", false);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto bitsI = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
         auto bitsF = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
         bool bitsS = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Funct", false);

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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Point", false);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto addrB = GetAddrBase(ctx);
         auto addrN = GetString(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
         auto addrS = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_StrEn", false);

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
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Tuple", false);

         std::vector<IR::Type> typev;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         if(!ctx.in.peek(Core::TOK_ParenC)) do
            typev.emplace_back(GetType(ctx));
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {{Core::Move, typev.begin(), typev.end()}};
      }

      //
      // GetType_Union
      //
      IR::Type_Union GetType_Union(ParserCtx const &ctx)
      {
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Union))
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Union", false);

         std::vector<IR::Type> typev;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         if(!ctx.in.peek(Core::TOK_ParenC)) do
            typev.emplace_back(GetType(ctx));
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {{Core::Move, typev.begin(), typev.end()}};
      }
   }
}

// EOF

