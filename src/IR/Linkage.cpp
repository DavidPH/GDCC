//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // operator OArchive << Linkage
   //
   OArchive &operator << (OArchive &out, Linkage in)
   {
      switch(in)
      {
      #define GDCC_IR_LinkageList(name) \
         case Linkage::name: return out << Core::STR_##name;
      #include "IR/LinkageList.hpp"
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
      #define GDCC_IR_LinkageList(name) \
         case Linkage::name: return out << #name;
      #include "IR/LinkageList.hpp"
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
      #define GDCC_IR_LinkageList(name) \
         case Core::STR_##name: out = Linkage::name; return in;
      #include "IR/LinkageList.hpp"

      default:
         std::cerr << "invalid GDCC::IR::Linkage\n";
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

// EOF

