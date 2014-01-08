//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C _Bool type.
//
//-----------------------------------------------------------------------------

#include "CC/Type/Bool.hpp"

#include "Bytecode/Platform.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      GDCC_CC_Type_ImplGet(Bool, Bool,)

      //
      // Type_Bool::getIRType
      //
      IR::Type Type_Bool::getIRType() const
      {
         return IR::Type_Fixed(getSizeBitsI(), getSizeBitsF(), getSizeBitsS(),
            isTypeSaturate());
      }
   }
}

// EOF

