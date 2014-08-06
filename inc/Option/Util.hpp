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
      int StrCmp(char const *l, char const *r);

      std::unique_ptr<char[]> StrDup(char const *s);
      std::unique_ptr<char[]> StrDup(char const *s, std::size_t len);

      std::size_t StrHash(char const *str);
      std::size_t StrHash(char const *str, std::size_t len);
      std::size_t StrHash(char const *str, std::size_t len, std::size_t hash);

      std::pair<std::size_t, std::size_t> StrLenHash(char const *str);
   }
}

#endif//GDCC__Option__Util_H__

