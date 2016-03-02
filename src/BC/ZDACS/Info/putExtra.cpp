//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS extra output.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/Linkage.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace BC
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
   namespace BC
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

