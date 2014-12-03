//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
      class MacroMap;
      class Pragma;

      //
      // IncludeDTBuf
      //
      class IncludeDTBuf final : public DirectiveTBuf
      {
      public:
         IncludeDTBuf(Core::TokenBuf &src_, IStream &istr_, MacroMap &macros_,
            Pragma &pragma_, Core::String dir_) :
            DirectiveTBuf{src_},
            istr(istr_),
            macros(macros_),
            pragma(pragma_),
            dir{dir_}
         {
         }

         virtual ~IncludeDTBuf();

      protected:
         void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);

         bool doIncHdr(Core::String name, Core::Origin pos);
         bool doIncStr(Core::String name, Core::Origin pos);

         virtual bool directive(Core::Token const &tok);

         bool tryIncSys(Core::String name);
         bool tryIncUsr(Core::String name);

         virtual void underflow();

         std::unique_ptr<std::streambuf> str;
         std::unique_ptr<IncStream>      inc;

         IStream     &istr;
         MacroMap    &macros;
         Pragma      &pragma;
         Core::String dir;
      };
   }
}

#endif//GDCC__CPP__IncludeDTBuf_H__

