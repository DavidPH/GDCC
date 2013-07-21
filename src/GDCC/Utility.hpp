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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // Range
   //
   template<typename T> class Range
   {
   public:
      Range() = default;
      constexpr Range(T first_, T last_) : first{first_}, last{last_} {}

      T begin() const {return first;}
      T end() const {return last;}

   private:
      T first, last;
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

