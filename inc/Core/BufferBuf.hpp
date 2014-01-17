//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Buffering std::streambuf.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__BufferBuf_H__
#define GDCC__Core__BufferBuf_H__

#include <streambuf>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // IBufferBuf
      //
      // A statically sized buffer that maintains a minimum number of past
      // characters. Because reading ahead may have side effects, the default
      // max read ahead is 1.
      //
      template<
         std::size_t BufSize = 1,
         std::size_t BufBack = 1,
         std::size_t BufRead = 1,
         typename    CharT   = char,
         typename    Traits  = std::char_traits<CharT>>

      class IBufferBuf : public std::basic_streambuf<CharT, Traits>
      {
      public:
         using Super = std::basic_streambuf<CharT, Traits>;
         using Src   = std::basic_streambuf<CharT, Traits>;

         using typename Super::int_type;


         explicit IBufferBuf(Src &src_) : src(src_) {setg(buf, buf, buf);}

      protected:
         using Super::eback;
         using Super::egptr;
         using Super::gptr;
         using Super::setg;

         // bufAvail
         std::size_t bufAvail() {return egptr() - gptr();}

         // bufEnd
         CharT *bufEnd() {return buf + BufSize;}

         // bufLive
         std::size_t bufLive() {return bufAvail() + BufBack;}

         // bufSpace
         std::size_t bufSpace() {return bufEnd() - egptr();}

         //
         // pbackfail
         //
         virtual int_type pbackfail(int_type c)
         {
            // If no back buffer space, fail.
            if(gptr() == eback())
               return Traits::eof();

            // Back the get pointer up one and put the new character there.
            setg(eback(), gptr() - 1, egptr());
            *gptr() = static_cast<CharT>(c);

            // Succeed.
            return c;
         }

         //
         // underflow
         //
         virtual int_type underflow()
         {
            std::size_t space;
            CharT      *itr;

            // If no buffer space left try to shift to fit more characters.
            if(!bufSpace() && (space = bufLive()) < BufSize)
            {
               itr = buf;
               for(auto chr = gptr() - BufBack, end = itr + space; itr != end;)
                  *itr++ = *chr++;

               setg(buf, itr, itr);
            }
            else
               itr = egptr();

            // Read new characters.
            space = std::min(bufSpace(), BufRead);
            while(space-- && src.sgetc() != Traits::eof())
               *itr++ = src.sbumpc();

            setg(buf, gptr(), itr);

            // Return the next character, if any.
            return gptr() == egptr() ? Traits::eof() : *gptr();
         }

         Src  &src;
         CharT buf[BufSize];
      };
   }
}

#endif//GDCC__Core__BufferBuf_H__

