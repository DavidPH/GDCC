//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Iterator ranges.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Range_H__
#define GDCC__Core__Range_H__

#include "../Core/Types.hpp"

#include <iterator>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // Range
   //
   template<typename T>
   class Range
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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
      //
   // MakeRange
   //
   template<typename T>
   Range<T> MakeRange(T first, T last)
   {
      return Range<T>(first, last);
   }
}

#endif//GDCC__Core__Range_H__

