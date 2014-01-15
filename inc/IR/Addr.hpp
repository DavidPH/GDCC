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

#ifndef GDCC__IR__Addr_H__
#define GDCC__IR__Addr_H__

#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class IArchive;
      class OArchive;

      //
      // AddrBase
      //
      enum class AddrBase
      {
         #define GDCC_IR_AddrList(name) name,
         #include "../IR/AddrList.hpp"
      };

      //
      // AddrSpace
      //
      class AddrSpace
      {
      public:
         AddrSpace() = default;
         constexpr AddrSpace(AddrBase base_) :
            base{base_}, name{Core::STRNULL} {}
         constexpr AddrSpace(AddrBase base_, Core::String name_) :
            base{base_}, name{name_} {}

         constexpr bool operator == (AddrSpace const &as) const
            {return base == as.base && name == as.name;}
         constexpr bool operator != (AddrSpace const &as) const
            {return base != as.base || name != as.name;}

         AddrSpace &operator = (AddrBase base_)
            {base = base_; name = Core::STRNULL; return *this;}

         AddrBase     base;
         Core::String name;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, AddrBase  in);
      OArchive &operator << (OArchive &out, AddrSpace in);
      std::ostream &operator << (std::ostream &out, AddrBase in);

      IArchive &operator >> (IArchive &in, AddrBase  &out);
      IArchive &operator >> (IArchive &in, AddrSpace &out);
   }
}

#endif//GDCC__IR__Addr_H__

