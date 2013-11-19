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

#include "Parse.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Function.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseFunction
   //
   void ParseFunction(GDCC::TokenStream &in, GDCC::IR::Program &prog,
      GDCC::IR::Function &func)
   {
      while(!in.drop(GDCC::TOK_LnEnd)) switch(static_cast<GDCC::StringIndex>(
         ExpectToken(in, GDCC::TOK_Identi, "identifier").get().str))
      {
      case GDCC::STR_alloc:
         func.alloc = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_block:
         while(in.drop(GDCC::TOK_LnEnd)) {}
         SkipToken(in, GDCC::TOK_BraceO, "{");
         ParseBlock(in, prog, func.block, GDCC::TOK_BraceC);
         break;

      case GDCC::STR_ctype:
         func.ctype = ParseCallType(SkipToken(in, GDCC::TOK_Equal, "=").get());
         break;

      case GDCC::STR_defin:
         func.defin = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_label:
         SkipToken(in, GDCC::TOK_Equal, "=");
         func.label = ExpectToken(in, GDCC::TOK_String, "string").get().str;
         break;

      case GDCC::STR_linka:
         func.linka = ParseLinkage(SkipToken(in, GDCC::TOK_Equal, "=").get());
         break;

      case GDCC::STR_localArs:
         func.localArs = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_localReg:
         func.localReg = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_param:
         func.param = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_retrn:
         func.retrn = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_sflagClS:
         func.sflagClS = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_sflagNet:
         func.sflagNet = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_stype:
         func.stype = ParseScriptType(SkipToken(in, GDCC::TOK_Equal, "=").get());
         break;

      case GDCC::STR_valueInt:
         func.valueInt = ParseFastI(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_valueStr:
         SkipToken(in, GDCC::TOK_Equal, "=");
         func.valueStr = ExpectToken(in, GDCC::TOK_String, "string").get().str;
         break;

      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad Function argument: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

