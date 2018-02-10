//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// StrEnt parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/StrEnt.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseStrEnt
   //
   void ParseStrEnt(ParserCtx const &ctx, IR::StrEnt &str)
   {
      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      while(!ctx.in.drop(Core::TOK_ParenC))
         switch(TokenPeekIdenti(ctx).in.get().str)
      {
      case Core::STR_alias:    str.alias    = GetFastU(TokenDropEq(ctx));  break;
      case Core::STR_alloc:    str.alloc    = GetFastU(TokenDropEq(ctx));  break;
      case Core::STR_defin:    str.defin    = GetFastU(TokenDropEq(ctx));  break;
      case Core::STR_multiDef: str.multiDef = GetFastU(TokenDropEq(ctx));  break;
      case Core::STR_valueInt: str.valueInt = GetFastU(TokenDropEq(ctx));  break;
      case Core::STR_valueStr: str.valueStr = GetString(TokenDropEq(ctx)); break;

      default:
         throw Core::ParseExceptExpect(ctx.in.reget(), "StrEnt argument", false);
      }
   }
}

// EOF

