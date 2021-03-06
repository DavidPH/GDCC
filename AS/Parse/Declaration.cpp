//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Declaration parsing.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "AS/Macro.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseDeclaration
   //
   void ParseDeclaration(ParserCtx const &ctx)
   {
      switch(TokenPeekIdenti(ctx).in.get().str)
      {
      case Core::STR_DJump:
         TokenPeekString(ctx);
         ParseDJump(ctx, ctx.prog.getDJump(ctx.in.get().str));
         break;

      case Core::STR_Function:
         TokenPeekString(ctx);
         ParseFunction(ctx, ctx.prog.getFunction(ctx.in.get().str));
         break;

      case Core::STR_Glyph:
         TokenPeekString(ctx);
         ParseGlyph(ctx, ctx.prog.getGlyphData(ctx.in.get().str));
         break;

      case Core::STR_Import:
         TokenPeekString(ctx);
         ParseImport(ctx, ctx.prog.getImport(ctx.in.get().str));
         break;

      case Core::STR_Macro:
         TokenPeekString(ctx);
         {
            Core::String name = ctx.in.get().str;
            IR::Block    list;

            TokenDrop(ctx, Core::TOK_ParenO, "'('");
            ParseBlock(ctx, list, Core::TOK_ParenC);

            ctx.macros.add(name, Macro(std::move(list)));
         }
         break;

      case Core::STR_Object:
         TokenPeekString(ctx);
         ParseObject(ctx, ctx.prog.getObject(ctx.in.get().str));
         break;

      case Core::STR_Space:
         ParseSpace(ctx, ctx.prog.getSpace(GetAddrSpace(ctx)));
         break;

      case Core::STR_StrEnt:
         TokenPeekString(ctx);
         ParseStrEnt(ctx, ctx.prog.getStrEnt(ctx.in.get().str));
         break;

      default:
         ctx.in.unget();
         Core::ErrorExpect("declaration", ctx.in.peek());
      }
   }
}

// EOF

