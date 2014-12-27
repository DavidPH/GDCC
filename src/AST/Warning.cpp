//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree generalized attribute handling.
//
//-----------------------------------------------------------------------------

#include "AST/Warning.hpp"

#include "Core/Warning.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace AST
   {
      //
      // --warn-deprecated
      //
      static Core::WarnOpt WarnDeprecatedOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-deprecated")
            .setGroup("warnings")
            .setDescS("Warns on use of deprecated entities.")
            .setDescL("Warns on use of deprecated entities.\n\n"
               "Enabled by --warn-common."),

         &WarnDeprecated
      };

      //
      // --warn-return-type
      //
      static Core::WarnOpt WarnReturnTypeOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-return-type")
            .setGroup("warnings")
            .setDescS("Warns on suspicious return type uasge.")
            .setDescL("Warns on suspicious return type usage, such as having "
               "no return statement in a non-void function.\n\n"
               "Enabled by --warn-common."),

         &WarnReturnType
      };

      //
      // --warn-unused-initializer
      //
      static Core::WarnOpt WarnUnusedInitOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-unused-initializer")
            .setGroup("warnings")
            .setDescS("Warns about unused initializer expressions.")
            .setDescL("Warns about unused initializer expressions, such as "
               "having too many initializers for an aggregate or overriding "
               "initializers using designators.\n\n"
               "Enabled by --warn-common."),

         &WarnUnusedInit
      };
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace AST
   {
      Core::Warning WarnDeprecated{&Core::WarnCommon, "--warn-deprecated"};
      Core::Warning WarnReturnType{&Core::WarnCommon, "--warn-return-type"};
      Core::Warning WarnUnusedInit{&Core::WarnCommon, "--warn-unused-initializer"};
   }
}

// EOF

