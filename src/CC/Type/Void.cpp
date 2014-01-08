//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C void type.
//
//-----------------------------------------------------------------------------

#include "CC/Type/Void.hpp"

#include "Bytecode/Platform.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      GDCC_CC_Type_ImplGet(Void, Void,)

      //
      // Type_Void::getIRType
      //
      IR::Type Type_Void::getIRType() const
      {
         return IR::Type_Empty();
      }
   }
}

// EOF

