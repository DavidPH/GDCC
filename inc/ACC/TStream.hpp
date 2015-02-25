//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
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

#include "../ACC/IncludeDTBuf.hpp"
#include "../ACC/PPTokenTBuf.hpp"

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

namespace GDCC
{
   namespace ACC
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
         IncStream(std::streambuf &buf_, CPP::MacroMap &macros,
            CPP::Pragma &pragma, Core::String file, Core::String dir) :
            Core::TokenStream{&udir},
            istr{buf_, file},
            tbuf{istr},
            cdir{tbuf, macros},
            ddir{cdir, macros},
            idir{ddir, istr, macros, pragma, dir},
            pdir{idir, pragma},
            udir{pdir, macros}
         {
         }

      protected:
         using IStr = IStream;
         using TBuf = Core::StreamTBuf<IStream>;
         using CDir = CPP::ConditionDTBuf;
         using DDir = CPP::DefineDTBuf;
         using IDir = IncludeDTBuf;
         using PDir = CPP::PragmaDTBuf;
         using UDir = CPP::UndefDTBuf;

         IStr istr;
         TBuf tbuf;
         CDir cdir;
         DDir ddir;
         IDir idir;
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
         PPStream(std::streambuf &buf_, CPP::MacroMap &macros,
            CPP::Pragma &pragma, Core::String file, Core::String dir) :
            IncStream{buf_, macros, pragma, file, dir},
            mbuf{udir, macros},
            pubf{mbuf, pragma},
            sbuf{pubf},
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
         TStream(std::streambuf &buf_, CPP::MacroMap &macros,
            CPP::Pragma &pragma, Core::String file, Core::String dir) :
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

#endif//GDCC__ACC__TStream_H__

