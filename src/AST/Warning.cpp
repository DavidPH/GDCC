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
            .setDescS("Warns on use of deprecated entities."),

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
            .setDescL("Warns on suspicious return type usage, such as have no "
               "return statement in a non-void function."),

         &WarnReturnType
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
   }
}

// EOF

