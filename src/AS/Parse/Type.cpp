//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
// Global Functions                                                           |
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
      // GetType_Empty
      //
      IR::Type_Empty GetType_Empty(ParserCtx const &ctx)
      {
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Empty))
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Empty", false);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return IR::Type_Empty();
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

         return IR::Type_Fixed(bitsI, bitsF, bitsS, satur);
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

         return IR::Type_Float(bitsI, bitsF, bitsS, satur);
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

         return IR::Type_Funct(callT);
      }

      //
      // GetType_Multi
      //
      IR::Type_Multi GetType_Multi(ParserCtx const &ctx)
      {
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Multi))
            throw Core::ParseExceptExpect(ctx.in.peek(), "Type_Multi", false);

         std::vector<IR::Type> typev;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         if(!ctx.in.peek(Core::TOK_ParenC)) do
            typev.emplace_back(GetType(ctx));
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return IR::Type_Multi({Core::Move, typev.begin(), typev.end()});
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

         return IR::Type_Point(addrB, addrN, addrS, addrW);
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

         return IR::Type_StrEn();
      }
   }
}

// EOF

