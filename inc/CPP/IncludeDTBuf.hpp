//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// #include directive token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__IncludeDTBuf_H__
#define GDCC__CPP__IncludeDTBuf_H__

#include "../CPP/DirectiveTBuf.hpp"

#include <memory>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      class IStream;
      class Pragma;

      //
      // IncludeDTBuf
      //
      class IncludeDTBuf final : public DirectiveTBuf
      {
      public:
         IncludeDTBuf(Core::TokenBuf &src_, IStream &istr_, Pragma &pragma_,
            Core::String dir_) :
            DirectiveTBuf{src_},
            istr{istr_},
            pragma{pragma_},
            dir{dir_}
         {
         }

         virtual ~IncludeDTBuf();

      protected:
         virtual bool directive(Core::Token const &tok);

         virtual void underflow();

         bool tryIncSys(Core::String str);
         bool tryIncUsr(Core::String str);

         std::unique_ptr<std::streambuf> str;
         std::unique_ptr<IncStream>      inc;

         IStream     &istr;
         Pragma      &pragma;
         Core::String dir;
      };
   }
}

#endif//GDCC__CPP__IncludeDTBuf_H__

