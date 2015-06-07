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

#include "ACC/IncludeDTBuf.hpp"

#include "ACC/Parse.hpp"
#include "ACC/TStream.hpp"

#include "AST/Statement.hpp"

#include "CPP/Macro.hpp"

#include "Core/Path.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // IncludeDTBuf constructor
      //
      IncludeDTBuf::IncludeDTBuf(Core::TokenBuf &src_, CPP::IStreamHeader &istr_,
         MacroMap &macros_, PragmaData &pragd_, CPP::PragmaParserBase &pragp_,
         Core::String dir_, Scope_Global &scope_, IR::Program &prog_) :
         CPP::IncludeDTBuf{src_, istr_, macros_, pragd_, pragp_, dir_},
         macros(macros_),
         pragd(pragd_),
         prog (prog_),
         scope(scope_)
      {
      }

      //
      // ImportDTBuf constructor
      //
      ImportDTBuf::ImportDTBuf(Core::TokenBuf &src_, CPP::IStreamHeader &istr_,
         MacroMap &macros_, PragmaData &pragd_, CPP::PragmaParserBase &pragp_,
         Core::String dir_, Scope_Global &scope_, IR::Program &prog_) :
         IncludeDTBuf{src_, istr_, macros_, pragd_, pragp_, dir_, scope_, prog_}
      {
      }

      //
      // IncludeDTBuf::doInc
      //
      void IncludeDTBuf::doInc(Core::String name,
         std::unique_ptr<std::streambuf> &&newStr)
      {
         macros.linePush(CPP::Macro::Stringize(name));

         str = std::move(newStr);
         inc.reset(new IncStream(*str, macros, pragd, pragp, name,
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
         std::unique_ptr<std::streambuf> &&newStr)
      {
         ImportStream tstr{*newStr, macros, pragd, pragp, name};
         Parser       ctx {tstr, pragd, prog, true};

         pragd.push();
         macros.tempPush();

         // Read declarations.
         while(ctx.in.peek().tok != Core::TOK_EOF)
            ctx.getDecl(scope);

         macros.tempDrop();
         pragd.drop();
      }
   }
}

// EOF

