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

#include "IR/Linkage.hpp"

#include "IR/IArchive.hpp"

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
      // operator OArchive << Linkage
      //
      OArchive &operator << (OArchive &out, Linkage in)
      {
         switch(in)
         {
         case Linkage::None:   return out << Core::STR_None;
         case Linkage::ExtACS: return out << Core::STR_ExtACS;
         case Linkage::ExtASM: return out << Core::STR_ExtASM;
         case Linkage::ExtC:   return out << Core::STR_ExtC;
         case Linkage::ExtCXX: return out << Core::STR_ExtCXX;
         case Linkage::ExtDS:  return out << Core::STR_ExtDS;
         case Linkage::IntC:   return out << Core::STR_IntC;
         case Linkage::IntCXX: return out << Core::STR_IntCXX;
         }
      }

      //
      // operator std::ostream << Linkage
      //
      std::ostream  &operator << (std::ostream &out, Linkage in)
      {
         switch(in)
         {
         case Linkage::None:   return out << "None";
         case Linkage::ExtACS: return out << "ExtACS";
         case Linkage::ExtASM: return out << "ExtASM";
         case Linkage::ExtC:   return out << "ExtC";
         case Linkage::ExtCXX: return out << "ExtCXX";
         case Linkage::ExtDS:  return out << "ExtDS";
         case Linkage::IntC:   return out << "IntC";
         case Linkage::IntCXX: return out << "IntCXX";
         }
      }

      //
      // operator IArchive >> Linkage
      //
      IArchive &operator >> (IArchive &in, Linkage &out)
      {
         switch(GetIR<Core::StringIndex>(in))
         {
         case Core::STR_None:   out = Linkage::None;   return in;
         case Core::STR_ExtACS: out = Linkage::ExtACS; return in;
         case Core::STR_ExtASM: out = Linkage::ExtASM; return in;
         case Core::STR_ExtC:   out = Linkage::ExtC;   return in;
         case Core::STR_ExtCXX: out = Linkage::ExtCXX; return in;
         case Core::STR_ExtDS:  out = Linkage::ExtDS;  return in;
         case Core::STR_IntC:   out = Linkage::IntC;   return in;
         case Core::STR_IntCXX: out = Linkage::IntCXX; return in;

         default:
            std::cerr << "invalid Linkage\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

