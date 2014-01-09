//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing utilities.
//
//-----------------------------------------------------------------------------

#include "Option/Util.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      //
      // StrCmp
      //
      int StrCmp(char const *l, char const *r)
      {
         if(l) return r ? std::strcmp(l, r) : 1;
         else  return r ? -1 : 0;
      }

      //
      // StrDup
      //
      std::unique_ptr<char[]> StrDup(char const *s)
      {
         return StrDup(s, std::strlen(s));
      }

      //
      // StrDup
      //
      std::unique_ptr<char[]> StrDup(char const *s, std::size_t len)
      {
         std::unique_ptr<char[]> dup{new char[len + 1]};
         std::memcpy(dup.get(), s, len);
         dup[len] = '\0';

         return dup;
      }

      //
      // StrHash
      //
      std::size_t StrHash(char const *s)
      {
         std::size_t hash = 0;
         if(s) for(; *s; ++s)
            hash = hash * 5 + *s;
         return hash;
      }
   }
}

// EOF

