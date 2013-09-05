//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Addr.hpp"
#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Linkage.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ExpectToken
   //
   GDCC::TokenStream &ExpectToken(GDCC::TokenStream &in, GDCC::TokenType tt, char const *str)
   {
      if(in.peek().tok != tt)
      {
         std::cerr << "ERROR: " << in.peek().pos << ": expected " << str << '\n';
         throw EXIT_FAILURE;
      }

      return in;
   }

   //
   // ParseAddrBase
   //
   GDCC::IR::AddrBase ParseAddrBase(GDCC::Token const &tok)
   {
      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
         #define GDCC_IR_AddrList(name) \
            case GDCC::STR_##name: return GDCC::IR::AddrBase::name;
         #include "GDCC/IR/AddrList.hpp"

      default:
         std::cerr << "ERROR: " << tok.pos << ": bad AddrBase: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // ParseCallType
   //
   GDCC::IR::CallType ParseCallType(GDCC::Token const &tok)
   {
      switch(static_cast<GDCC::StringIndex>(tok.str))
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
         std::cerr << "ERROR: " << tok.pos << ": bad CallType: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // ParseLinkage
   //
   GDCC::IR::Linkage ParseLinkage(GDCC::Token const &tok)
   {
      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_ExtACS: return GDCC::IR::Linkage::ExtACS;
      case GDCC::STR_ExtASM: return GDCC::IR::Linkage::ExtASM;
      case GDCC::STR_ExtC:   return GDCC::IR::Linkage::ExtC;
      case GDCC::STR_ExtCXX: return GDCC::IR::Linkage::ExtCXX;
      case GDCC::STR_ExtDS:  return GDCC::IR::Linkage::ExtDS;
      case GDCC::STR_IntC:   return GDCC::IR::Linkage::IntC;
      case GDCC::STR_IntCXX: return GDCC::IR::Linkage::IntCXX;

      default:
         std::cerr << "ERROR: " << tok.pos << ": bad Linkage: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // ParseScriptType
   //
   GDCC::IR::ScriptType ParseScriptType(GDCC::Token const &tok)
   {
      switch(static_cast<GDCC::StringIndex>(tok.str))
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
         std::cerr << "ERROR: " << tok.pos << ": bad ScriptType: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // SkipToken
   //
   GDCC::TokenStream &SkipToken(GDCC::TokenStream &in, GDCC::TokenType tt, char const *str)
   {
      if(!in.drop(tt))
      {
         std::cerr << "ERROR: " << in.peek().pos << ": expected " << str << '\n';
         throw EXIT_FAILURE;
      }

      return in;
   }
}

// EOF

