//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Reference counting.
//
//-----------------------------------------------------------------------------

#include "Core/Counter.hpp"

#include <stdexcept>

//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // CounterBase::NoClone
   //
   [[noreturn]]
   void CounterBase::NoClone(char const *msg)
   {
      // Primitive exception class.
      class NoClone_T : public std::exception
      {
      public:
         NoClone_T(char const *s) noexcept : msg{s} {}

         virtual char const *what() const noexcept {return msg;}

         char const *const msg;
      };

      throw NoClone_T(msg);
   }
}

// EOF

