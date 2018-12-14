//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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
#include "../CPP/IncludeDTBuf.hpp"
#include "../CPP/MacroDTBuf.hpp"
#include "../CPP/MacroTBuf.hpp"
#include "../CPP/PPTokenTBuf.hpp"
#include "../CPP/PragmaDTBuf.hpp"
#include "../CPP/StringTBuf.hpp"

#include "../Core/BufferTBuf.hpp"
#include "../Core/SourceTBuf.hpp"
#include "../Core/TokenStream.hpp"
#include "../Core/WSpaceTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
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
      IncStream(Core::TokenSource &tsrc, IncludeLang &langs, MacroMap &macros,
         PragmaDataBase &pragd, PragmaParserBase &pragp, Core::String dir) :
         Core::TokenStream{&udir},
         tbuf{tsrc},
         cdir{tbuf, macros},
         ddir{cdir, macros},
         edir{ddir},
         idir{edir, tsrc, langs, macros, pragd, pragp, dir},
         ldir{idir, macros},
         pdir{ldir, pragp},
         udir{pdir, macros}
      {
      }

   protected:
      using TBuf = Core::SourceTBuf<>;
      using CDir = ConditionDTBuf;
      using DDir = DefineDTBuf;
      using EDir = ErrorDTBuf;
      using IDir = IncludeDTBuf;
      using LDir = LineDTBuf;
      using PDir = PragmaDTBuf;
      using UDir = UndefDTBuf;

      TBuf tbuf;
      CDir cdir;
      DDir ddir;
      EDir edir;
      IDir idir;
      LDir ldir;
      PDir pdir;
      UDir udir;
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
      PPStream(Core::TokenSource &tsrc, IncludeLang &langs, MacroMap &macros,
         PragmaDataBase &pragd, PragmaParserBase &pragp, Core::String dir) :
         IncStream{tsrc, langs, macros, pragd, pragp, dir},
         mbuf{udir, macros},
         pbuf{mbuf, pragp},
         pubf{pbuf, pragd},
         sbuf{pubf},
         cbuf{sbuf}
      {
         tkbuf(&cbuf);
      }

   protected:
      using MBuf = MacroTBuf;
      using PBuf = PragmaTBuf;
      using PuBf = PragmaPushTBuf;
      using SBuf = StringTBuf;
      using CBuf = ConcatTBuf;

      MBuf mbuf;
      PBuf pbuf;
      PuBf pubf;
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
      TStream(Core::TokenSource &tsrc, IncludeLang &langs, MacroMap &macros,
         PragmaDataBase &pragd, PragmaParserBase &pragp, Core::String dir) :
         PPStream{tsrc, langs, macros, pragd, pragp, dir},
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

#endif//GDCC__CPP__TStream_H__

