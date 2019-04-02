//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

namespace GDCC::CPP
{
   //
   // IncludeDTBuf
   //
   class IncludeDTBuf : public DirectiveTBuf
   {
   public:
      IncludeDTBuf(Core::TokenBuf &src, Core::TokenSource &tsrc,
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

      std::unique_ptr<std::streambuf>    incBuf;
      std::unique_ptr<IStream>           incStr;
      std::unique_ptr<Core::TokenSource> incSrc;
      std::unique_ptr<Core::TokenStream> inc;

      Core::TokenSource &tsrc;
      IncludeLang       &langs;
      MacroMap          &macros;
      PragmaDataBase    &pragd;
      PragmaParserBase  &pragp;
      Core::String       dir;
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

#endif//GDCC__CPP__IncludeDTBuf_H__

