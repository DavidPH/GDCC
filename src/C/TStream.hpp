//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C token stream.
//
//-----------------------------------------------------------------------------

#ifndef C__TStream_H__
#define C__TStream_H__

#include "ConcatTBuf.hpp"
#include "ConditionDTBuf.hpp"
#include "IStream.hpp"
#include "IncludeDTBuf.hpp"
#include "MacroDTBuf.hpp"
#include "MacroTBuf.hpp"
#include "Pragma.hpp"
#include "StringTBuf.hpp"

#include "GDCC/StreamTBuf.hpp"
#include "GDCC/TokenStream.hpp"
#include "GDCC/WSpaceTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // IncStream
   //
   class IncStream : public GDCC::TokenStream
   {
   public:
      IncStream(std::streambuf &buf, GDCC::String file) : GDCC::TokenStream{&pbuf},
         istr{buf, file}, tbuf{istr}, cdir{tbuf}, ddir{cdir}, edir{ddir},
         idir{edir}, ldir{idir}, pdir{ldir}, udir{pdir}, pbuf{udir} {}

   protected:
      using IStr = IStream;
      using TBuf = GDCC::StreamTBuf<IStream>;
      using CDir = ConditionDTBuf;
      using DDir = DefineDTBuf;
      using EDir = ErrorDTBuf;
      using IDir = IncludeDTBuf;
      using LDir = LineDTBuf;
      using PDir = PragmaDTBuf;
      using UDir = UndefDTBuf;
      using PBuf = PragmaTBuf;

      IStr istr;
      TBuf tbuf;
      CDir cdir;
      DDir ddir;
      EDir edir;
      IDir idir;
      LDir ldir;
      PDir pdir;
      UDir udir;
      PBuf pbuf;
   };

   //
   // TStream
   //
   class TStream : public IncStream
   {
   public:
      TStream(std::streambuf &buf, GDCC::String file) : IncStream{buf, file},
         mbuf{pbuf}, wbuf{mbuf}, sbuf{wbuf}, cbuf{sbuf} {tkbuf(&cbuf);}

   protected:
      using MBuf = MacroTBuf;
      using WBuf = GDCC::WSpaceTBuf;
      using SBuf = StringTBuf;
      using CBuf = ConcatTBuf;

      MBuf mbuf;
      WBuf wbuf;
      SBuf sbuf;
      CBuf cbuf;
   };
}

#endif//C__TStream_H__

