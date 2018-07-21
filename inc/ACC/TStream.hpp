//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS token stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__TStream_H__
#define GDCC__ACC__TStream_H__

#include "../ACC/IStream.hpp"

#include "../ACC/DirectiveTBuf.hpp"
#include "../ACC/IncludeDTBuf.hpp"
#include "../ACC/Macro.hpp"
#include "../ACC/MacroDTBuf.hpp"
#include "../ACC/PPTokenTBuf.hpp"
#include "../ACC/Pragma.hpp"

#include "../CPP/ConcatTBuf.hpp"
#include "../CPP/ConditionDTBuf.hpp"
#include "../CPP/MacroDTBuf.hpp"
#include "../CPP/MacroTBuf.hpp"
#include "../CPP/PragmaDTBuf.hpp"
#include "../CPP/StringTBuf.hpp"

#include "../Core/BufferTBuf.hpp"
#include "../Core/StreamTBuf.hpp"
#include "../Core/TokenStream.hpp"
#include "../Core/WSpaceTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // ImportStream
   //
   class ImportStream : public Core::TokenStream
   {
   public:
      //
      // constructor
      //
      ImportStream(std::streambuf &buf_, MacroMap &macros, PragmaData &pragd,
         CPP::PragmaParserBase &pragp, Core::String file) :
         Core::TokenStream{&bbuf},
         istr{buf_, file},
         tbuf{istr},
         cdir{tbuf, macros},
         ddir{cdir, macros, pragd, true},
         ldir{ddir, pragd},
         pdir{ldir, pragp},
         ignd{pdir, true},

         mbuf{ignd, macros, Core::STR___va_args__},
         pubf{mbuf, pragd},
         sbuf{pubf, ParseEscape},
         cbuf{sbuf},

         wbuf{cbuf},
         ppbf{wbuf},
         bbuf{ppbf}
      {
      }

   protected:
      using IStr = IStream;
      using TBuf = Core::StreamTBuf<IStream>;
      using CDir = CPP::ConditionDTBuf;
      using DDir = DefineDTBuf;
      using LDir = LibraryDTBuf;
      using PDir = CPP::PragmaDTBuf;
      using IgnD = IgnoreDTBuf;

      using MBuf = CPP::MacroTBuf;
      using PuBf = CPP::PragmaPushTBuf;
      using SBuf = CPP::StringTBuf;
      using CBuf = CPP::ConcatTBuf;

      using WBuf = Core::WSpaceTBuf;
      using PPBf = PPTokenTBuf;
      using BBuf = Core::BufferTBuf<8, 3>;

      IStr istr;
      TBuf tbuf;
      CDir cdir;
      DDir ddir;
      LDir ldir;
      PDir pdir;
      IgnD ignd;

      MBuf mbuf;
      PuBf pubf;
      SBuf sbuf;
      CBuf cbuf;

      WBuf wbuf;
      PPBf ppbf;
      BBuf bbuf;
   };

   //
   // IncStream
   //
   class IncStream : public Core::TokenStream
   {
   public:
      //
      // constructor
      //
      IncStream(std::streambuf &buf_, CPP::IncludeLang &langs,
         MacroMap &macros, PragmaData &pragd, CPP::PragmaParserBase &pragp,
         Core::String file, Core::String dir, Scope_Global &scope,
         IR::Program &prog) :
         Core::TokenStream{&udir},
         istr{buf_, file},
         tbuf{istr},
         cdir{tbuf, macros},
         ddir{cdir, macros, pragd, false},
         impd{ddir, istr, langs, macros, pragd, pragp, dir, scope, prog},
         idir{impd, istr, langs, macros, pragd, pragp, dir, scope, prog},
         pdir{idir, pragp},
         ignd{pdir, false},
         udir{ignd, macros}
      {
      }

   protected:
      using IStr = IStream;
      using TBuf = Core::StreamTBuf<IStream>;
      using CDir = CPP::ConditionDTBuf;
      using DDir = DefineDTBuf;
      using ImpD = ImportDTBuf;
      using IDir = IncludeDTBuf;
      using PDir = CPP::PragmaDTBuf;
      using IgnD = IgnoreDTBuf;
      using UDir = CPP::UndefDTBuf;

      IStr istr;
      TBuf tbuf;
      CDir cdir;
      DDir ddir;
      ImpD impd;
      IDir idir;
      PDir pdir;
      IgnD ignd;
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
      PPStream(std::streambuf &buf_, CPP::IncludeLang &langs,
         MacroMap &macros, PragmaData &pragd, CPP::PragmaParserBase &pragp,
         Core::String file, Core::String dir, Scope_Global &scope,
         IR::Program &prog) :
         IncStream{buf_, langs, macros, pragd, pragp, file, dir, scope, prog},
         mbuf{udir, macros, Core::STR___va_args__},
         pubf{mbuf, pragd},
         sbuf{pubf, ParseEscape},
         cbuf{sbuf}
      {
         tkbuf(&cbuf);
      }

   protected:
      using MBuf = CPP::MacroTBuf;
      using PuBf = CPP::PragmaPushTBuf;
      using SBuf = CPP::StringTBuf;
      using CBuf = CPP::ConcatTBuf;

      MBuf mbuf;
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
      TStream(std::streambuf &buf_, CPP::IncludeLang &langs,
         MacroMap &macros, PragmaData &pragd, CPP::PragmaParserBase &pragp,
         Core::String file, Core::String dir, Scope_Global &scope,
         IR::Program &prog) :
         PPStream{buf_, langs, macros, pragd, pragp, file, dir, scope, prog},
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

#endif//GDCC__ACC__TStream_H__

