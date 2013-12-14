//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Function.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseFunction
      //
      void ParseFunction(Core::TokenStream &in, IR::Program &prog,
         IR::Function &func)
      {
         while(!in.drop(Core::TOK_LnEnd)) switch(static_cast<Core::StringIndex>(
            ExpectToken(in, Core::TOK_Identi, "identifier").get().str))
         {
         case Core::STR_alloc:
            func.alloc = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_block:
            while(in.drop(Core::TOK_LnEnd)) {}
            SkipToken(in, Core::TOK_BraceO, "{");
            ParseBlock(in, prog, func.block, Core::TOK_BraceC);
            break;

         case Core::STR_ctype:
            func.ctype = ParseCallType(SkipToken(in, Core::TOK_Equal, "=").get());
            break;

         case Core::STR_defin:
            func.defin = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_label:
            SkipToken(in, Core::TOK_Equal, "=");
            func.label = ExpectToken(in, Core::TOK_String, "string").get().str;
            break;

         case Core::STR_linka:
            func.linka = ParseLinkage(SkipToken(in, Core::TOK_Equal, "=").get());
            break;

         case Core::STR_localArs:
            func.localArs = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_localReg:
            func.localReg = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_param:
            func.param = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_retrn:
            func.retrn = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_sflagClS:
            func.sflagClS = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_sflagNet:
            func.sflagNet = ParseFastU(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_stype:
            func.stype = ParseScriptType(SkipToken(in, Core::TOK_Equal, "=").get());
            break;

         case Core::STR_valueInt:
            func.valueInt = ParseFastI(SkipToken(in, Core::TOK_Equal, "="), prog);
            break;

         case Core::STR_valueStr:
            SkipToken(in, Core::TOK_Equal, "=");
            func.valueStr = ExpectToken(in, Core::TOK_String, "string").get().str;
            break;

         default:
            in.unget();
            std::cerr << "ERROR: " << in.peek().pos << ": bad Function argument: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

