//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation code handling.
//
//-----------------------------------------------------------------------------

#include "IR/Code.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // operator OArchive << Code
   //
   OArchive &operator << (OArchive &out, Code in)
   {
      switch(in)
      {
         #define GDCC_IR_CodeList(name) \
            case Code::name: return out << Core::STR_##name;
         #include "IR/CodeList.hpp"
      case Code::None: return out << Core::STR_None;
      }

      Core::Error({}, "invalid enum GDCC::IR::Code");
   }

   //
   // operator std::ostream << Code
   //
   std::ostream &operator << (std::ostream &out, Code in)
   {
      switch(in)
      {
         #define GDCC_IR_CodeList(name) \
            case Code::name: return out << #name;
         #include "IR/CodeList.hpp"
      case Code::None: return out << "None";
      }

      Core::Error({}, "invalid enum GDCC::IR::Code");
   }

   //
   // operator IArchive >> Code
   //
   IArchive &operator >> (IArchive &in, Code &out)
   {
      switch(GetIR<Core::StringIndex>(in))
      {
         #define GDCC_IR_CodeList(name) \
            case Core::STR_##name: out = Code::name; return in;
         #include "IR/CodeList.hpp"
      case Core::STR_None: out = Code::None; return in;

      default:
         Core::Error({}, "invalid Code");
      }
   }
}

// EOF

