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

         std::cerr << "invalid enum GDCC::IR::AddrBase\n";
         throw EXIT_FAILURE;
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

         std::cerr << "invalid enum GDCC::IR::AddrBase\n";
         throw EXIT_FAILURE;
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

      //
      // GetAddrGen
      //
      AddrSpace GetAddrGen()
      {
         return AddrBase::LocArs;
      }

      //
      // IsAddrEnclosed
      //
      bool IsAddrEnclosed(AddrSpace encloser, AddrSpace enclosee)
      {
         #define GDCC_IR_AddrList(name) using AddrBase::name;
         #include "IR/AddrList.hpp"

         // Substitute implementation for generic address space.
         if(encloser.base == Gen) encloser = GetAddrGen();
         if(enclosee.base == Gen) enclosee = GetAddrGen();

         // All address spaces enclose themselves.
         if(encloser == enclosee) return true;

         // LocArs always encloses Loc.
         if(encloser.base == LocArs && enclosee.base == Loc) return true;

         // Array spaces.
         if(encloser.base == GblArs && enclosee.base == GblArr) return true;
         if(encloser.base == MapArs && enclosee.base == MapArr) return true;
         if(encloser.base == StrArs && enclosee.base == StrArr) return true;
         if(encloser.base == WldArs && enclosee.base == WldArr) return true;

         // Far pointer.
         if(encloser.base == Far) switch(enclosee.base)
         {
         case GblArr: case GblArs: case GblReg:
         case Loc:    case LocArs:
       //case MapArr: case MapArs: case MapReg:
         case StrArr: case StrArs:
         case WldArr: case WldArs: case WldReg:
            return true;

         default: break;
         }

         return false;
      }
   }
}

// EOF

