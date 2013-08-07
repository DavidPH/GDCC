//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Address space handling.
//
//-----------------------------------------------------------------------------

#include "Addr.hpp"

#include "IArchive.hpp"

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
               case AddrBase::name: return out << STR_##name;
            #include "AddrList.hpp"
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
            #include "AddrList.hpp"
         }
      }

      //
      // operator IArchive >> AddrBase
      //
      IArchive &operator >> (IArchive &in, AddrBase &out)
      {
         switch(GetIR<StringIndex>(in))
         {
            #define GDCC_IR_AddrList(name) \
               case STR_##name: out = AddrBase::name; return in;
            #include "AddrList.hpp"

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

