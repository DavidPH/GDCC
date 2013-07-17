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
   }
}

// EOF

