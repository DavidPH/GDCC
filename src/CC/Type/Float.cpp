//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C floating-point types.
//
//-----------------------------------------------------------------------------

#include "CC/Type/Float.hpp"

#include "IR/Type.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

namespace GDCC
{
   namespace CC
   {
      // Table: Type_Float::getBaseType
      static SR::Type::CRef const FloatTable_BaseType[4] =
      {
         GetTypeFloatRS(), // FIXME
         GetTypeFloatRS(),
         GetTypeFloatRSL(),
         GetTypeFloatRSLL(),
      };

      // Table: Type_Float::getBias
      static constexpr Core::FastU FloatTable_Bias[4] =
         {0xF, 0x7F, 0x3FF, 0x3FF};

      // Table: Type_Float::getSizeBitsF
      static constexpr Core::FastU FloatTable_SizeBitsF[4] = {5, 8, 11, 11};

      // Table: Type_Float::getSizeBitsI
      static constexpr Core::FastU FloatTable_SizeBitsI[4] = {10, 23, 52, 52};

      // Table: Type_Float::getSizeBytes 8-bit byte
      static constexpr Core::FastU FloatTable_SizeBytes_B8[8] =
         {2, 4, 4, 8, 8, 16, 8, 16};

      // Table: Type_Float::getSizeBytes ZDACS
      static constexpr Core::FastU FloatTable_SizeBytes_ZDACS[8] =
         {1, 1, 1, 2, 2, 4, 2, 4};

      // Table: Type_Float::getSizePoint 8-bit byte
      static constexpr Core::FastU FloatTable_SizePoint_B8[8] =
         {2, 4, 4, 8, 8, 16, 8, 16};

      // Table: Type_Float::getSizeWords 8-bit byte
      static constexpr Core::FastU FloatTable_SizeWords_B8[8] =
         {1, 1, 1, 2, 2, 4, 2, 4};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      GDCC_CC_Type_ImplGet(FloatCS,   Float, 1, true, false, true)
      GDCC_CC_Type_ImplGet(FloatCSL,  Float, 2, true, false, true)
      GDCC_CC_Type_ImplGet(FloatCSLL, Float, 3, true, false, true)

      GDCC_CC_Type_ImplGet(FloatIS,   Float, 1, false, true, true)
      GDCC_CC_Type_ImplGet(FloatISL,  Float, 2, false, true, true)
      GDCC_CC_Type_ImplGet(FloatISLL, Float, 3, false, true, true)

      GDCC_CC_Type_ImplGet(FloatRS,   Float, 1, false, false, true)
      GDCC_CC_Type_ImplGet(FloatRSL,  Float, 2, false, false, true)
      GDCC_CC_Type_ImplGet(FloatRSLL, Float, 3, false, false, true)

      //
      // Type_Float constructor
      //
      Type_Float::Type_Float(unsigned size_, bool cplx_, bool imag_, bool sign_) :
         size{size_},
         cplx{cplx_},
         imag{imag_},
         real{!cplx_ && !imag_},
         sign{sign_},
         unsi{!sign_}
      {
      }

      //
      // Type_Float::getBaseType
      //
      SR::Type::CRef Type_Float::getBaseType() const
      {
         if(real) throw SR::TypeError();
         return FloatTable_BaseType[size];
      }

      //
      // Type_Float::getBias
      //
      Core::FastU Type_Float::getBias() const
      {
         return FloatTable_Bias[size];
      }

      //
      // Type_Float::getIRType
      //
      IR::Type Type_Float::getIRType() const
      {
         return IR::Type_Float(getSizeBitsI(), getSizeBitsF(), getSizeBitsS(),
            isTypeSaturate());
      }

      //
      // Type_Float::getSizeAlign
      //
      Core::FastU Type_Float::getSizeAlign() const
      {
         if(Platform::IsFamily_ZDACS())
            return 1;

         return !size && !cplx ? 2 : 4;
      }

      //
      // Type_Float::getSizeBitsF
      //
      Core::FastU Type_Float::getSizeBitsF() const
      {
         return FloatTable_SizeBitsF[size];
      }

      //
      // Type_Float::getSizeBitsI
      //
      Core::FastU Type_Float::getSizeBitsI() const
      {
         return FloatTable_SizeBitsI[size];
      }

      //
      // Type_Float::getSizeBytes
      //
      Core::FastU Type_Float::getSizeBytes() const
      {
         if(Platform::IsFamily_ZDACS())
            return FloatTable_SizeBytes_ZDACS[(size << 1) | cplx];

         return FloatTable_SizeBytes_B8[(size << 1) | cplx];
      }

      //
      // Type_Float::getSizePoint
      //
      Core::FastU Type_Float::getSizePoint() const
      {
         if(Platform::IsFamily_ZDACS())
            return FloatTable_SizeBytes_ZDACS[(size << 1) | cplx];

         return FloatTable_SizePoint_B8[(size << 1) | cplx];
      }

      //
      // Type_Float::getSizeShift
      //
      Core::FastU Type_Float::getSizeShift() const
      {
         return 1;
      }

      //
      // Type_Float::getSizeWords
      //
      Core::FastU Type_Float::getSizeWords() const
      {
         if(Platform::IsFamily_ZDACS())
            return FloatTable_SizeBytes_ZDACS[(size << 1) | cplx];

         return FloatTable_SizeWords_B8[(size << 1) | cplx];
      }
   }
}

// EOF

