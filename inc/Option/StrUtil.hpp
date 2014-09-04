//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Util_H__
#define GDCC__Option__Util_H__

#include <memory>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      /// Compares two strings.
      int StrCmp(char const *l, char const *r);

      /// Duplicates a null-terminated string.
      std::unique_ptr<char[]> StrDup(char const *s);

      /// Duplicates a string.
      std::unique_ptr<char[]> StrDup(char const *s, std::size_t len);

      /// Computes a null-terminated string's hash.
      std::size_t StrHash(char const *str);

      /// Computes a string's hash.
      std::size_t StrHash(char const *str, std::size_t len);

      /// Computes a string's hash.
      std::size_t StrHash(char const *str, std::size_t len, std::size_t hash);

      /// Computes a null-terminated string's length and hash.
      std::pair<std::size_t, std::size_t> StrLenHash(char const *str);
   }
}

#endif//GDCC__Option__Util_H__

