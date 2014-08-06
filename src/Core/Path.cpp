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

#include "Core/Path.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // PathConcat
      //
      String PathConcat(char const *l, String r)
      {
         std::string tmp{l};

         if(tmp.empty()) return r;

         if(!IsPathSep(tmp.back()))
            tmp += PathSep(l);

         if(!r.empty())
         {
            if(IsPathSep(r.front()))
               tmp.append(r.data() + 1, r.size() - 1);
            else
               tmp.append(r.data(), r.size());
         }

         return {tmp.data(), tmp.size()};
      }

      //
      // PathConcat
      //
      String PathConcat(String l, String r)
      {
         if(l.empty()) return r;
         if(r.empty()) return l;

         if(IsPathSep(l.back()))
         {
            if(!IsPathSep(r.front()))
               return l + r;

            std::string tmp{l.data(), l.size() - 1};
            tmp.append(r.data(), r.size());

            return {tmp.data(), tmp.size()};
         }
         else
         {
            if(IsPathSep(r.front()))
               return l + r;

            std::string tmp{l.data(), l.size()};
            tmp += PathSep(l.data());
            tmp.append(r.data(), r.size());

            return {tmp.data(), tmp.size()};
         }
      }

      //
      // PathDirname
      //
      String PathDirname(String path)
      {
         char const *itr = path.end();

         for(;;)
         {
            if(itr == path.data()) return STR_;
            if(IsPathSep(*--itr)) break;
         }

         std::size_t len = itr - path.data();
         return {path.data(), len};
      }

      //
      // PathNormalize
      //
      char *PathNormalize(char *path)
      {
         #ifdef _WIN32
         char const sep = PathSep(path);
         for(auto itr = path; *itr; ++itr)
            if(IsPathSep(*itr)) *itr = sep;
         #endif

         return path;
      }

      //
      // PathNormalize
      //
      String PathNormalize(String path)
      {
         #ifdef _WIN32
         char const sep = PathSep(path);
         for(char c : path) if(IsPathSep(c) && c != sep)
         {
            std::unique_ptr<char[]> tmp{new char[path.size() + 1]};
            std::memcpy(tmp.get(), path.data(), path.size() + 1);
            PathNormalize(tmp.get());

            return {tmp.get(), path.size()};
         }
         #endif

         return path;
      }
   }
}

// EOF

