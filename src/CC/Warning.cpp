//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C warnings
//
//-----------------------------------------------------------------------------

#include "CC/Warning.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace CC
   {
      //
      // --warn-file-scope-semicolon
      //
      static Core::WarnOpt WarnFileSemicoOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-file-scope-semicolon")
            .setGroup("warnings")
            .setDescS("Warns on extraneous file-scope semicolon.")
            .setDescL("Warns on extraneous file-scope semicolon.\n\n"
               "Enabled by --warn-common."),

         &WarnFileSemico
      };
   }
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace CC
   {
      Core::Warning WarnFileSemico{&Core::WarnCommon, "--warn-file-scope-semicolon"};
   }
}

// EOF

