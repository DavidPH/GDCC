//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// #include directive token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__IncludeDTBuf_H__
#define GDCC__ACC__IncludeDTBuf_H__

#include "../CPP/IncludeDTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // IncludeDTBuf
      //
      class IncludeDTBuf : public CPP::IncludeDTBuf
      {
      public:
         IncludeDTBuf(Core::TokenBuf &src, CPP::IStreamHeader &istr,
            CPP::MacroMap &macros, CPP::PragmaDataBase &pragd,
            CPP::PragmaParserBase &pragp, Core::String dir) :
            CPP::IncludeDTBuf{src, istr, macros, pragd, pragp, dir} {}

      protected:
         virtual void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);
      };
   }
}

#endif//GDCC__ACC__IncludeDTBuf_H__

