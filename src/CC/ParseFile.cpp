//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C file parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Scope/Global.hpp"

#include "AST/Statement.hpp"

#include "CPP/Macro.hpp"
#include "CPP/TStream.hpp"

#include "Core/File.hpp"
#include "Core/Path.hpp"
#include "Core/StringBuf.hpp"

#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetGlobalLabel
      //
      Core::String GetGlobalLabel(std::size_t hash)
      {
         char buf[2 + (sizeof(std::size_t) * CHAR_BIT + 3) / 4 + 1];

         buf[0] = '_';
         buf[1] = '$';

         std::sprintf(buf + 2, "%*zX", static_cast<int>(sizeof(buf) - 3), hash);

         return {buf, sizeof(buf) - 1};
      }

      //
      // ParseFile
      //
      void ParseFile(char const *inName, IR::Program &prog)
      {
         auto buf = Core::FileOpenBlock(inName);
         if(!buf)
         {
            std::cerr << "couldn't open '" << inName << "' for reading\n";
            throw EXIT_FAILURE;
         }


         Core::String      file {inName};
         CPP::IncludeLang  langs{"C"};
         CPP::MacroMap     macr {CPP::Macro::Stringize(file)};
         Core::String      path {Core::PathDirname(file)};
         CPP::PragmaData   pragd{};
         CPP::PragmaParser pragp{pragd};
         Core::StringBuf   sbuf {buf->data(), buf->size()};
         CPP::TStream      tstr {sbuf, langs, macr, pragd, pragp, file, path};
         Parser            ctx  {tstr, pragd, prog};
         Scope_Global      scope{GetGlobalLabel(buf->getHash())};

         // Read declarations.
         while(ctx.in.peek().tok != Core::TOK_EOF)
            ctx.getDecl(scope);

         // Add ACS libraries.
         for(auto const &lib : pragd.stateLibrary)
            prog.getImport(lib);

         scope.allocAuto();

         // Generate IR data.
         scope.genIR(prog);
      }
   }
}

// EOF

