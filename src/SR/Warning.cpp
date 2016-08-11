//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation generalized attribute handling.
//
//-----------------------------------------------------------------------------

#include "SR/Warning.hpp"

#include "Core/Warning.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace SR
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
      // --warn-parentheses
      //
      static Core::WarnOpt WarnParenthesesOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-parentheses")
            .setGroup("warnings")
            .setDescS("Warns on suspicious lack of parentheses.")
            .setDescL("Warns on suspicious lack of parentheses, such as with "
               "sizeof or assignments in conditions.\n\n"
               "Enabled by --warn-common."),

         &WarnParentheses
      };

      //
      // --warn-return-type
      //
      static Core::WarnOpt WarnReturnTypeOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-return-type")
            .setGroup("warnings")
            .setDescS("Warns on suspicious return type usage.")
            .setDescL("Warns on suspicious return type usage, such as having "
               "no return statement in a non-void function.\n\n"
               "Enabled by --warn-common."),

         &WarnReturnType
      };

      //
      // --warn-extra-return-type
      //
      static Core::WarnOpt WarnReturnTypeExtOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-extra-return-type")
            .setGroup("warnings")
            .setDescS("Warns on possible suspicious return type uasge.")
            .setDescL("Warns on possible suspicious return type usage, as in "
               "--warn-return-type, but may include false positives.\n\n"
               "Enabled by --warn-extra."),

         &WarnReturnTypeExt
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

      //
      // --warn-unused-value
      //
      static Core::WarnOpt WarnUnusedValueOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-unused-value")
            .setGroup("warnings")
            .setDescS("Warns about expressions with unused value.")
            .setDescL("Warns about expressions with unused value and no side "
               "effects.\n\n"
               "Enabled by --warn-common."),

         &WarnUnusedValue
      };
   }
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace SR
   {
      Core::Warning WarnDeprecated{&Core::WarnCommon, "--warn-deprecated"};
      Core::Warning WarnParentheses{&Core::WarnCommon, "--warn-parentheses"};
      Core::Warning WarnReturnType{&Core::WarnCommon, "--warn-return-type"};
      Core::Warning WarnReturnTypeExt{&Core::WarnExtra, "--warn-extra-return-type"};
      Core::Warning WarnUnusedInit{&Core::WarnCommon, "--warn-unused-initializer"};
      Core::Warning WarnUnusedValue{&Core::WarnCommon, "--warn-unused-value"};
   }
}

// EOF

