//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Object parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Object.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseObject
      //
      void ParseObject(ParserCtx const &ctx, IR::Object &obj)
      {
         while(!ctx.in.drop(Core::TOK_LnEnd))
            switch(TokenPeekIdenti(ctx).in.get().str)
         {
         case Core::STR_alias: obj.alias = GetFastU(TokenDropEq(ctx));     break;
         case Core::STR_alloc: obj.alloc = GetFastU(TokenDropEq(ctx));     break;
         case Core::STR_defin: obj.defin = GetFastU(TokenDropEq(ctx));     break;
         case Core::STR_initi: obj.initi = GetExp(TokenDropEq(ctx));       break;
         case Core::STR_linka: obj.linka = GetLinkage(TokenDropEq(ctx));   break;
         case Core::STR_space: obj.space = GetAddrSpace(TokenDropEq(ctx)); break;
         case Core::STR_value: obj.value = GetFastU(TokenDropEq(ctx));     break;
         case Core::STR_words: obj.words = GetFastU(TokenDropEq(ctx));     break;

         default:
            throw Core::ParseExceptExpect(ctx.in.reget(), "Object argument", false);
         }
      }
   }
}

// EOF

