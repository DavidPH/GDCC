//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Block parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "AS/Macro.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Block.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseBlock
   //
   void ParseBlock(ParserCtx const &ctx, IR::Block &block, Core::TokenType end)
   {
      std::vector<IR::Arg> args;
      IR::Code             code;
      Core::FastU          w, n;

      while(!ctx.in.drop(end)) switch(ctx.in.peek().tok)
      {
      case Core::TOK_String:
         block.addLabel(ctx.in.get().str);
         break;

      case Core::TOK_Identi:
         block.setOrigin(ctx.in.peek().pos);

         code = GetCode(ctx);

         if(ctx.in.drop(Core::TOK_Identi, Core::STR_B))
            w = 1, n = 1;
         else if(ctx.in.drop(Core::TOK_Identi, Core::STR_W))
            w = Target::GetWordBytes(), n = 1;
         else
            w = 1, n = Target::GetWordBytes();

         if(!ctx.in.peek(Core::TOK_ParenO))
            n = GetFastU(ctx);

         // Read arguments to instruction.
         args.clear();

         TokenDrop(ctx, Core::TOK_ParenO, "'('");

         while(!ctx.in.drop(Core::TOK_ParenC))
            args.push_back(GetArg(ctx, w, n));

         block.addStmntArgs(code, {Core::Move, args.begin(), args.end()});

         break;

      case Core::TOK_Not:
         ctx.in.get();
         if(auto macro = ctx.macros.find(TokenPeekString(ctx).in.peek()))
         {
            block.setOrigin(ctx.in.get().pos);

            if(ctx.in.drop(Core::TOK_Identi, Core::STR_B))
               w = 1, n = 1;
            else if(ctx.in.drop(Core::TOK_Identi, Core::STR_W))
               w = Target::GetWordBytes(), n = 1;
            else
               w = 1, n = Target::GetWordBytes();

            if(!ctx.in.peek(Core::TOK_ParenO))
               n = GetFastU(ctx);

            // Read arguments to macro invocation.
            args.clear();

            TokenDrop(ctx, Core::TOK_ParenO, "'('");

            while(ctx.in.drop(Core::TOK_ParenC))
               args.push_back(GetArg(ctx, w, n));

            TokenDrop(ctx, Core::TOK_LnEnd, "end of line");

            // Expand macro.
            macro->expand(block, args.data(), args.size());
         }
         else
            Core::ErrorExpect("macro name", ctx.in.peek());

         break;

      case Core::TOK_LnEnd:
         ctx.in.get();
         break;

      default:
         Core::ErrorExpect("block terminator", ctx.in.peek());
      }
   }
}

// EOF

