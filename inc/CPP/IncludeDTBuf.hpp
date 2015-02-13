//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
   namespace Core
   {
      class TokenStream;
   }

   namespace CPP
   {
      class IStreamHeader;
      class MacroMap;
      class Pragma;

      //
      // IncludeDTBuf
      //
      class IncludeDTBuf : public DirectiveTBuf
      {
      public:
         IncludeDTBuf(Core::TokenBuf &src, IStreamHeader &istr,
            MacroMap &macros, Pragma &pragma, Core::String dir);

         virtual ~IncludeDTBuf();


         static void AddIncludeLang(char const *lang);

      protected:
         virtual void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);

         bool doIncHdr(Core::String name, Core::Origin pos);
         bool doIncStr(Core::String name, Core::Origin pos);

         virtual bool directive(Core::Token const &tok);

         bool tryIncSys(Core::String name);
         bool tryIncUsr(Core::String name);

         virtual void underflow();

         std::unique_ptr<std::streambuf>    str;
         std::unique_ptr<Core::TokenStream> inc;

         IStreamHeader &istr;
         MacroMap      &macros;
         Pragma        &pragma;
         Core::String   dir;
      };
   }
}

#endif//GDCC__CPP__IncludeDTBuf_H__

