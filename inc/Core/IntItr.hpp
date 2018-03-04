//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Integer-counting iterators.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__IntItr_H__
#define GDCC__Core__IntItr_H__

#include <iterator>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // IntItr
   //
   template<typename I>
   class IntItr
   {
   public:
      using difference_type   = I;
      using value_type        = I;
      using pointer           = IntItr<I>;
      using reference         = I;
      using iterator_category = std::random_access_iterator_tag;


      IntItr() = default;
      IntItr(I const &i_) : i{i_} {}
      IntItr(I &&i_) : i{std::move(i_)} {}

      IntItr &operator ++ () {++i; return *this;}
      IntItr operator ++ (int) {return {i++};}
      IntItr &operator -- () {--i; return *this;}
      IntItr operator -- (int) {return {i--};}

      constexpr I operator [] (I const &r) const {return i + r;}

      constexpr IntItr operator + (I const &r) const {return {i + r};}
      constexpr IntItr operator - (I const &r) const {return {i - r};}
      constexpr I operator - (IntItr const &r) const {return i - r.i;}

      constexpr bool operator >= (IntItr const &r) const {return i >= r.i;}
      constexpr bool operator >  (IntItr const &r) const {return i >  r.i;}
      constexpr bool operator <= (IntItr const &r) const {return i <= r.i;}
      constexpr bool operator <  (IntItr const &r) const {return i <  r.i;}

      constexpr bool operator == (IntItr const &r) const {return i == r.i;}
      constexpr bool operator != (IntItr const &r) const {return i != r.i;}

      IntItr &operator += (I const &r) {i += r; return *this;}
      IntItr &operator -= (I const &r) {i -= r; return *this;}

      I const &operator * () const {return i;}

   private:
      I i;
   };
}

#endif//GDCC__Core__IntItr_H__

