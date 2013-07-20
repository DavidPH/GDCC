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

#include "IArchive.hpp"

#include "../String.hpp"

#include <iostream>


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

      //
      // operator IArchive >> Linkage
      //
      IArchive &operator >> (IArchive &in, Linkage &out)
      {
         switch(GetIR<StringIndex>(in))
         {
         case STR_None:   out = Linkage::None;   return in;
         case STR_ExtACS: out = Linkage::ExtACS; return in;
         case STR_ExtASM: out = Linkage::ExtASM; return in;
         case STR_ExtC:   out = Linkage::ExtC;   return in;
         case STR_ExtCXX: out = Linkage::ExtCXX; return in;
         case STR_ExtDS:  out = Linkage::ExtDS;  return in;
         case STR_IntC:   out = Linkage::IntC;   return in;
         case STR_IntCXX: out = Linkage::IntCXX; return in;

         default:
            std::cerr << "invalid Linkage\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

