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
            #define GDCC_IR_TypeList(name) \
               case Core::STR_##name: return GetValue_##name(ctx);
            #include "IR/TypeList.hpp"

         default:
            throw Core::ParseExceptExpect(ctx.in.peek(), "Value", false);
         }
      }

      //
      // GetValue_Array
      //
      IR::Value_Array GetValue_Array(ParserCtx const &ctx)
      {
         auto vtype = GetType_Array(ctx);

         std::vector<IR::Value> value;
         value.reserve(vtype.elemC);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         if(!ctx.in.peek(Core::TOK_ParenC)) do
            value.emplace_back(GetExp(ctx)->getValue());
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {{Core::Move, value.begin(), value.end()}, std::move(vtype)};
      }

      //
      // GetValue_Assoc
      //
      IR::Value_Assoc GetValue_Assoc(ParserCtx const &ctx)
      {
         auto vtype = GetType_Assoc(ctx);

         std::vector<IR::Value> value;
         value.reserve(vtype.assoc.size());

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         if(!ctx.in.peek(Core::TOK_ParenC)) do
            value.emplace_back(GetExp(ctx)->getValue());
         while(ctx.in.drop(Core::TOK_Comma));

         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {{Core::Move, value.begin(), value.end()}, std::move(vtype)};
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
      // GetValue_Fixed
      //
      IR::Value_Fixed GetValue_Fixed(ParserCtx const &ctx)
      {
         throw Core::ExceptStr(ctx.in.reget().pos, "Value_Fixed stub");
      }

      //
      // GetValue_Float
      //
      IR::Value_Float GetValue_Float(ParserCtx const &ctx)
      {
         throw Core::ExceptStr(ctx.in.reget().pos, "Value_Float stub");
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

      //
      // GetValue_Tuple
      //
      IR::Value_Tuple GetValue_Tuple(ParserCtx const &ctx)
      {
         auto vtype = GetType_Tuple(ctx);

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
      // GetValue_Union
      //
      IR::Value_Union GetValue_Union(ParserCtx const &ctx)
      {
         auto vtype = GetType_Union(ctx);

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto value = GetExp(ctx)->getValue();
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return {std::move(value), std::move(vtype)};
      }
   }
}

// EOF

