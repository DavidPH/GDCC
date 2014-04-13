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
      GDCC_CC_Type_ImplGet(Bool,     Bool,)
      GDCC_CC_Type_ImplGet(BoolSoft, BoolSoft,)

      //
      // Type_Bool::getIRType
      //
      IR::Type Type_Bool::getIRType() const
      {
         return IR::Type_Fixed(getSizeBitsI(), getSizeBitsF(), getSizeBitsS(),
            isTypeSaturate());
      }

      //
      // Type_BoolSoft::getSizeAlign
      //
      Core::FastU Type_BoolSoft::getSizeAlign() const
      {
         return Bytecode::GetWordAlign();
      }

      //
      // Type_BoolSoft::getSizeBytes
      //
      Core::FastU Type_BoolSoft::getSizeBytes() const
      {
         return Bytecode::GetWordBytes();
      }

      //
      // Type_BoolSoft::getSizePoint
      //
      Core::FastU Type_BoolSoft::getSizePoint() const
      {
         return 1;
      }

      //
      // Type_BoolSoft::getSizeShift
      //
      Core::FastU Type_BoolSoft::getSizeShift() const
      {
         return Bytecode::GetWordShift();
      }

      //
      // Type_BoolSoft::getSizeWords
      //
      Core::FastU Type_BoolSoft::getSizeWords() const
      {
         return 1;
      }
   }
}

// EOF

