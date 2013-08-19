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

#include "IStream.hpp"

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
   void ParseFunction(IStream &in, GDCC::IR::Function &func)
   {
      for(GDCC::Token tok; in >> tok;) switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_alloc:
         func.alloc = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_block:
         while(in >> tok && tok.tok == GDCC::TOK_LnEnd) {}
         if(tok.tok != GDCC::TOK_BraceO)
         {
            std::cerr << "ERROR: " << tok.pos << ": expected {\n";
            throw EXIT_FAILURE;
         }
         ParseBlock(in, func.block, GDCC::TOK_BraceC);
         break;

      case GDCC::STR_ctype:
         func.ctype = ParseCallType((SkipEqual(in) >> tok, tok));
         break;

      case GDCC::STR_defin:
         func.defin = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_label:
         func.label = (SkipEqual(in) >> tok, tok).str;
         break;

      case GDCC::STR_linka:
         func.linka = ParseLinkage((SkipEqual(in) >> tok, tok));
         break;

      case GDCC::STR_localArs:
         func.localArs = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_localReg:
         func.localReg = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_param:
         func.param = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_retrn:
         func.retrn = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_sflagClS:
         func.sflagClS = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_sflagNet:
         func.sflagNet = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_stype:
         func.stype = ParseScriptType((SkipEqual(in) >> tok, tok));
         break;

      case GDCC::STR_valueInt:
         func.valueInt = ParseFastI(SkipEqual(in));
         break;

      case GDCC::STR_valueStr:
         func.valueStr = (SkipEqual(in) >> tok, tok).str;
         break;

      default:
         if(tok.tok == GDCC::TOK_LnEnd) return;

         std::cerr << "ERROR: " << tok.pos << ": bad function argument: '"
            << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

