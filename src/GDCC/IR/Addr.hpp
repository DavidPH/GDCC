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

#ifndef GDCC__IR__Addr_H__
#define GDCC__IR__Addr_H__

#include "../String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // AddrBase
      //
      enum class AddrBase
      {
         #define GDCC_IR_AddrList(name) name,
         #include "AddrList.hpp"
      };

      //
      // AddressSpace
      //
      class AddressSpace
      {
      public:
         AddressSpace() = default;
         constexpr AddressSpace(AddrBase base_, String name_) : base{base_}, name{name_} {}

         AddrBase base;
         String   name;
      };
   }
}

#endif//GDCC__IR__Addr_H__

