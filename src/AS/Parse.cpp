//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"
#include "IR/ScriptType.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ExpectToken
      //
      Core::TokenStream &ExpectToken(Core::TokenStream &in, Core::TokenType tt,
         char const *str)
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
      IR::AddrBase ParseAddrBase(Core::Token const &tok)
      {
         switch(tok.str)
         {
            #define GDCC_IR_AddrList(name) \
               case Core::STR_##name: return IR::AddrBase::name;
            #include "IR/AddrList.hpp"

         default:
            std::cerr << "ERROR: " << tok.pos << ": bad AddrBase: '" << tok.str << "'\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // ParseCallType
      //
      IR::CallType ParseCallType(Core::Token const &tok)
      {
         switch(tok.str)
         {
         case Core::STR_Action:  return IR::CallType::Action;
         case Core::STR_AsmFunc: return IR::CallType::AsmFunc;
         case Core::STR_LangACS: return IR::CallType::LangACS;
         case Core::STR_LangASM: return IR::CallType::LangASM;
         case Core::STR_LangAXX: return IR::CallType::LangAXX;
         case Core::STR_LangC:   return IR::CallType::LangC;
         case Core::STR_LangCXX: return IR::CallType::LangCXX;
         case Core::STR_LangDS:  return IR::CallType::LangDS;
         case Core::STR_Native:  return IR::CallType::Native;
         case Core::STR_Script:  return IR::CallType::Script;
         case Core::STR_ScriptI: return IR::CallType::ScriptI;
         case Core::STR_ScriptS: return IR::CallType::ScriptS;
         case Core::STR_Special: return IR::CallType::Special;

         default:
            std::cerr << "ERROR: " << tok.pos << ": bad CallType: '" << tok.str << "'\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // ParseLinkage
      //
      IR::Linkage ParseLinkage(Core::Token const &tok)
      {
         switch(tok.str)
         {
         case Core::STR_ExtACS: return IR::Linkage::ExtACS;
         case Core::STR_ExtASM: return IR::Linkage::ExtASM;
         case Core::STR_ExtC:   return IR::Linkage::ExtC;
         case Core::STR_ExtCXX: return IR::Linkage::ExtCXX;
         case Core::STR_ExtDS:  return IR::Linkage::ExtDS;
         case Core::STR_IntC:   return IR::Linkage::IntC;
         case Core::STR_IntCXX: return IR::Linkage::IntCXX;

         default:
            std::cerr << "ERROR: " << tok.pos << ": bad Linkage: '" << tok.str << "'\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // ParseScriptType
      //
      IR::ScriptType ParseScriptType(Core::Token const &tok)
      {
         switch(tok.str)
         {
         case Core::STR_None:       return IR::ScriptType::None;
         case Core::STR_Death:      return IR::ScriptType::Death;
         case Core::STR_Disconnect: return IR::ScriptType::Disconnect;
         case Core::STR_Enter:      return IR::ScriptType::Enter;
         case Core::STR_Lightning:  return IR::ScriptType::Lightning;
         case Core::STR_Open:       return IR::ScriptType::Open;
         case Core::STR_Respawn:    return IR::ScriptType::Respawn;
         case Core::STR_Return:     return IR::ScriptType::Return;
         case Core::STR_Unloading:  return IR::ScriptType::Unloading;

         default:
            std::cerr << "ERROR: " << tok.pos << ": bad ScriptType: '" << tok.str << "'\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // SkipToken
      //
      Core::TokenStream &SkipToken(Core::TokenStream &in, Core::TokenType tt,
         char const *str)
      {
         if(!in.drop(tt))
         {
            std::cerr << "ERROR: " << in.peek().pos << ": expected " << str << '\n';
            throw EXIT_FAILURE;
         }

         return in;
      }
   }
}

// EOF

