//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
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

#include "../ACC/Pragma.hpp"

#include "../CPP/IncludeDTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // IncludeDTBuf
   //
   class IncludeDTBuf : public CPP::IncludeDTBuf
   {
   public:
      IncludeDTBuf(Core::TokenBuf &src, Core::TokenSource &tsrc,
         CPP::IncludeLang &langs, MacroMap &macros, PragmaData &pragd,
         CPP::PragmaParserBase &pragp, Core::String dir,
         Scope_Global &scope, IR::Program &prog);

   protected:
      virtual void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);

      MacroMap     &macros;
      PragmaData   &pragd;
      IR::Program  &prog;
      Scope_Global &scope;
   };

   //
   // ImportDTBuf
   //
   class ImportDTBuf : public IncludeDTBuf
   {
   public:
      ImportDTBuf(Core::TokenBuf &src, Core::TokenSource &tsrc,
         CPP::IncludeLang &langs, MacroMap &macros, PragmaData &pragd,
         CPP::PragmaParserBase &pragp, Core::String dir,
         Scope_Global &scope, IR::Program &prog);

   protected:
      virtual bool directive(Core::Token const &tok);

      virtual void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);
   };
}

#endif//GDCC__ACC__IncludeDTBuf_H__

