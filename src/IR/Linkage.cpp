//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
#include "IR/OArchive.hpp"

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
         case Linkage::ExtAXX: return out << Core::STR_ExtAXX;
         case Linkage::ExtC:   return out << Core::STR_ExtC;
         case Linkage::ExtCXX: return out << Core::STR_ExtCXX;
         case Linkage::ExtDS:  return out << Core::STR_ExtDS;
         case Linkage::IntC:   return out << Core::STR_IntC;
         case Linkage::IntCXX: return out << Core::STR_IntCXX;
         }

         std::cerr << "invalid enum GDCC::IR::Linkage\n";
         throw EXIT_FAILURE;
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
         case Linkage::ExtAXX: return out << "ExtAXX";
         case Linkage::ExtC:   return out << "ExtC";
         case Linkage::ExtCXX: return out << "ExtCXX";
         case Linkage::ExtDS:  return out << "ExtDS";
         case Linkage::IntC:   return out << "IntC";
         case Linkage::IntCXX: return out << "IntCXX";
         }

         std::cerr << "invalid enum GDCC::IR::Linkage\n";
         throw EXIT_FAILURE;
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
         case Core::STR_ExtAXX: out = Linkage::ExtAXX; return in;
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

      //
      // GetLinkageExt
      //
      Linkage GetLinkageExt(Linkage linka)
      {
         switch(linka)
         {
         case Linkage::IntC:   return Linkage::ExtC;
         case Linkage::IntCXX: return Linkage::ExtCXX;

         default: return linka;
         }
      }

      //
      // GetLinkageInt
      //
      Linkage GetLinkageInt(Linkage linka)
      {
         switch(linka)
         {
         case Linkage::ExtACS: return Linkage::IntC;
         case Linkage::ExtASM: return Linkage::IntC;
         case Linkage::ExtAXX: return Linkage::IntCXX;
         case Linkage::ExtC:   return Linkage::IntC;
         case Linkage::ExtCXX: return Linkage::IntCXX;
         case Linkage::ExtDS:  return Linkage::IntCXX;

         default: return linka;
         }
      }
   }
}

// EOF

