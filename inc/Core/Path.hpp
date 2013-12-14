//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Path string manipulation utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Path_H__
#define GDCC__Core__Path_H__

#include "String.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      constexpr bool IsPathSep(char c);

      String PathConcat(char const *l, String r);
      String PathConcat(String      l, String r);

      String PathDirname(String path);

      char  *PathNormalize(char  *path);
      String PathNormalize(String path);

      char PathSep(char const *path);
      char PathSep(String      path);
   }
}

namespace GDCC
{
   namespace Core
   {
      //
      // IsPathSep
      //
      constexpr bool IsPathSep(char c)
      {
         #ifdef _WIN32
         return c == '/' || c == '\\';
         #else
         return c == '/';
         #endif
      }

      //
      // PathSep
      //
      inline char PathSep(char const *path)
      {
         #ifdef _WIN32
         for(; *path; ++path) if(IsPathSep(*path)) return *path;
         #else
         (void)path;
         #endif

         return '/';
      }

      //
      // PathSep
      //
      inline char PathSep(String path)
      {
         return PathSep(path.data());
      }
   }
}

#endif//GDCC__Core__Path_H__

