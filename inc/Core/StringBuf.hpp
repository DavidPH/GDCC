//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C string buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__StringBuf_H__
#define GDCC__Core__StringBuf_H__

#include "../Core/String.hpp"

#include <istream>
#include <streambuf>

//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // StringBuf
   //
   class StringBuf final : public std::streambuf
   {
   public:
      //
      // constructor
      //
      explicit StringBuf(String src)
      {
         char *str = const_cast<char *>(src.data());
         setg(str, str, str + src.size());
      }

      //
      // constructor
      //
      StringBuf(char const *str_, std::size_t len)
      {
         char *str = const_cast<char *>(str_);
         setg(str, str, str + len);
      }
   };

   //
   // StringStream
   //
   class StringStream : public std::istream
   {
   public:
      explicit StringStream(String src) : std::istream{&buf}, buf{src} {}
      StringStream(char const *str, std::size_t len) :
         std::istream{&buf}, buf{str, len} {}

   protected:
      StringBuf buf;
   };
}

#endif//GDCC__Core__StringBuf_H__

