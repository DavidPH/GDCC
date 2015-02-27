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

#include "../ACC/Pragma.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Program;
   }

   namespace ACC
   {
      class Scope_Global;

      //
      // IncludeDTBuf
      //
      class IncludeDTBuf : public CPP::IncludeDTBuf
      {
      public:
         IncludeDTBuf(Core::TokenBuf &src_, CPP::IStreamHeader &istr_,
            CPP::MacroMap &macros_, PragmaData &pragd_,
            CPP::PragmaParserBase &pragp_, Core::String dir_,
            Scope_Global &scope_, IR::Program &prog_) :
            CPP::IncludeDTBuf{src_, istr_, macros_, pragd_, pragp_, dir_},
            pragd(pragd_),
            prog (prog_),
            scope(scope_)
         {
         }

      protected:
         virtual void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);

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
         ImportDTBuf(Core::TokenBuf &src_, CPP::IStreamHeader &istr_,
            CPP::MacroMap &macros_, PragmaData &pragd_,
            CPP::PragmaParserBase &pragp_, Core::String dir_,
            Scope_Global &scope_, IR::Program &prog_) :
            IncludeDTBuf{src_, istr_, macros_, pragd_, pragp_, dir_, scope_, prog_}
         {
         }

      protected:
         virtual bool directive(Core::Token const &tok);

         virtual void doInc(Core::String name, std::unique_ptr<std::streambuf> &&buf);
      };
   }
}

#endif//GDCC__ACC__IncludeDTBuf_H__

