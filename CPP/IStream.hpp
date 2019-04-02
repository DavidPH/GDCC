//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C input stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__IStream_H__
#define GDCC__CPP__IStream_H__

#include "../CPP/TrigraphBuf.hpp"

#include "../Core/CommentBuf.hpp"
#include "../Core/EscapeBuf.hpp"
#include "../Core/FeatureHold.hpp"
#include "../Core/LineTermBuf.hpp"
#include "../Core/OriginBuf.hpp"
#include "../Core/UTFBuf.hpp"

#include <istream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
{
   //
   // IStream
   //
   class IStream : public std::istream
   {
   public:
      IStream(std::streambuf &buf, Core::String file, std::size_t line = 1) :
         std::istream{&bbuf},
         wbuf{buf},
         lbuf{wbuf},
         obuf{lbuf, {file, line, 1}},
         tbuf{obuf},
         ebuf{tbuf},
         nbuf{ebuf},
         bbuf{nbuf}
      {
      }

      Core::OriginSource &getOriginSource() {return obuf;}

   protected:
      using WBuf = Core::UTF8to32Buf<>;
      using LBuf = Core::LineTermBuf<8, 1, 1, char32_t>;
      using OBuf = Core::OriginBuf<8, 2, 1, char32_t>;
      using TBuf = TrigraphBuf<8, 1, 1, char32_t>;
      using EBuf = Core::StripEscapeBuf<8, 1, 1, char32_t, '\n'>;
      using NBuf = Core::UTF32to8Buf<>;
      using BBuf = Core::IBufferBuf<8, 2, 1, char>;

      WBuf wbuf;
      LBuf lbuf;
      OBuf obuf;
      TBuf tbuf;
      EBuf ebuf;
      NBuf nbuf;
      BBuf bbuf;
   };
}

#endif//GDCC__CPP__IStream_H__

