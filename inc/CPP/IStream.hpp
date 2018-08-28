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
         std::istream{&ebuf},
         lbuf{buf},
         obuf{lbuf, {file, line, 1}},
         tbuf{obuf},
         ebuf{tbuf}
      {
      }

      Core::OriginSource &getOriginSource() {return obuf;}

   protected:
      using LBuf = Core::LineTermBuf<8>;
      using OBuf = Core::OriginBuf<8, 2>;
      using TBuf = TrigraphBuf<8>;
      using EBuf = Core::StripEscapeBuf<8, 1, 1, char, '\n'>;

      LBuf lbuf;
      OBuf obuf;
      TBuf tbuf;
      EBuf ebuf;
   };
}

#endif//GDCC__CPP__IStream_H__

