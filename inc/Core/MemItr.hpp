//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include <iterator>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // MemItr
   //
   // The template defaults make it suitable for wrapping STL pair iterators.
   //
   template<typename Itr,
      typename MT = typename std::iterator_traits<Itr>::value_type::second_type,
      MT std::iterator_traits<Itr>::value_type::*P =
         &std::iterator_traits<Itr>::value_type::second>
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

