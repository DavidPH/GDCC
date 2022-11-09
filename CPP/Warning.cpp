//-----------------------------------------------------------------------------
//
// Copyright (C) 2022 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C preprocessor warnings
//
//-----------------------------------------------------------------------------

#include "CPP/Warning.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::CPP
{
   //
   // --warn-unknown-pragma
   //
   static Core::WarnOpt WarnUnknownPragmaOpt
   {
      &Core::GetWarnOptList(), Option::Base::Info()
         .setName("warn-unknown-pragma")
         .setGroup("warnings")
         .setDescS("Warns on unknown pragmas.")
         .setDescL("Warns on unknown pragmas.\n\n"
            "Enabled by --warn-common."),

      &WarnUnknownPragma
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::CPP
{
   Core::Warning WarnUnknownPragma{&Core::WarnCommon, "--warn-unknown-pragma"};
}

// EOF

