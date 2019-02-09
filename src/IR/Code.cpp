//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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
      return out << in.base << in.type;
   }

   //
   // operator OArchive << CodeBase
   //
   OArchive &operator << (OArchive &out, CodeBase in)
   {
      switch(in)
      {
         #define GDCC_IR_CodeList(name) \
            case CodeBase::name: return out << Core::STR_##name;
         #include "IR/CodeList.hpp"
      case CodeBase::None: return out << Core::STR_None;
      }

      Core::Error({}, "invalid enum GDCC::IR::CodeBase");
   }

   //
   // operator OArchive << CodeType
   //
   OArchive &operator << (OArchive &out, CodeType in)
   {
      return out << in[0] << in[1] << in[2] << in[3];
   }

   //
   // operator std::ostream << Code
   //
   std::ostream &operator << (std::ostream &out, Code in)
   {
      out << in.base;
      if(in.type) out << ':' << in.type;
      return out;
   }

   //
   // operator std::ostream << CodeBase
   //
   std::ostream &operator << (std::ostream &out, CodeBase in)
   {
      switch(in)
      {
         #define GDCC_IR_CodeList(name) \
            case CodeBase::name: return out << #name;
         #include "IR/CodeList.hpp"
      case CodeBase::None: return out << "None";
      }

      Core::Error({}, "invalid enum GDCC::IR::CodeBase");
   }

   //
   // operator std::ostream << CodeType
   //
   std::ostream &operator << (std::ostream &out, CodeType in)
   {
      for(auto t : in)
         out << t;
      return out;
   }

   //
   // operator IArchive >> Code
   //
   IArchive &operator >> (IArchive &in, Code &out)
   {
      return in >> out.base >> out.type;
   }

   //
   // operator IArchive >> CodeBase
   //
   IArchive &operator >> (IArchive &in, CodeBase &out)
   {
      switch(GetIR<Core::StringIndex>(in))
      {
         #define GDCC_IR_CodeList(name) \
            case Core::STR_##name: out = CodeBase::name; return in;
         #include "IR/CodeList.hpp"
      case Core::STR_None: out = CodeBase::None; return in;

      default:
         Core::Error({}, "invalid CodeBase");
      }
   }

   //
   // operator IArchive >> CodeType
   //
   IArchive &operator >> (IArchive &in, CodeType &out)
   {
      return in >> out[0] >> out[1] >> out[2] >> out[3];
   }
}

// EOF

