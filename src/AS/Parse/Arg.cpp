//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Arg parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Arg.hpp"


//----------------------------------------------------------------------------|
// Static Function                                                            |
//

//
// GetArg0
//
template<typename T>
static T GetArg0(GDCC::AS::ParserCtx const &ctx)
{
   using namespace GDCC;

   AS::TokenDrop(ctx, Core::TOK_ParenO, "'('");
   AS::TokenDrop(ctx, Core::TOK_ParenC, "')'");

   return T();
}

//
// GetArgU
//
template<typename T>
static T GetArgU(GDCC::AS::ParserCtx const &ctx)
{
   using namespace GDCC;

   AS::TokenDrop(ctx, Core::TOK_ParenO, "'('");
   auto exp = AS::GetFastU(ctx);
   AS::TokenDrop(ctx, Core::TOK_ParenC, "')'");

   return T(std::move(exp));
}

//
// GetArg1
//
template<typename T>
static T GetArg1(GDCC::AS::ParserCtx const &ctx)
{
   using namespace GDCC;

   AS::TokenDrop(ctx, Core::TOK_ParenO, "'('");
   auto exp = AS::GetExp(ctx);
   AS::TokenDrop(ctx, Core::TOK_ParenC, "')'");

   return T(std::move(exp));
}

//
// GetArg2
//
template<typename T>
static T GetArg2(GDCC::AS::ParserCtx const &ctx)
{
   using namespace GDCC;

   AS::TokenDrop(ctx, Core::TOK_ParenO, "'('");
   auto arg = AS::GetArg(ctx);
   AS::TokenDrop(ctx, Core::TOK_Comma, "','");
   auto exp = AS::GetExp(ctx);
   AS::TokenDrop(ctx, Core::TOK_ParenC, "')'");

   return T(std::move(arg), std::move(exp));
}

//
// GetArg3
//
template<typename T>
static T GetArg3(GDCC::AS::ParserCtx const &ctx)
{
   using namespace GDCC;

   AS::TokenDrop(ctx, Core::TOK_ParenO, "'('");
   auto arg0 = AS::GetArg(ctx);
   AS::TokenDrop(ctx, Core::TOK_Comma, "','");
   auto arg1 = AS::GetArg(ctx);
   AS::TokenDrop(ctx, Core::TOK_Comma, "','");
   auto exp  = AS::GetExp(ctx);
   AS::TokenDrop(ctx, Core::TOK_ParenC, "')'");

   return T(std::move(arg0), std::move(arg1), std::move(exp));
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // GetArg
      //
      IR::Arg GetArg(ParserCtx const &ctx)
      {
         switch(TokenPeekIdenti(ctx).in.get().str)
         {
         case Core::STR_Gen:    return GetArg2<IR::Arg_Gen   >(ctx);

         case Core::STR_Cpy:    return GetArgU<IR::Arg_Cpy   >(ctx);
         case Core::STR_Lit:    return GetArg1<IR::Arg_Lit   >(ctx);
         case Core::STR_Nul:    return GetArg0<IR::Arg_Nul   >(ctx);
         case Core::STR_Stk:    return GetArg0<IR::Arg_Stk   >(ctx);

         case Core::STR_Far:    return GetArg2<IR::Arg_Far   >(ctx);
         case Core::STR_GblArs: return GetArg2<IR::Arg_GblArs>(ctx);
         case Core::STR_GblReg: return GetArg2<IR::Arg_GblReg>(ctx);
         case Core::STR_Loc:    return GetArg2<IR::Arg_Loc   >(ctx);
         case Core::STR_LocArs: return GetArg2<IR::Arg_LocArs>(ctx);
         case Core::STR_LocReg: return GetArg2<IR::Arg_LocReg>(ctx);
         case Core::STR_MapArs: return GetArg2<IR::Arg_MapArs>(ctx);
         case Core::STR_MapReg: return GetArg2<IR::Arg_MapReg>(ctx);
         case Core::STR_StrArs: return GetArg2<IR::Arg_StrArs>(ctx);
         case Core::STR_Vaa:    return GetArg2<IR::Arg_Vaa   >(ctx);
         case Core::STR_WldArs: return GetArg2<IR::Arg_WldArs>(ctx);
         case Core::STR_WldReg: return GetArg2<IR::Arg_WldReg>(ctx);

         case Core::STR_GblArr: return GetArg3<IR::Arg_GblArr>(ctx);
         case Core::STR_MapArr: return GetArg3<IR::Arg_MapArr>(ctx);
         case Core::STR_StrArr: return GetArg3<IR::Arg_StrArr>(ctx);
         case Core::STR_WldArr: return GetArg3<IR::Arg_WldArr>(ctx);

         default:
            ctx.in.unget();
            throw Core::ParseExceptExpect(ctx.in.peek(), "Arg", false);
         }
      }
   }
}

// EOF

