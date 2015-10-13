//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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

namespace GDCC
{
   namespace AS
   {
      //
      // GetArg0
      //
      template<typename T>
      static T GetArg0(ParserCtx const &ctx)
      {
         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return T();
      }

      //
      // GetArgU
      //
      template<typename T>
      static T GetArgU(ParserCtx const &ctx)
      {
         Core::FastU off = 0;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         if(!ctx.in.peek(Core::TOK_ParenC))
            off = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return T(std::move(off));
      }

      //
      // GetArg1
      //
      template<typename T>
      static T GetArg1(ParserCtx const &ctx)
      {
         Core::FastU off = 0;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto exp = GetExp(ctx);
         if(ctx.in.drop(Core::TOK_Comma))
            off = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return T(std::move(exp), std::move(off));
      }

      //
      // GetArg2
      //
      template<typename T>
      static T GetArg2(ParserCtx const &ctx)
      {
         Core::FastU off = 0;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto arg = GetArg(ctx);
         if(ctx.in.drop(Core::TOK_Comma))
            off = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return T(std::move(arg), std::move(off));
      }

      //
      // GetArg3
      //
      template<typename T>
      static T GetArg3(ParserCtx const &ctx)
      {
         Core::FastU off = 0;

         TokenDrop(ctx, Core::TOK_ParenO, "'('");
         auto arg0 = GetArg(ctx);
         TokenDrop(ctx, Core::TOK_Comma, "','");
         auto arg1 = GetArg(ctx);
         if(ctx.in.drop(Core::TOK_Comma))
            off = GetFastU(ctx);
         TokenDrop(ctx, Core::TOK_ParenC, "')'");

         return T(std::move(arg0), std::move(arg1), std::move(off));
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
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

         case Core::STR_Aut:    return GetArg2<IR::Arg_Aut   >(ctx);
         case Core::STR_Far:    return GetArg2<IR::Arg_Far   >(ctx);
         case Core::STR_GblArs: return GetArg2<IR::Arg_GblArs>(ctx);
         case Core::STR_GblReg: return GetArg2<IR::Arg_GblReg>(ctx);
         case Core::STR_HubArs: return GetArg2<IR::Arg_HubArs>(ctx);
         case Core::STR_HubReg: return GetArg2<IR::Arg_HubReg>(ctx);
         case Core::STR_LocReg: return GetArg2<IR::Arg_LocReg>(ctx);
         case Core::STR_ModArs: return GetArg2<IR::Arg_ModArs>(ctx);
         case Core::STR_ModReg: return GetArg2<IR::Arg_ModReg>(ctx);
         case Core::STR_Sta:    return GetArg2<IR::Arg_Sta   >(ctx);
         case Core::STR_StrArs: return GetArg2<IR::Arg_StrArs>(ctx);
         case Core::STR_Vaa:    return GetArg2<IR::Arg_Vaa   >(ctx);

         case Core::STR_GblArr: return GetArg3<IR::Arg_GblArr>(ctx);
         case Core::STR_HubArr: return GetArg3<IR::Arg_HubArr>(ctx);
         case Core::STR_LocArr: return GetArg3<IR::Arg_LocArr>(ctx);
         case Core::STR_ModArr: return GetArg3<IR::Arg_ModArr>(ctx);
         case Core::STR_StrArr: return GetArg3<IR::Arg_StrArr>(ctx);

         default:
            ctx.in.unget();
            throw Core::ParseExceptExpect(ctx.in.peek(), "Arg", false);
         }
      }
   }
}

// EOF

