//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS file parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"
#include "ACC/Macro.hpp"
#include "ACC/Scope.hpp"
#include "ACC/TSource.hpp"
#include "ACC/TStream.hpp"

#include "CC/Exp.hpp"

#include "CPP/IStream.hpp"

#include "Core/File.hpp"
#include "Core/Path.hpp"
#include "Core/StringBuf.hpp"

#include "IR/Program.hpp"

#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // ParseFile
   //
   void ParseFile(char const *inName, IR::Program &prog)
   {
      auto buf = Core::FileOpenBlock(inName);

      Core::String     file {inName};
      CPP::IncludeLang langs{"ACS"};
      MacroMap         macr {CPP::Macro::Stringize(file)};
      Core::String     path {Core::PathDirname(file)};
      PragmaData       pragd{};
      PragmaParser     pragp{pragd};
      Core::StringBuf  sbuf {buf->data(), buf->size()};
      CPP::IStream     istr {sbuf, file};
      TSource          tsrc {istr, istr.getOriginSource()};
      Scope_Global     scope{CC::GetGlobalLabel(buf->getHash())};
      Factory          fact {};
      TStream          tstr {tsrc, fact, langs, macr, pragd, pragp, path, scope, prog};
      Parser           ctx  {tstr, fact, pragd, prog};

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

// EOF

