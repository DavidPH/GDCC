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
#include <vector>


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
      class IncludeLang;
      class MacroMap;
      class PragmaDataBase;
      class PragmaParserBase;

      //
      // IncludeDTBuf
      //
      class IncludeDTBuf : public DirectiveTBuf
      {
      public:
         IncludeDTBuf(Core::TokenBuf &src, IStreamHeader &istr,
            IncludeLang &langs, MacroMap &macros, PragmaDataBase &pragd,
            PragmaParserBase &pragp, Core::String dir);

         virtual ~IncludeDTBuf();

      protected:
         virtual bool directive(Core::Token const &tok);

         virtual void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);

         bool doIncHdr(Core::String name, Core::Origin pos);
         bool doIncStr(Core::String name, Core::Origin pos);

         void readInc(Core::Token const &tok);

         bool tryIncSys(Core::String name);
         bool tryIncUsr(Core::String name);

         virtual void underflow();

         std::unique_ptr<std::streambuf>    str;
         std::unique_ptr<Core::TokenStream> inc;

         IStreamHeader    &istr;
         IncludeLang      &langs;
         MacroMap         &macros;
         PragmaDataBase   &pragd;
         PragmaParserBase &pragp;
         Core::String      dir;
      };

      //
      // IncludeLang
      //
      class IncludeLang
      {
      public:
         IncludeLang();
         explicit IncludeLang(char const *lang);

         void addLang(char const *lang);

         std::vector<std::string>::const_iterator
         begin() const {return langs.begin();}

         std::vector<std::string>::const_iterator
         end() const {return langs.end();}

      private:
         std::vector<std::string> langs;
      };
   }
}

#endif//GDCC__CPP__IncludeDTBuf_H__

