//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS extra output.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/Linkage.hpp"
#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // --bc-zdacs-dump-ScriptI
         //
         static GDCC::Option::CStr DumpScriptI
         {
            &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
               .setName("bc-zdacs-dump-ScriptI")
               .setGroup("output")
               .setDescS("Writes a list of numbered scripts to a file.")
               .setDescS("Writes a list of numbered scripts to a file. "
                  "Use - to write to stdout. Structure is: <glyph> <linka> "
                  "<valueInt>")
         };
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::putExtra
         //
         void Info::putExtra()
         {
            if(auto outName = DumpScriptI.data())
            {
               auto buf = Core::FileOpenStream(outName, std::ios_base::out);
               if(!buf)
               {
                  std::cerr << "couldn't open '" << outName << "' for writing\n";
                  throw EXIT_FAILURE;
               }

               std::ostream out{buf.get()};

               for(auto &fn : prog->rangeFunction())
               {
                  if(fn.ctype != IR::CallType::ScriptI)
                     continue;

                  out << fn.glyph << ' ' << fn.linka << ' ' << fn.valueInt << '\n';
               }
            }
         }
      }
   }
}

// EOF

