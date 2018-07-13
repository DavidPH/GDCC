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

#ifndef GDCC__IR__Addr_H__
#define GDCC__IR__Addr_H__

#include "../IR/Types.hpp"

#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
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
         base{base_}, name{Core::STR_} {}
      constexpr AddrSpace(AddrBase base_, Core::String name_) :
         base{base_}, name{name_} {}

      constexpr bool operator == (AddrSpace const &as) const
         {return base == as.base && name == as.name;}
      constexpr bool operator != (AddrSpace const &as) const
         {return base != as.base || name != as.name;}

      AddrSpace &operator = (AddrBase base_)
         {base = base_; name = Core::STR_; return *this;}

      AddrBase     base;
      Core::String name;
   };
}

namespace std
{
   //
   // hash<::GDCC::IR::AddrBase>
   //
   template<> struct hash<::GDCC::IR::AddrBase>
   {
      size_t operator () (::GDCC::IR::AddrBase base) const
         {return static_cast<size_t>(base);}
   };

   //
   // hash<::GDCC::IR::AddrSpace>
   //
   template<> struct hash<::GDCC::IR::AddrSpace>
   {
      size_t operator () (::GDCC::IR::AddrSpace space) const
         {return space.name.getHash() ^ static_cast<size_t>(space.base);}
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, AddrBase  in);
   OArchive &operator << (OArchive &out, AddrSpace in);
   std::ostream &operator << (std::ostream &out, AddrBase in);

   IArchive &operator >> (IArchive &in, AddrBase  &out);
   IArchive &operator >> (IArchive &in, AddrSpace &out);

   AddrSpace GetAddrGen();

   bool IsAddrEnclosed(AddrSpace encloser, AddrSpace enclosee);
}

#endif//GDCC__IR__Addr_H__

