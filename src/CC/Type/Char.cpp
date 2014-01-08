//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C char type.
//
//-----------------------------------------------------------------------------

#include "CC/Type/Char.hpp"

#include "Bytecode/Platform.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      GDCC_CC_Type_ImplGet(Char, Char,)

      //
      // Type_Char::getIRType
      //
      IR::Type Type_Char::getIRType() const
      {
         return IR::Type_Fixed(getSizeBitsI(), getSizeBitsF(), getSizeBitsS(),
            isTypeSaturate());
      }

      //
      // Type_Char::getSizeBitsI
      //
      Core::FastU Type_Char::getSizeBitsI() const
      {
         return Bytecode::GetByteBitsI() - getSizeBitsS();
      }

      //
      // Type_Char::getSizeBitsS
      //
      bool Type_Char::getSizeBitsS() const
      {
         switch(Bytecode::TargetCur)
         {
         case Bytecode::Target::MageCraft: return false;
         default:                          return true;
         }
      }
   }
}

// EOF

