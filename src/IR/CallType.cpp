//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function call type/convention handling.
//
//-----------------------------------------------------------------------------

#include "IR/CallType.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include "Core/String.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << CallType
      //
      OArchive &operator << (OArchive &out, CallType in)
      {
         switch(in)
         {
         case CallType::None:    return out << Core::STR_None;
         case CallType::Action:  return out << Core::STR_Action;
         case CallType::AsmFunc: return out << Core::STR_AsmFunc;
         case CallType::LangACS: return out << Core::STR_LangACS;
         case CallType::LangASM: return out << Core::STR_LangASM;
         case CallType::LangAXX: return out << Core::STR_LangAXX;
         case CallType::LangC:   return out << Core::STR_LangC;
         case CallType::LangCXX: return out << Core::STR_LangCXX;
         case CallType::LangDS:  return out << Core::STR_LangDS;
         case CallType::Native:  return out << Core::STR_Native;
         case CallType::Script:  return out << Core::STR_Script;
         case CallType::ScriptI: return out << Core::STR_ScriptI;
         case CallType::ScriptS: return out << Core::STR_ScriptS;
         case CallType::Special: return out << Core::STR_Special;
         }

         std::cerr << "invalid enum CallType\n";
         throw EXIT_FAILURE;
      }

      //
      // operator std::ostream << CallType
      //
      std::ostream &operator << (std::ostream &out, CallType in)
      {
         switch(in)
         {
         case CallType::None:    return out << "None";
         case CallType::Action:  return out << "Action";
         case CallType::AsmFunc: return out << "AsmFunc";
         case CallType::LangACS: return out << "LangACS";
         case CallType::LangASM: return out << "LangASM";
         case CallType::LangAXX: return out << "LangAXX";
         case CallType::LangC:   return out << "LangC";
         case CallType::LangCXX: return out << "LangCXX";
         case CallType::LangDS:  return out << "LangDS";
         case CallType::Native:  return out << "Native";
         case CallType::Script:  return out << "Script";
         case CallType::ScriptI: return out << "ScriptI";
         case CallType::ScriptS: return out << "ScriptS";
         case CallType::Special: return out << "Special";
         }

         std::cerr << "invalid enum CallType\n";
         throw EXIT_FAILURE;
      }

      //
      // operator IArchive >> CallType
      //
      IArchive &operator >> (IArchive &in, CallType &out)
      {
         switch(GetIR<Core::String>(in))
         {
         case Core::STR_None:    out = CallType::None;    return in;
         case Core::STR_Action:  out = CallType::Action;  return in;
         case Core::STR_AsmFunc: out = CallType::AsmFunc; return in;
         case Core::STR_LangACS: out = CallType::LangACS; return in;
         case Core::STR_LangASM: out = CallType::LangASM; return in;
         case Core::STR_LangAXX: out = CallType::LangAXX; return in;
         case Core::STR_LangC:   out = CallType::LangC;   return in;
         case Core::STR_LangCXX: out = CallType::LangCXX; return in;
         case Core::STR_LangDS:  out = CallType::LangDS;  return in;
         case Core::STR_Native:  out = CallType::Native;  return in;
         case Core::STR_Script:  out = CallType::Script;  return in;
         case Core::STR_ScriptI: out = CallType::ScriptI; return in;
         case Core::STR_ScriptS: out = CallType::ScriptS; return in;
         case Core::STR_Special: out = CallType::Special; return in;

         default:
            std::cerr << "invalid CallType\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

