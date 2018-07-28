//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
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

      Core::Error({}, "invalid enum GDCC::IR::AddrBase");
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

      Core::Error({}, "invalid enum GDCC::IR::AddrBase");
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
         Core::Error({}, "invalid AddrBase");
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

