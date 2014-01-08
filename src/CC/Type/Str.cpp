//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C __str type.
//
//-----------------------------------------------------------------------------

#include "CC/Type/Str.hpp"

#include "Bytecode/Platform.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      GDCC_CC_Type_ImplGet(Str, Str,)

      //
      // Type_Str::getIRType
      //
      IR::Type Type_Str::getIRType() const
      {
         return IR::Type_StrEn();
      }

      //
      // Type_Str::getSizeAlign
      //
      Core::FastU Type_Str::getSizeAlign() const
      {
         return Bytecode::GetWordAlign();
      }

      //
      // Type_Str::getSizeBytes
      //
      Core::FastU Type_Str::getSizeBytes() const
      {
         return Bytecode::GetWordBytes();
      }

      //
      // Type_Str::getSizePoint
      //
      Core::FastU Type_Str::getSizePoint() const
      {
         return Bytecode::GetWordPoint();
      }

      //
      // Type_Str::getSizeShift
      //
      Core::FastU Type_Str::getSizeShift() const
      {
         return Bytecode::GetWordShift();
      }
   }
}

// EOF

