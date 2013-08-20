//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C string buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__StringBuf_H__
#define GDCC__StringBuf_H__

#include "String.hpp"

#include <istream>
#include <streambuf>

//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
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
         auto const &dat = src.getData();
         char *str = const_cast<char *>(dat.str);
         setg(str, str, str + dat.len);
      }
   };

   //
   // StringStream
   //
   class StringStream : public std::istream
   {
   public:
      explicit StringStream(String src) : std::istream{&buf}, buf{src} {}

   protected:
      StringBuf buf;
   };
}

#endif//GDCC__StringBuf_H__

