//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Miscellaneous utility functions and classes.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Utility_H__
#define GDCC__Utility_H__

#include <functional>
#include <iterator>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // LessMem
   //
   template<typename T, typename KT, KT T::*P, typename Cmp = std::less<KT>>
   struct LessMem : private Cmp
   {
      using Cmp::Cmp;

      constexpr bool operator () (T const &l, T const &r) const
         {return Cmp::operator()(l.*P, r.*P);}
      constexpr bool operator () (T const *l, T const *r) const
         {return Cmp::operator()(l->*P, r->*P);}
   };

   //
   // MemItr
   //
   // The template defaults make it suitable for wrapping STL pair iterators.
   //
   template<typename Itr,
      typename MT = typename std::iterator_traits<Itr>::value_type::second_type,
      MT std::iterator_traits<Itr>::value_type::*P =
         &std::iterator_traits<Itr>::value_type::second>
   struct MemItr : Itr
   {
      using Itr::Itr;

      MemItr() = default;
      MemItr(Itr const &i) : Itr{i} {}
      MemItr(Itr &&i) : Itr{std::move(i)} {}

      constexpr MT &operator * () const {return Itr::operator*().*P;}

      constexpr MT *operator -> () const {return &Itr::operator->()->*P;}
   };

   //
   // Range
   //
   template<typename T> class Range
   {
   public:
      Range() = default;
      constexpr Range(T first_, T last_) : first{first_}, last{last_} {}

      T begin() const {return first;}

      bool empty() const {return first == last;}

      T end() const {return last;}

      std::size_t size() const {return std::distance(first, last);}

      T first, last;
   };
}

namespace std
{
   //
   // iterator_traits<::GDCC::MemItr>
   //
   template<typename Itr, typename MT, MT iterator_traits<Itr>::value_type::*P>
   struct iterator_traits<::GDCC::MemItr<Itr, MT, P>>
   {
      using difference_type   = typename iterator_traits<Itr>::difference_type;
      using value_type        = MT;
      using pointer           = MT *;
      using reference         = MT &;
      using iterator_category = typename iterator_traits<Itr>::iterator_category;
   };
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   template<typename T>
   Range<T> MakeRange(T first, T last) {return Range<T>(first, last);}
}

#endif//GDCC__Utility_H__

