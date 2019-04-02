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

#ifndef GDCC__Target__Addr_H__
#define GDCC__Target__Addr_H__

#include "../Target/Types.hpp"

#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Target
{
   //
   // AddrBase
   //
   enum class AddrBase
   {
      #define GDCC_Target_AddrList(name) name,
      #include "../Target/AddrList.hpp"
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
   // hash<::GDCC::Target::AddrBase>
   //
   template<> struct hash<::GDCC::Target::AddrBase>
   {
      size_t operator () (::GDCC::Target::AddrBase base) const
         {return static_cast<size_t>(base);}
   };

   //
   // hash<::GDCC::Target::AddrSpace>
   //
   template<> struct hash<::GDCC::Target::AddrSpace>
   {
      size_t operator () (::GDCC::Target::AddrSpace space) const
         {return space.name.getHash() ^ static_cast<size_t>(space.base);}
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Target
{
   std::ostream &operator << (std::ostream &out, AddrBase in);

   AddrSpace GetAddrGen();

   bool IsAddrEnclosed(AddrSpace encloser, AddrSpace enclosee);
}

#endif//GDCC__Target__Addr_H__

