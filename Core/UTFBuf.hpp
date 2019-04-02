//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// UTF conversion streambufs.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__UTFBuf_H__
#define GDCC__Core__UTFBuf_H__

#include "../Core/BufferBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // UTF8to32Buf
   //
   template<
      typename TraitsO = std::char_traits<char32_t>,
      typename TraitsI = std::char_traits<char>>
   class UTF8to32Buf : public std::basic_streambuf<char32_t, TraitsO>
   {
   public:
      using Super = std::basic_streambuf<char32_t, TraitsO>;
      using Src   = std::basic_streambuf<char>;

      using typename Super::int_type;


      explicit UTF8to32Buf(Src &src_) : src{src_} {setg(buf, buf, buf);}

   protected:
      using Super::setg;

      //
      // underflow
      //
      virtual int_type underflow()
      {
         char32_t      o;
         Src::int_type i = src.sbumpc();

         if(i == TraitsI::eof())
            return setg(buf, buf, buf), TraitsO::eof();

              if(!(i & 0x80)) o = i & 0x7F;
         else if(!(i & 0x40)) o = i & 0x3F; // Not valid.
         else if(!(i & 0x20)) o = i & 0x1F;
         else if(!(i & 0x10)) o = i & 0x0F;
         else if(!(i & 0x08)) o = i & 0x07;
         else if(!(i & 0x04)) o = i & 0x03;
         else if(!(i & 0x02)) o = i & 0x01;
         else if(!(i & 0x01)) o = i & 0x00; // Not valid.
         else                 o = i & 0x00; // Not valid.

         while((i = src.sgetc()) != TraitsI::eof() && (i & 0xC0) == 0x80)
            o = (o << 6) | (i & 0x3F), src.sbumpc();

         setg(buf, buf, buf + 1);
         return buf[0] = o;
      }

      Src     &src;
      char32_t buf[1];
   };

   //
   // UTF32to8Buf
   //
   template<
      typename TraitsO = std::char_traits<char>,
      typename TraitsI = std::char_traits<char32_t>>
   class UTF32to8Buf : public std::basic_streambuf<char, TraitsO>
   {
   public:
      using Super = std::basic_streambuf<char, TraitsO>;
      using Src   = std::basic_streambuf<char32_t>;

      using typename Super::int_type;


      explicit UTF32to8Buf(Src &src_) : src{src_} {setg(buf, buf, buf);}

   protected:
      using Super::setg;

      //
      // underflow
      //
      virtual int_type underflow()
      {
         char         *o = buf;
         Src::int_type i = src.sbumpc();

         if(i == TraitsI::eof())
            return setg(buf, buf, buf), TraitsO::eof();


         if(i <= 0x7F)       {*o++ = 0x00 | ((i >>  0) & 0x7F); goto put0;}
         if(i <= 0x7FF)      {*o++ = 0xC0 | ((i >>  6) & 0x1F); goto put1;}
         if(i <= 0xFFFF)     {*o++ = 0xE0 | ((i >> 12) & 0x0F); goto put2;}
         if(i <= 0x1FFFFF)   {*o++ = 0xF0 | ((i >> 18) & 0x07); goto put3;}
         if(i <= 0x3FFFFFF)  {*o++ = 0xF8 | ((i >> 24) & 0x03); goto put4;}
         if(i <= 0x7FFFFFFF) {*o++ = 0xFC | ((i >> 30) & 0x01); goto put5;}

         *o++ = ('\xFE');

               *o++ = 0x80 | ((i >> 30) & 0x3F);
         put5: *o++ = 0x80 | ((i >> 24) & 0x3F);
         put4: *o++ = 0x80 | ((i >> 18) & 0x3F);
         put3: *o++ = 0x80 | ((i >> 12) & 0x3F);
         put2: *o++ = 0x80 | ((i >>  6) & 0x3F);
         put1: *o++ = 0x80 | ((i >>  0) & 0x3F);
         put0: ;

         setg(buf, buf, o);
         return buf[0];
      }

      Src &src;
      char buf[7];
   };
}

#endif//GDCC__Core__UTFBuf_H__

