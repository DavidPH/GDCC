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

#include "Path.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   //
   // PathConcat
   //
   String PathConcat(char const *l, String r)
   {
      auto const &rd = r.getData();
      std::string tmp{l};

      if(tmp.empty()) return r;

      if(!IsPathSep(tmp.back()))
         tmp += PathSep(l);

      if(!rd.empty())
      {
         if(IsPathSep(rd.front()))
            tmp.append(rd.str + 1, rd.len - 1);
         else
            tmp.append(rd.str, rd.len);
      }

      auto hash = HashString(tmp.data(), tmp.size());
      return AddString(tmp.data(), tmp.size(), hash);
   }

   //
   // PathConcat
   //
   String PathConcat(String l, String r)
   {
      auto const &ld = l.getData(), &rd = r.getData();

      if(ld.empty()) return r;
      if(rd.empty()) return l;

      if(IsPathSep(ld.back()))
      {
         if(!IsPathSep(rd.front()))
            return l + r;

         std::string tmp{ld.str, ld.len - 1};
         tmp.append(rd.str, rd.len);

         auto hash = HashString(tmp.data(), tmp.size());
         return AddString(tmp.data(), tmp.size(), hash);
      }
      else
      {
         if(IsPathSep(rd.front()))
            return l + r;

         std::string tmp{ld.str, ld.len};
         tmp += PathSep(ld.str);
         tmp.append(rd.str, rd.len);

         auto hash = HashString(tmp.data(), tmp.size());
         return AddString(tmp.data(), tmp.size(), hash);
      }
   }

   //
   // PathDirname
   //
   String PathDirname(String path)
   {
      auto const &data = path.getData();

      char const *itr = data.end();

      for(;;)
      {
         if(itr == data.str) return STR_;
         if(IsPathSep(*--itr)) break;
      }

      std::size_t len = itr - data.str;
      auto hash = HashString(data.str, len);
      return AddString(data.str, len, hash);
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
         auto const &pathData = path.getData();

         std::unique_ptr<char[]> tmp{new char[pathData.len + 1]};
         std::memcpy(tmp.get(), pathData.str, pathData.len + 1);
         PathNormalize(tmp.get());

         auto hash = HashString(tmp.get(), pathData.len);
         return AddString(tmp.get(), pathData.len, hash);
      }
      #endif

      return path;
   }
}

// EOF

