//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "AS/LabelTBuf.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseFunction
   //
   void ParseFunction(ParserCtx const &ctx_, IR::Function &func)
   {
      // Insert label mangling TokenBuf into parser.
      LabelTBuf         ltb{*ctx_.in.tkbuf(), func.glyph};
      Core::TokenStream ts {&ltb};
      ParserCtx         ctx{ctx_, ts};

      while(!ctx.in.drop(Core::TOK_LnEnd))
         switch(TokenPeekIdenti(ctx).in.get().str)
      {
      case Core::STR_allocAut: func.alloc    = GetFastU(TokenDropEq(ctx));      break;
      case Core::STR_alloc:    func.alloc    = GetFastU(TokenDropEq(ctx));      break;
      case Core::STR_ctype:    func.ctype    = GetCallType(TokenDropEq(ctx));   break;
      case Core::STR_defin:    func.defin    = GetFastU(TokenDropEq(ctx));      break;
      case Core::STR_label:    func.label    = GetString(TokenDropEq(ctx));     break;
      case Core::STR_linka:    func.linka    = GetLinkage(TokenDropEq(ctx));    break;
      case Core::STR_localAut: func.localAut = GetFastU(TokenDropEq(ctx));      break;
      case Core::STR_localReg: func.localReg = GetFastU(TokenDropEq(ctx));      break;
      case Core::STR_param:    func.param    = GetFastU(TokenDropEq(ctx));      break;
      case Core::STR_retrn:    func.retrn    = GetFastU(TokenDropEq(ctx));      break;
      case Core::STR_valueInt: func.valueInt = GetFastI(TokenDropEq(ctx));      break;
      case Core::STR_valueStr: func.valueStr = GetString(TokenDropEq(ctx));     break;

      case Core::STR_block:
         while(ctx.in.drop(Core::TOK_LnEnd)) {}
         TokenDrop(ctx, Core::TOK_BraceO, "'{'");
         ParseBlock(ctx, func.block, Core::TOK_BraceC);
         break;

      case Core::STR_localArr:
         {
            TokenDrop(ctx, Core::TOK_BrackO, "'['");
            auto arr = GetFastU(ctx);
            TokenDrop(ctx, Core::TOK_BrackC, "']'");
            func.localArr[arr] = GetFastU(TokenDropEq(ctx));
         }
         break;

      case Core::STR_stype:
         {
            TokenDropEq(ctx);

            TokenDrop(ctx, Core::TOK_BrackO, "'['");
            std::vector<Core::String> stype;
            if(!ctx.in.peek(Core::TOK_BrackC)) do
               stype.push_back(TokenPeekString(ctx).in.get().str);
            while(ctx.in.drop(Core::TOK_Comma));
            TokenDrop(ctx, Core::TOK_BrackC, "']'");

            func.stype = {stype.begin(), stype.end()};
         }
         break;

      default:
         throw Core::ParseExceptExpect(ctx.in.reget(), "Function argument", false);
      }
   }
}

// EOF

