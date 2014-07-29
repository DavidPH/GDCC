//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Object parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Object.hpp"

#include <iostream>


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
      void ParseObject(Core::TokenStream &in, IR::Program &prog,
         IR::Object &obj)
      {
         while(!in.drop(Core::TOK_LnEnd)) switch(static_cast<Core::StringIndex>(
            ExpectToken(in, Core::TOK_Identi, "identifier").get().str))
         {
         case Core::STR_alias:
            obj.alias = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_alloc:
            obj.alloc = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_defin:
            obj.defin = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_initi:
            obj.initi = ParseExp(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_linka:
            obj.linka = ParseLinkage(SkipToken(in, Core::TOK_Equal, "=").get());
            break;

         case Core::STR_value:
            obj.value = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_words:
            obj.words = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         default:
            in.unget();
            std::cerr << "ERROR: " << in.peek().pos << ": bad Object argument: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

