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

#include "GDCC/Token.hpp"

#include "GDCC/IR/Function.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ParseCallType
//
static GDCC::IR::CallType ParseCallType(Asm::IStream &in)
{
   GDCC::Token tok;
   switch(static_cast<GDCC::StringIndex>((in >> tok, tok).str))
   {
   case GDCC::STR_Action:  return GDCC::IR::CallType::Action;
   case GDCC::STR_AsmFunc: return GDCC::IR::CallType::AsmFunc;
   case GDCC::STR_LangACS: return GDCC::IR::CallType::LangACS;
   case GDCC::STR_LangASM: return GDCC::IR::CallType::LangASM;
   case GDCC::STR_LangAXX: return GDCC::IR::CallType::LangAXX;
   case GDCC::STR_LangC:   return GDCC::IR::CallType::LangC;
   case GDCC::STR_LangCXX: return GDCC::IR::CallType::LangCXX;
   case GDCC::STR_LangDS:  return GDCC::IR::CallType::LangDS;
   case GDCC::STR_Native:  return GDCC::IR::CallType::Native;
   case GDCC::STR_Script:  return GDCC::IR::CallType::Script;
   case GDCC::STR_ScriptI: return GDCC::IR::CallType::ScriptI;
   case GDCC::STR_ScriptS: return GDCC::IR::CallType::ScriptS;
   case GDCC::STR_Special: return GDCC::IR::CallType::Special;

   default:
      std::cerr << "ERROR: " << tok.pos << ": bad function ctype: '" << tok.str << "'\n";
      throw EXIT_FAILURE;
   }
}

//
// ParseScriptType
//
static GDCC::IR::ScriptType ParseScriptType(Asm::IStream &in)
{
   GDCC::Token tok;
   switch(static_cast<GDCC::StringIndex>((in >> tok, tok).str))
   {
   case GDCC::STR_None:       return GDCC::IR::ScriptType::None;
   case GDCC::STR_Death:      return GDCC::IR::ScriptType::Death;
   case GDCC::STR_Disconnect: return GDCC::IR::ScriptType::Disconnect;
   case GDCC::STR_Enter:      return GDCC::IR::ScriptType::Enter;
   case GDCC::STR_Lightning:  return GDCC::IR::ScriptType::Lightning;
   case GDCC::STR_Open:       return GDCC::IR::ScriptType::Open;
   case GDCC::STR_Respawn:    return GDCC::IR::ScriptType::Respawn;
   case GDCC::STR_Return:     return GDCC::IR::ScriptType::Return;
   case GDCC::STR_Unloading:  return GDCC::IR::ScriptType::Unloading;

   default:
      std::cerr << "ERROR: " << tok.pos << ": bad function stype: '" << tok.str << "'\n";
      throw EXIT_FAILURE;
   }
}

//
// SkipEqual
//
static Asm::IStream &SkipEqual(Asm::IStream &in)
{
   GDCC::Token tok;
   if((in >> tok, tok).tok != GDCC::TOK_Equal)
   {
      std::cerr << "ERROR: " << tok.pos << ": expected =\n";
      throw EXIT_FAILURE;
   }

   return in;
}


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
         while(in >> tok && tok.tok == GDCC::TOK_EOL) {}
         if(tok.tok != GDCC::TOK_BraceO)
         {
            std::cerr << "ERROR: " << tok.pos << ": expected {\n";
            throw EXIT_FAILURE;
         }
         ParseBlock(in, func.block, GDCC::TOK_BraceC);
         break;

      case GDCC::STR_ctype:
         func.ctype = ParseCallType(SkipEqual(in));
         break;

      case GDCC::STR_exdef:
         func.exdef = ParseFastU(SkipEqual(in));
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

      case GDCC::STR_stype:
         func.stype = ParseScriptType(SkipEqual(in));
         break;

      case GDCC::STR_valueInt:
         func.valueInt = ParseFastI(SkipEqual(in));
         break;

      case GDCC::STR_valueStr:
         func.valueStr = (SkipEqual(in) >> tok, tok).str;
         break;

      default:
         if(tok.tok == GDCC::TOK_EOL) return;

         std::cerr << "ERROR: " << tok.pos << ": bad function argument: '"
            << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

