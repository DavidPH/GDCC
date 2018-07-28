//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::AS
{
   //
   // GetArgSize
   //
   static Core::FastU GetArgSize(ParserCtx const &ctx, Core::FastU w, Core::FastU n)
   {
      if(ctx.in.drop(Core::TOK_Identi, Core::STR_B))
         w = 1, n = 1;
      else if(ctx.in.drop(Core::TOK_Identi, Core::STR_W))
         w = Platform::GetWordBytes(), n = 1;

      if(!ctx.in.peek(Core::TOK_ParenO))
         n = GetFastU(ctx);

      return w * n;
   }

   //
   // GetArg0
   //
   template<typename T>
   static T GetArg0(ParserCtx const &ctx, Core::FastU w, Core::FastU n)
   {
      Core::FastU size = GetArgSize(ctx, w, n);

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return T(size);
   }

   //
   // GetArgU
   //
   template<typename T>
   static T GetArgU(ParserCtx const &ctx, Core::FastU w, Core::FastU n)
   {
      Core::FastU off  = 0;
      Core::FastU size = GetArgSize(ctx, w, n);

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      if(!ctx.in.peek(Core::TOK_ParenC))
         off = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return T(size, std::move(off));
   }

   //
   // GetArg1
   //
   template<typename T>
   static T GetArg1(ParserCtx const &ctx, Core::FastU w, Core::FastU n)
   {
      Core::FastU off  = 0;
      Core::FastU size = GetArgSize(ctx, w, n);

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto exp = GetExp(ctx);
      if(!ctx.in.peek(Core::TOK_ParenC))
         off = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return T(size, std::move(exp), std::move(off));
   }

   //
   // GetArg2
   //
   template<typename T>
   static T GetArg2(ParserCtx const &ctx, Core::FastU w, Core::FastU n)
   {
      Core::FastU off  = 0;
      Core::FastU size = GetArgSize(ctx, w, n);

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto arg = GetArg(ctx, Platform::GetWordBytes(), 1);
      if(!ctx.in.peek(Core::TOK_ParenC))
         off = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return T(size, std::move(arg), std::move(off));
   }

   //
   // GetArg3
   //
   template<typename T>
   static T GetArg3(ParserCtx const &ctx, Core::FastU w, Core::FastU n)
   {
      Core::FastU off  = 0;
      Core::FastU size = GetArgSize(ctx, w, n);

      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      auto arg0 = GetArg(ctx, Platform::GetWordBytes(), 1);
      auto arg1 = GetArg(ctx, Platform::GetWordBytes(), 1);
      if(!ctx.in.peek(Core::TOK_ParenC))
         off = GetFastU(ctx);
      TokenDrop(ctx, Core::TOK_ParenC, "')'");

      return T(size, std::move(arg0), std::move(arg1), std::move(off));
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // GetArg
   //
   IR::Arg GetArg(ParserCtx const &ctx, Core::FastU w, Core::FastU n)
   {
      switch(TokenPeekIdenti(ctx).in.get().str)
      {
      case Core::STR_Gen:    return GetArg2<IR::Arg_Gen   >(ctx, w, n);

      case Core::STR_Cpy:    return GetArgU<IR::Arg_Cpy   >(ctx, w, n);
      case Core::STR_Lit:    return GetArg1<IR::Arg_Lit   >(ctx, w, n);
      case Core::STR_Nul:    return GetArg0<IR::Arg_Nul   >(ctx, w, n);
      case Core::STR_Stk:    return GetArg0<IR::Arg_Stk   >(ctx, w, n);

      case Core::STR_Aut:    return GetArg2<IR::Arg_Aut   >(ctx, w, n);
      case Core::STR_Far:    return GetArg2<IR::Arg_Far   >(ctx, w, n);
      case Core::STR_GblArs: return GetArg2<IR::Arg_GblArs>(ctx, w, n);
      case Core::STR_GblReg: return GetArg2<IR::Arg_GblReg>(ctx, w, n);
      case Core::STR_HubArs: return GetArg2<IR::Arg_HubArs>(ctx, w, n);
      case Core::STR_HubReg: return GetArg2<IR::Arg_HubReg>(ctx, w, n);
      case Core::STR_LocReg: return GetArg2<IR::Arg_LocReg>(ctx, w, n);
      case Core::STR_ModArs: return GetArg2<IR::Arg_ModArs>(ctx, w, n);
      case Core::STR_ModReg: return GetArg2<IR::Arg_ModReg>(ctx, w, n);
      case Core::STR_Sta:    return GetArg2<IR::Arg_Sta   >(ctx, w, n);
      case Core::STR_StrArs: return GetArg2<IR::Arg_StrArs>(ctx, w, n);
      case Core::STR_Vaa:    return GetArg2<IR::Arg_Vaa   >(ctx, w, n);

      case Core::STR_GblArr: return GetArg3<IR::Arg_GblArr>(ctx, w, n);
      case Core::STR_HubArr: return GetArg3<IR::Arg_HubArr>(ctx, w, n);
      case Core::STR_LocArr: return GetArg3<IR::Arg_LocArr>(ctx, w, n);
      case Core::STR_ModArr: return GetArg3<IR::Arg_ModArr>(ctx, w, n);
      case Core::STR_StrArr: return GetArg3<IR::Arg_StrArr>(ctx, w, n);

      default:
         ctx.in.unget();
         Core::ErrorExpect("Arg", ctx.in.peek());
      }
   }
}

// EOF

