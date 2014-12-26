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

#include "IR/Type.hpp"

#include "Platform/Platform.hpp"


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
      // Type_Bool::getRankC
      //
      AST::TypeRankC Type_Bool::getRankC() const
      {
         return AST::TypeRankC::Bool;
      }

      //
      // Type_BoolSoft::getSizeAlign
      //
      Core::FastU Type_BoolSoft::getSizeAlign() const
      {
         return Platform::GetWordAlign();
      }

      //
      // Type_BoolSoft::getSizeBytes
      //
      Core::FastU Type_BoolSoft::getSizeBytes() const
      {
         return Platform::GetWordBytes();
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
         return Platform::GetWordShift();
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

