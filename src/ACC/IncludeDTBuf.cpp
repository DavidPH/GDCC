//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// #include directive token buffer.
//
//-----------------------------------------------------------------------------

#include "ACC/IncludeDTBuf.hpp"

#include "ACC/Parse.hpp"
#include "ACC/TSource.hpp"
#include "ACC/TStream.hpp"

#include "CPP/IStream.hpp"
#include "CPP/Macro.hpp"

#include "Core/Path.hpp"

#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // IncludeDTBuf constructor
   //
   IncludeDTBuf::IncludeDTBuf(
      Core::TokenBuf        &src_,
      Core::TokenSource     &tsrc_,
      Factory               &fact_,
      CPP::IncludeLang      &langs_,
      MacroMap              &macros_,
      PragmaData            &pragd_,
      CPP::PragmaParserBase &pragp_,
      Core::String           dir_,
      Scope_Global          &scope_,
      IR::Program           &prog_) :
      CPP::IncludeDTBuf{src_, tsrc_, langs_, macros_, pragd_, pragp_, dir_},
      fact  {fact_},
      macros{macros_},
      pragd {pragd_},
      prog  {prog_},
      scope {scope_}
   {
   }

   //
   // ImportDTBuf constructor
   //
   ImportDTBuf::ImportDTBuf(
      Core::TokenBuf        &src_,
      Core::TokenSource     &tsrc_,
      Factory               &fact_,
      CPP::IncludeLang      &langs_,
      MacroMap              &macros_,
      PragmaData            &pragd_,
      CPP::PragmaParserBase &pragp_,
      Core::String           dir_,
      Scope_Global          &scope_,
      IR::Program           &prog_) :
      IncludeDTBuf{src_, tsrc_, fact_, langs_, macros_, pragd_, pragp_, dir_, scope_, prog_}
   {
   }

   //
   // IncludeDTBuf::doInc
   //
   void IncludeDTBuf::doInc(Core::String name,
      std::unique_ptr<std::streambuf> &&newBuf)
   {
      macros.linePush(CPP::Macro::Stringize(name));

      incBuf = std::move(newBuf);
      incStr.reset(new CPP::IStream(*incBuf, name));
      incSrc.reset(new TSource(*incStr, incStr->getOriginSource()));
      inc.reset(new IncStream(*incSrc, fact, langs, macros, pragd, pragp,
         Core::PathDirname(name), scope, prog));
   }

   //
   // ImportDTBuf::directive
   //
   bool ImportDTBuf::directive(Core::Token const &tok)
   {
      if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_import)
         return false;

      // Clear directive name.
      src.get();

      readInc(tok);

      return true;
   }

   //
   // ImportDTBuf::doInc
   //
   void ImportDTBuf::doInc(Core::String name,
      std::unique_ptr<std::streambuf> &&newBuf)
   {
      CPP::IStream istr{*newBuf, name};
      TSource      tsrc{istr, istr.getOriginSource()};
      ImportStream tstr{tsrc, macros, pragd, pragp};
      Parser       ctx {tstr, fact, pragd, prog, true};

      pragd.push();
      macros.tempPush();

      // Read declarations.
      while(ctx.in.peek().tok != Core::TOK_EOF)
         ctx.getDecl(scope);

      macros.tempDrop();
      pragd.drop();
   }
}

// EOF

