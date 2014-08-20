//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

namespace GDCC
{
   namespace Core
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
         using Itr::Itr;

         MemItr() = default;
         MemItr(Itr const &i) : Itr{i} {}
         MemItr(Itr &&i) : Itr{std::move(i)} {}

         constexpr MT &operator * () const {return Itr::operator*().*P;}

         constexpr MT *operator -> () const {return &Itr::operator->()->*P;}
      };
   }
}

namespace std
{
   //
   // iterator_traits<::GDCC::Core::MemItr>
   //
   template<typename Itr, typename MT, MT iterator_traits<Itr>::value_type::*P>
   struct iterator_traits<::GDCC::Core::MemItr<Itr, MT, P>>
   {
      using difference_type   = typename iterator_traits<Itr>::difference_type;
      using value_type        = MT;
      using pointer           = MT *;
      using reference         = MT &;
      using iterator_category = typename iterator_traits<Itr>::iterator_category;
   };
}

#endif//GDCC__Core__MemItr_H__

