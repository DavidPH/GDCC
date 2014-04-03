//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C token stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__TStream_H__
#define GDCC__CPP__TStream_H__

#include "../CPP/ConcatTBuf.hpp"
#include "../CPP/ConditionDTBuf.hpp"
#include "../CPP/IStream.hpp"
#include "../CPP/IncludeDTBuf.hpp"
#include "../CPP/MacroDTBuf.hpp"
#include "../CPP/MacroTBuf.hpp"
#include "../CPP/PPTokenTBuf.hpp"
#include "../CPP/PragmaDTBuf.hpp"
#include "../CPP/StringTBuf.hpp"

#include "../Core/BufferTBuf.hpp"
#include "../Core/StreamTBuf.hpp"
#include "../Core/TokenStream.hpp"
#include "../Core/WSpaceTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // IncStream
      //
      class IncStream : public Core::TokenStream
      {
      public:
         //
         // constructor
         //
         IncStream(std::streambuf &buf_, MacroMap &macros, Pragma &pragma,
            Core::String file, Core::String dir) :
            Core::TokenStream{&pbuf},
            istr{buf_, file},
            tbuf{istr},
            cdir{tbuf, macros},
            ddir{cdir, macros},
            edir{ddir},
            idir{edir, istr, macros, pragma, dir},
            ldir{idir, macros},
            pdir{ldir, pragma},
            udir{pdir, macros},
            pbuf{udir, pragma}
         {
         }

      protected:
         using IStr = IStream;
         using TBuf = Core::StreamTBuf<IStream>;
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
         PPStream(std::streambuf &buf_, MacroMap &macros, Pragma &pragma,
            Core::String file, Core::String dir) :
            IncStream{buf_, macros, pragma, file, dir},
            mbuf{pbuf, macros},
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
         TStream(std::streambuf &buf_, MacroMap &macros, Pragma &pragma,
            Core::String file, Core::String dir) :
            PPStream{buf_, macros, pragma, file, dir},
            wbuf{cbuf},
            ppbf{wbuf},
            bbuf{ppbf}
         {
            tkbuf(&bbuf);
         }

      protected:
         using WBuf = Core::WSpaceTBuf;
         using PPBf = PPTokenTBuf;
         using BBuf = Core::BufferTBuf<8, 3>;

         WBuf wbuf;
         PPBf ppbf;
         BBuf bbuf;
      };
   }
}

#endif//GDCC__CPP__TStream_H__

