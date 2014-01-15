//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Address space handling.
//
//-----------------------------------------------------------------------------

#include "IR/Addr.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << AddrBase
      //
      OArchive &operator << (OArchive &out, AddrBase in)
      {
         switch(in)
         {
            #define GDCC_IR_AddrList(name) \
               case AddrBase::name: return out << Core::STR_##name;
            #include "IR/AddrList.hpp"
         }
      }

      //
      // operator OArchive << AddrSpace
      //
      OArchive &operator << (OArchive &out, AddrSpace in)
      {
         return out << in.base << in.name;
      }

      //
      // operator std::ostream << AddrBase
      //
      std::ostream &operator << (std::ostream &out, AddrBase in)
      {
         switch(in)
         {
            #define GDCC_IR_AddrList(name) \
               case AddrBase::name: return out << #name;
            #include "IR/AddrList.hpp"
         }
      }

      //
      // operator IArchive >> AddrBase
      //
      IArchive &operator >> (IArchive &in, AddrBase &out)
      {
         switch(GetIR<Core::StringIndex>(in))
         {
            #define GDCC_IR_AddrList(name) \
               case Core::STR_##name: out = AddrBase::name; return in;
            #include "IR/AddrList.hpp"

         default:
            std::cerr << "invalid AddrBase\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // operator IArchive >> AddrSpace
      //
      IArchive &operator >> (IArchive &in, AddrSpace &out)
      {
         return in >> out.base >> out.name;
      }
   }
}

// EOF

