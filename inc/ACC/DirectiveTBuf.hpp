//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS preprocessor directive token buffers.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__DirectiveTBuf_H__
#define GDCC__ACC__DirectiveTBuf_H__

#include "../ACC/Types.hpp"

#include "../CPP/DirectiveTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // IgnoreDTBuf
   //
   class IgnoreDTBuf : public CPP::DirectiveTBuf
   {
   public:
      IgnoreDTBuf(Core::TokenBuf &src_, bool importing_) :
         CPP::DirectiveTBuf{src_}, importing{importing_} {}

   protected:
      virtual bool directive(Core::Token const &tok);

      bool importing;
   };

   //
   // LibraryDTBuf
   //
   class LibraryDTBuf : public CPP::DirectiveTBuf
   {
   public:
      LibraryDTBuf(Core::TokenBuf &src_, PragmaData &prag_) :
         CPP::DirectiveTBuf{src_}, prag(prag_) {}

   protected:
      virtual bool directive(Core::Token const &tok);

      PragmaData &prag;
   };
}

#endif//GDCC__ACC__DirectiveTBuf_H__

