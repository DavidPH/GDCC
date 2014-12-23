//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseBlock
      //
      void ParseBlock(ParserCtx const &ctx, IR::Block &block, Core::TokenType end)
      {
         std::vector<IR::Arg> args;
         IR::OpCode op;

         while(!ctx.in.drop(end)) switch(ctx.in.peek().tok)
         {
         case Core::TOK_String:
            block.addLabel(ctx.in.get().str);
            break;

         case Core::TOK_Identi:
            block.setOrigin(ctx.in.peek().pos);

            op = GetOpCode(ctx);

            // Read arguments to instruction.
            args.clear();

            while(!ctx.in.peek(Core::TOK_LnEnd))
               args.push_back(GetArg(TokenDrop(ctx, Core::TOK_Comma, "end of line")));

            block.setArgs(Core::Array<IR::Arg>(Core::Move, args.begin(), args.end()));
            block.addStatement(op);

            break;

         case Core::TOK_Not:
            ctx.in.get();
            if(auto macro = ctx.macros.find(TokenPeekIdenti(ctx).in.peek()))
            {
               block.setOrigin(ctx.in.get().pos);

               // Read arguments to macro invocation.
               args.clear();

               while(!ctx.in.peek(Core::TOK_LnEnd))
                  args.push_back(GetArg(TokenDrop(ctx, Core::TOK_Comma, "end of line")));

               // Expand macro.
               macro->expand(block, args.data(), args.size());
            }
            else
               throw Core::ParseExceptExpect(ctx.in.peek(), "macro name", false);

            break;

         case Core::TOK_LnEnd:
            ctx.in.get();
            break;

         default:
            throw Core::ParseExceptExpect(ctx.in.peek(), "block terminator", false);
         }
      }
   }
}

// EOF

