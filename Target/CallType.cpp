//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function call type/convention handling.
//
//-----------------------------------------------------------------------------

#include "Target/CallType.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Target
{
   //
   // operator std::ostream << CallType
   //
   std::ostream &operator << (std::ostream &out, CallType in)
   {
      switch(in)
      {
         #define GDCC_Target_CallTypeList(name) \
            case CallType::name: return out << #name;
         #include "Target/CallTypeList.hpp"
      }

      Core::Error({}, "invalid enum GDCC::Target::CallType");
   }

   //
   // GetCallTypeIR
   //
   CallType GetCallTypeIR(CallType ctype)
   {
      switch(ctype)
      {
      case CallType::LangACS:
         return CallType::StkCall;

      case CallType::LangASM:
      case CallType::LangAXX:
      case CallType::LangC:
      case CallType::LangCXX:
      case CallType::LangDS:
         return CallType::StdCall;

      case CallType::SScript:
         return CallType::SScriptI;

      case CallType::Script:
         return CallType::ScriptI;

      default:
         return ctype;
      }
   }
}

// EOF

