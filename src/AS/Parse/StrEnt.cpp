//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// StrEnt parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/StrEnt.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseStrEnt
      //
      void ParseStrEnt(Core::TokenStream &in, IR::Program &prog,
         IR::StrEnt &str)
      {
         while(!in.drop(Core::TOK_LnEnd)) switch(static_cast<Core::StringIndex>(
            ExpectToken(in, Core::TOK_Identi, "identifier").get().str))
         {
         case Core::STR_alias:
            str.alias = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_alloc:
            str.alloc = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_defin:
            str.defin = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_valueInt:
            str.valueInt = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_valueStr:
            SkipToken(in, Core::TOK_Equal, "=");
            str.valueStr = ExpectToken(in, Core::TOK_String, "string").get().str;
            break;

         default:
            in.unget();
            std::cerr << "ERROR: " << in.peek().pos << ": bad StrEnt argument: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

