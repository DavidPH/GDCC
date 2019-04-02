//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // PathRestore
   //
   class PathRestore
   {
   public:
      PathRestore(std::string &path_) : path{path_}, len{path.size()} {}
      ~PathRestore() {path.resize(len);}

   private:
      std::string           &path;
      std::string::size_type len;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   std::string const &GetSystemPath();

   constexpr bool IsPathSep(char c);

   std::string &PathAppend(std::string &l, String r);
   std::string &PathAppend(std::string &l, char const *r);

   String PathConcat(char const *l, String r);
   String PathConcat(String      l, String r);

   String PathDirname(String path);

   std::string &PathDirnameEq(std::string &path);

   char const *PathFilename(char const *path);

   String PathNormalize(String path);

   char        *PathNormalizeEq(char        *path);
   std::string &PathNormalizeEq(std::string &path);

   char PathSep(char const *path);
   char PathSep(String      path);

   std::string &PathTerminateEq(std::string &path);

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

#endif//GDCC__Core__Path_H__

