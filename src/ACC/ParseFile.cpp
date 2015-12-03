//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS file parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Exp.hpp"
#include "ACC/Macro.hpp"
#include "ACC/Scope.hpp"
#include "ACC/TStream.hpp"

#include "AST/Statement.hpp"

#include "CC/Exp.hpp"

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
   namespace ACC
   {
      //
      // ParseFile
      //
      void ParseFile(char const *inName, IR::Program &prog)
      {
         // Saves and restores certain global function pointers.
         struct FuncSave
         {
            FuncSave()
            {
               expPromo_Assign = CC::ExpPromo_Assign;
               CC::ExpPromo_Assign = ACC::ExpPromo_Assign;
            }

            ~FuncSave()
            {
               CC::ExpPromo_Assign = expPromo_Assign;
            }

            AST::Exp::CRef (*expPromo_Assign)(AST::Type const *typeL,
               AST::Exp const *e, Core::Origin pos);

         } funcSave;


         auto buf = Core::FileOpenBlock(inName);
         if(!buf)
         {
            std::cerr << "couldn't open '" << inName << "' for reading\n";
            throw EXIT_FAILURE;
         }

         Core::String     file {inName};
         CPP::IncludeLang langs{"ACS"};
         MacroMap         macr {CPP::Macro::Stringize(file)};
         Core::String     path {Core::PathDirname(file)};
         PragmaData       pragd{};
         PragmaParser     pragp{pragd};
         Core::StringBuf  sbuf{buf->data(), buf->size()};
         Scope_Global     scope{CC::GetGlobalLabel(buf->getHash())};
         TStream          tstr{sbuf, langs, macr, pragd, pragp, file, path, scope, prog};
         Parser           ctx {tstr, pragd, prog};

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

