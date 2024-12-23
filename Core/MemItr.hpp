//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Member-accessing iterators.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__MemItr_H__
#define GDCC__Core__MemItr_H__

#include "../Core/Types.hpp"

#include <iterator>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   template<typename Itr>
   using MemItrVT = typename std::iterator_traits<Itr>::value_type;

   //
   // MemItr
   //
   // The template defaults make it suitable for wrapping STL pair iterators.
   //
   template<typename Itr,
      typename MT = typename MemItrVT<Itr>::second_type,
      MT MemItrVT<Itr>::*P = &MemItrVT<Itr>::second>
   class MemItr : public Itr
   {
   public:
      using value_type = MT;
      using pointer    = MT *;
      using reference  = MT &;

      using Itr::Itr;

      MemItr() = default;
      MemItr(Itr const &i) : Itr{i} {}
      MemItr(Itr &&i) : Itr{std::move(i)} {}

      constexpr MT &operator * () const {return Itr::operator*().*P;}

      constexpr MT *operator -> () const {return &Itr::operator->()->*P;}
   };
}

#endif//GDCC__Core__MemItr_H__

