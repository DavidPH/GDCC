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
#include "PragmaDTBuf.hpp"
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
      //
      // constructor
      //
      IncStream(std::streambuf &buf, Pragma &pragma, GDCC::String file,
         GDCC::String dir) :
         GDCC::TokenStream{&pbuf},
         istr{buf, file},
         tbuf{istr},
         cdir{tbuf},
         ddir{cdir},
         edir{ddir},
         idir{edir, pragma, dir},
         ldir{idir},
         pdir{ldir, pragma},
         udir{pdir},
         pbuf{udir, pragma}
      {
      }

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
   // PPStream
   //
   class PPStream : public IncStream
   {
   public:
      //
      // constructor
      //
      PPStream(std::streambuf &buf, Pragma &pragma, GDCC::String file,
         GDCC::String dir) :
         IncStream{buf, pragma, file, dir},
         mbuf{pbuf},
         sbuf{mbuf},
         cbuf{sbuf}
      {
         tkbuf(&cbuf);
      }

   protected:
      using MBuf = MacroTBuf;
      using SBuf = StringTBuf;
      using CBuf = ConcatTBuf;

      MBuf mbuf;
      SBuf sbuf;
      CBuf cbuf;
   };

   //
   // TStream
   //
   class TStream : public PPStream
   {
   public:
      //
      // constructor
      //
      TStream(std::streambuf &buf, Pragma &pragma, GDCC::String file,
         GDCC::String dir) :
         PPStream{buf, pragma, file, dir},
         wbuf{cbuf},
         ppbf{wbuf}
      {
         tkbuf(&ppbf);
      }

   protected:
      using WBuf = GDCC::WSpaceTBuf;
      using PPBf = PPTokenTBuf;

      WBuf wbuf;
      PPBf ppbf;
   };
}

#endif//C__TStream_H__

