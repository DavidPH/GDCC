//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
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

namespace GDCC::CC
{
   //
   // --warn-incompatible-declaration
   //
   static Core::WarnOpt WarnDeclCompatOpt
   {
      &Core::GetWarnOptList(), Option::Base::Info()
         .setName("warn-incompatible-declaration")
         .setGroup("warnings")
         .setDescS("Warns on incompatible declarations.")
         .setDescL("Warns on incompatible declarations.\n\n"
            "Enabled by --warn-common."),

      &WarnDeclCompat
   };

   //
   // --warn-delay-call
   //
   static Core::WarnOpt WarnDelayCallOpt
   {
      &Core::GetWarnOptList(), Option::Base::Info()
         .setName("warn-delay-call")
         .setGroup("warnings")
         .setDescS("Warns on calls to delay functions.")
         .setDescL("Warns on calls to delay functions.\n\n"
            "Enabled by --warn-common."),

      &WarnDelayCall
   };

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

   //
   // --warn-unknown-attribute
   //
   static Core::WarnOpt WarnUnknownAttrOpt
   {
      &Core::GetWarnOptList(), Option::Base::Info()
         .setName("warn-unknown-attribute")
         .setGroup("warnings")
         .setDescS("Warns on unknown attributes.")
         .setDescL("Warns on unknown attributes.\n\n"
            "Enabled by --warn-common."),

      &WarnUnknownAttr
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::CC
{
   Core::Warning WarnDeclCompat{&Core::WarnCommon, "--warn-incompatible-declaration"};
   Core::Warning WarnDelayCall{&Core::WarnCommon, "--warn-delay-call"};
   Core::Warning WarnFileSemico{&Core::WarnStrict, "--warn-file-scope-semicolon"};
   Core::Warning WarnForwardRef{&Core::WarnCommon, "--warn-forward-reference"};
   Core::Warning WarnUnknownAttr{&Core::WarnCommon, "--warn-unknown-attribute"};
}

// EOF

