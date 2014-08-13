//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Value parsing.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Value.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // GetValue
      //
      IR::Value GetValue(ParserCtx const &ctx)
      {
         if(!ctx.in.drop(Core::TOK_Identi, Core::STR_Value))
            throw Core::ParseExceptExpect(ctx.in.peek(), "Value", true);

         switch(TokenPeekIdenti(ctx).in.peek().str)
         {
         case Core::STR_Empty: return GetValue_Empty(ctx);
       //case Core::STR_Fixed: return GetValue_Fixed(ctx);
       //case Core::STR_Float: return GetValue_Float(ctx);
         case Core::STR_Funct: return GetValue_Funct(ctx);
         case Core::STR_Multi: return GetValue_Multi(ctx);
         case Core::STR_Point: return GetValue_Point(ctx);
         case Core::STR_StrEn: return GetValue_StrEn(ctx);

         default:
            throw Core::ParseExceptExpect(ctx.in.peek(), "Value", false);
         }
      }

      //
      // GetValue_Empty
      //
      IR::Value_Empty GetValue_Empty(ParserCtx const &ctx)
      {
         auto vtype = GetType_Empty(ctx);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {std::move(vtype)};
      }

      //
      // GetValue_Funct
      //
      IR::Value_Funct GetValue_Funct(ParserCtx const &ctx)
      {
         auto vtype = GetType_Funct(ctx);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto value = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {value, std::move(vtype)};
      }

      //
      // GetValue_Multi
      //
      IR::Value_Multi GetValue_Multi(ParserCtx const &ctx)
      {
         auto vtype = GetType_Multi(ctx);

         std::vector<IR::Value> value;
         value.reserve(vtype.types.size());

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         if(!ctx.in.peek(Core::TOK_ParenC)) do
            value.emplace_back(GetExp(ctx)->getValue());
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {{Core::Move, value.begin(), value.end()}, std::move(vtype)};
      }

      //
      // GetValue_Point
      //
      IR::Value_Point GetValue_Point(ParserCtx const &ctx)
      {
         auto vtype = GetType_Point(ctx);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto value = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
         auto addrB = GetAddrBase(ctx);
         auto addrN = GetString(ctx);
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {value, addrB, addrN, std::move(vtype)};
      }

      //
      // GetValue_StrEn
      //
      IR::Value_StrEn GetValue_StrEn(ParserCtx const &ctx)
      {
         auto vtype = GetType_StrEn(ctx);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto value = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {value, std::move(vtype)};
      }
   }
}

// EOF

