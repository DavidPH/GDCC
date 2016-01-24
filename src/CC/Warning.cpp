//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
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
               "Enabled by --warn-strict."),

         &WarnFileSemico
      };

      //
      // --warn-forward-reference
      //
      static Core::WarnOpt WarnForwardRefOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-forward-reference")
            .setGroup("warnings")
            .setDescS("Warns on forward function references.")
            .setDescL("Warns on forward function references.\n\n"
               "Enabled by --warn-common."),

         &WarnForwardRef
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
      Core::Warning WarnFileSemico{&Core::WarnStrict, "--warn-file-scope-semicolon"};
      Core::Warning WarnForwardRef{&Core::WarnCommon, "--warn-forward-reference"};
   }
}

// EOF

