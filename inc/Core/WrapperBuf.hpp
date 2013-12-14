//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Streambuf-wrapping streambuf.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__WrapperBuf_H__
#define GDCC__Core__WrapperBuf_H__

#include <streambuf>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // WrapperBuf
      //
      template<typename Src = std::streambuf, std::size_t BufSize = 4,
         std::size_t BufBase = BufSize - 1>
      class WrapperBuf : public std::streambuf
      {
      public:
         explicit WrapperBuf(Src &src_) : src(src_), buf{'\0'}
            {setg(buf, buf + BufSize, buf + BufSize);}

      protected:
         //
         // pbackfail
         //
         virtual int pbackfail(int c)
         {
            if(c == EOF || gptr() == eback()) return EOF;

            *(gptr() - 1) = static_cast<char>(c);
            setg(eback(), gptr() - 1, egptr());

            return c;
         }

         //
         // underflow
         //
         virtual int underflow()
         {
            // Full buffer refill.
            if(gptr() == egptr())
            {
               // Push buffer contents into the back for possible ungets.
               for(std::size_t i = 0; i != BufBase; ++i)
                  buf[i] = buf[i + (BufSize - BufBase)];

               // Fill the buffer.
               for(char *itr = buf + BufBase, *end = buf + BufSize; itr != end; ++itr)
               {
                  if(src.sgetc() == EOF) {setg(buf, buf + BufBase, itr); goto flowed;}
                  *itr = src.sbumpc();
               }

               setg(buf, buf + BufBase, buf + BufSize);
            }

            // Return the next character, if any.
         flowed:
            return gptr() == egptr() ? EOF : *gptr();
         }

         Src &src;
         char buf[BufSize];
      };
   }
}

#endif//GDCC__Core__WrapperBuf_H__

