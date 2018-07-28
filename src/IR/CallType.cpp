//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // operator OArchive << CallType
   //
   OArchive &operator << (OArchive &out, CallType in)
   {
      switch(in)
      {
         #define GDCC_IR_CallTypeList(name) \
            case CallType::name: return out << Core::STR_##name;
         #include "IR/CallTypeList.hpp"
      }

      Core::Error({}, "invalid enum GDCC::IR::CallType");
   }

   //
   // operator std::ostream << CallType
   //
   std::ostream &operator << (std::ostream &out, CallType in)
   {
      switch(in)
      {
         #define GDCC_IR_CallTypeList(name) \
            case CallType::name: return out << #name;
         #include "IR/CallTypeList.hpp"
      }

      Core::Error({}, "invalid enum GDCC::IR::CallType");
   }

   //
   // operator IArchive >> CallType
   //
   IArchive &operator >> (IArchive &in, CallType &out)
   {
      switch(GetIR<Core::String>(in))
      {
         #define GDCC_IR_CallTypeList(name) \
            case Core::STR_##name: out = CallType::name; return in;
         #include "IR/CallTypeList.hpp"

      default:
         Core::Error({}, "invalid CallType");
      }
   }

   //
   // GetCallTypeIR
   //
   CallType GetCallTypeIR(CallType ctype)
   {
      switch(ctype)
      {
      case IR::CallType::LangACS:
         return IR::CallType::StkCall;

      case IR::CallType::LangASM:
      case IR::CallType::LangAXX:
      case IR::CallType::LangC:
      case IR::CallType::LangCXX:
      case IR::CallType::LangDS:
         return IR::CallType::StdCall;

      case IR::CallType::SScript:
         return IR::CallType::SScriptI;

      case IR::CallType::Script:
         return IR::CallType::ScriptI;

      default:
         return ctype;
      }
   }
}

// EOF

