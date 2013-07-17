//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Linkage type handling.
//
//-----------------------------------------------------------------------------

#include "Linkage.hpp"

#include "../String.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << Linkage
      //
      OArchive &operator << (OArchive &out, Linkage in)
      {
         switch(in)
         {
         case Linkage::None:   return out << STR_None;
         case Linkage::ExtACS: return out << STR_ExtACS;
         case Linkage::ExtASM: return out << STR_ExtASM;
         case Linkage::ExtC:   return out << STR_ExtC;
         case Linkage::ExtCXX: return out << STR_ExtCXX;
         case Linkage::ExtDS:  return out << STR_ExtDS;
         case Linkage::IntC:   return out << STR_IntC;
         case Linkage::IntCXX: return out << STR_IntCXX;
         }
      }
   }
}

// EOF

