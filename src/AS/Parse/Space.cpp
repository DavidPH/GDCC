//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Space parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Space.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseSpace
      //
      void ParseSpace(Core::TokenStream &in, IR::Program &prog,
         IR::Space &space)
      {
         while(!in.drop(Core::TOK_LnEnd)) switch(static_cast<Core::StringIndex>(
            ExpectToken(in, Core::TOK_Identi, "identifier").get().str))
         {
         case Core::STR_alloc:
            space.alloc = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_defin:
            space.defin = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_linka:
            space.linka = ParseLinkage(SkipToken(in, Core::TOK_Equal, "=").get());
            break;

         case Core::STR_value:
            space.value = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_words:
            space.words = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         default:
            in.unget();
            std::cerr << "ERROR: " << in.peek().pos << ": bad Space argument: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

