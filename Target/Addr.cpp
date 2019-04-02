//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Address space handling.
//
//-----------------------------------------------------------------------------

#include "Target/Addr.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Target
{
   //
   // operator std::ostream << AddrBase
   //
   std::ostream &operator << (std::ostream &out, AddrBase in)
   {
      switch(in)
      {
         #define GDCC_Target_AddrList(name) \
            case AddrBase::name: return out << #name;
         #include "Target/AddrList.hpp"
      }

      Core::Error({}, "invalid enum GDCC::Target::AddrBase");
   }

   //
   // GetAddrGen
   //
   AddrSpace GetAddrGen()
   {
      return AddrBase::Sta;
   }

   //
   // IsAddrEnclosed
   //
   bool IsAddrEnclosed(AddrSpace encloser, AddrSpace enclosee)
   {
      // Substitute implementation for generic address space.
      if(encloser.base == AddrBase::Gen) encloser = GetAddrGen();
      if(enclosee.base == AddrBase::Gen) enclosee = GetAddrGen();

      // All address spaces enclose themselves.
      if(encloser == enclosee) return true;

      // Sta always encloses Aut.
      if(encloser.base == AddrBase::Sta && enclosee.base == AddrBase::Aut) return true;

      // Array spaces.
      if(encloser.base == AddrBase::GblArs && enclosee.base == AddrBase::GblArr) return true;
      if(encloser.base == AddrBase::HubArs && enclosee.base == AddrBase::HubArr) return true;
      if(encloser.base == AddrBase::ModArs && enclosee.base == AddrBase::ModArr) return true;
      if(encloser.base == AddrBase::StrArs && enclosee.base == AddrBase::StrArr) return true;

      // Far pointer.
      if(encloser.base == AddrBase::Far) switch(enclosee.base)
      {
      case AddrBase::Aut:    case AddrBase::Sta:
      case AddrBase::GblArr: case AddrBase::GblArs: case AddrBase::GblReg:
      //case AddrBase::ModArr: case AddrBase::ModArs: case AddrBase::ModReg:
      case AddrBase::StrArr: case AddrBase::StrArs:
      case AddrBase::HubArr: case AddrBase::HubArs: case AddrBase::HubReg:
         return true;

      default: break;
      }

      return false;
   }
}

// EOF

