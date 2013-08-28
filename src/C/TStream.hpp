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
#include "PPTokenTBuf.hpp"
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
         mbuf{pbuf}, sbuf{mbuf}, wbuf{sbuf}, cbuf{wbuf}, ppbf{cbuf} {tkbuf(&ppbf);}

   protected:
      using MBuf = MacroTBuf;
      using SBuf = StringTBuf;
      using WBuf = GDCC::WSpaceTBuf;
      using CBuf = ConcatTBuf;
      using PPBf = PPTokenTBuf;

      MBuf mbuf;
      SBuf sbuf;
      WBuf wbuf;
      CBuf cbuf;
      PPBf ppbf;
   };
}

#endif//C__TStream_H__

