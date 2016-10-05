//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C char type.
//
//-----------------------------------------------------------------------------

#include "CC/Type/Char.hpp"

#include "IR/Type.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
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
      // Type_Char::getRankC
      //
      SR::TypeRankC Type_Char::getRankC() const
      {
         return SR::TypeRankC::IntegHH;
      }

      //
      // Type_Char::getSizeBitsI
      //
      Core::FastU Type_Char::getSizeBitsI() const
      {
         return Platform::GetByteBitsI() - getSizeBitsS();
      }

      //
      // Type_Char::getSizeBitsS
      //
      bool Type_Char::getSizeBitsS() const
      {
         if(Platform::IsFamily_ZDACS())
            return true;

         return false;
      }

      //
      // Type_Char::isTypeSubWord
      //
      bool Type_Char::isTypeSubWord() const
      {
         if(Platform::IsFamily_ZDACS())
            return false;

         return true;
      }
   }
}

// EOF

