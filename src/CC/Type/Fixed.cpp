//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C fixed-point types.
//
//-----------------------------------------------------------------------------

#include "CC/Type/Fixed.hpp"

#include "IR/Type.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

namespace GDCC::CC
{
   // Table: Type_Fixed::getBaseType
   static SR::Type::CRef const FixedTable_BaseType[6] =
   {
      GetTypeFixedPrUH(),
      GetTypeFixedPrSH(),
      GetTypeFixedPrU(),
      GetTypeFixedPrS(),
      GetTypeFixedPrUL(),
      GetTypeFixedPrSL(),
   };

   // Table: Type_Fract::getBaseType
   static SR::Type::CRef const FractTable_BaseType[6] =
   {
      GetTypeFractPrUH(),
      GetTypeFractPrSH(),
      GetTypeFractPrU(),
      GetTypeFractPrS(),
      GetTypeFractPrUL(),
      GetTypeFractPrSL(),
   };

   // Table: Type_Integ::getBaseType
   static SR::Type::CRef const IntegTable_BaseType[10] =
   {
      GetTypeIntegPrUHH(),
      GetTypeIntegPrSHH(),
      GetTypeIntegPrUH(),
      GetTypeIntegPrSH(),
      GetTypeIntegPrU(),
      GetTypeIntegPrS(),
      GetTypeIntegPrUL(),
      GetTypeIntegPrSL(),
      GetTypeIntegPrULL(),
      GetTypeIntegPrSLL(),
   };

   // Table: Type_Integ::getSizeBitsI ZDACS
   static constexpr Core::FastU IntegTable_SizeBitsI_ZDACS[5] =
      {32, 32, 32, 64, 96};

   // Table: Type_Integ::getSizeBytes ZDACS
   static constexpr Core::FastU IntegTable_SizeBytes_ZDACS[5] =
      {1, 1, 1, 2, 3};
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   GDCC_CC_Type_ImplGet(FixedPrS,  Fixed, 1, true,  true )
   GDCC_CC_Type_ImplGet(FixedPrSH, Fixed, 0, true,  true )
   GDCC_CC_Type_ImplGet(FixedPrSL, Fixed, 2, true,  true )
   GDCC_CC_Type_ImplGet(FixedPrU,  Fixed, 1, true,  false)
   GDCC_CC_Type_ImplGet(FixedPrUH, Fixed, 0, true,  false)
   GDCC_CC_Type_ImplGet(FixedPrUL, Fixed, 2, true,  false)
   GDCC_CC_Type_ImplGet(FixedSaS,  Fixed, 1, false, true )
   GDCC_CC_Type_ImplGet(FixedSaSH, Fixed, 0, false, true )
   GDCC_CC_Type_ImplGet(FixedSaSL, Fixed, 2, false, true )
   GDCC_CC_Type_ImplGet(FixedSaU,  Fixed, 1, false, false)
   GDCC_CC_Type_ImplGet(FixedSaUH, Fixed, 0, false, false)
   GDCC_CC_Type_ImplGet(FixedSaUL, Fixed, 2, false, false)

   GDCC_CC_Type_ImplGet(FractPrS,  Fract, 1, true,  true )
   GDCC_CC_Type_ImplGet(FractPrSH, Fract, 0, true,  true )
   GDCC_CC_Type_ImplGet(FractPrSL, Fract, 2, true,  true )
   GDCC_CC_Type_ImplGet(FractPrU,  Fract, 1, true,  false)
   GDCC_CC_Type_ImplGet(FractPrUH, Fract, 0, true,  false)
   GDCC_CC_Type_ImplGet(FractPrUL, Fract, 2, true,  false)
   GDCC_CC_Type_ImplGet(FractSaS,  Fract, 1, false, true )
   GDCC_CC_Type_ImplGet(FractSaSH, Fract, 0, false, true )
   GDCC_CC_Type_ImplGet(FractSaSL, Fract, 2, false, true )
   GDCC_CC_Type_ImplGet(FractSaU,  Fract, 1, false, false)
   GDCC_CC_Type_ImplGet(FractSaUH, Fract, 0, false, false)
   GDCC_CC_Type_ImplGet(FractSaUL, Fract, 2, false, false)

   GDCC_CC_Type_ImplGet(IntegPrS,   Integ, 2, true,  true )
   GDCC_CC_Type_ImplGet(IntegPrSH,  Integ, 1, true,  true )
   GDCC_CC_Type_ImplGet(IntegPrSHH, Integ, 0, true,  true )
   GDCC_CC_Type_ImplGet(IntegPrSL,  Integ, 3, true,  true )
   GDCC_CC_Type_ImplGet(IntegPrSLL, Integ, 4, true,  true )
   GDCC_CC_Type_ImplGet(IntegPrU,   Integ, 2, true,  false)
   GDCC_CC_Type_ImplGet(IntegPrUH,  Integ, 1, true,  false)
   GDCC_CC_Type_ImplGet(IntegPrUHH, Integ, 0, true,  false)
   GDCC_CC_Type_ImplGet(IntegPrUL,  Integ, 3, true,  false)
   GDCC_CC_Type_ImplGet(IntegPrULL, Integ, 4, true,  false)

   //
   // Type_FixedBase constructor
   //
   Type_FixedBase::Type_FixedBase(unsigned size_, bool prim_, bool sign_) :
      size{size_},
      prim{prim_},
      satu{!prim_},
      sign{sign_},
      unsi{!sign_}
   {
   }

   //
   // Type_FixedBase::getIRType
   //
   IR::Type Type_FixedBase::getIRType() const
   {
      return IR::Type_Fixed(getSizeBitsI(), getSizeBitsF(), getSizeBitsS(),
         isTypeSaturate());
   }

   //
   // Type_Fixed constructor
   //
   Type_Fixed::Type_Fixed(unsigned size_, bool prim_, bool sign_) :
      Super{size_, prim_, sign_}
   {
   }

   //
   // Type_Fixed::getBaseType
   //
   SR::Type::CRef Type_Fixed::getBaseType() const
   {
      if(prim) throw SR::TypeError();
      return FixedTable_BaseType[(size << 1) | sign];
   }

   //
   // Type_Fixed::getRankC
   //
   SR::TypeRankC Type_Fixed::getRankC() const
   {
      switch(size)
      {
      case 0: return SR::TypeRankC::FixedH;
      case 1: return SR::TypeRankC::Fixed;
      case 2: return SR::TypeRankC::FixedL;
      }

      return SR::TypeRankC::None;
   }

   //
   // Type_Fixed::getSignType
   //
   SR::Type::CRef Type_Fixed::getSignType() const
   {
      return FixedTable_BaseType[(size << 1) | 1];
   }

   //
   // Type_Fixed::getSizeAlign
   //
   Core::FastU Type_Fixed::getSizeAlign() const
   {
      return Platform::GetWordAlign();
   }

   //
   // Type_Fixed::getSizeBitsF
   //
   Core::FastU Type_Fixed::getSizeBitsF() const
   {
      if(Platform::IsFamily_ZDACS())
         return size <= 1 ? 16 : 32;

      return size <= 1 ? (8 << size) - 1 : 32;
   }

   //
   // Type_Fixed::getSizeBitsI
   //
   Core::FastU Type_Fixed::getSizeBitsI() const
   {
      if(Platform::IsFamily_ZDACS())
         return (size <= 1 ? 15 : 31) + unsi;

      return (size <= 1 ? (24 << size) : 63) + unsi;
   }

   //
   // Type_Fixed::getSizeBytes
   //
   Core::FastU Type_Fixed::getSizeBytes() const
   {
      if(Platform::IsFamily_ZDACS())
         return size <= 1 ? 1 : 2;

      return (size + 1) * Platform::GetWordBytes();
   }

   //
   // Type_Fixed::getSizePoint
   //
   Core::FastU Type_Fixed::getSizePoint() const
   {
      if(Platform::IsFamily_ZDACS())
         return size <= 1 ? 1 : 2;

      return (size + 1) * Platform::GetWordPoint();
   }

   //
   // Type_Fixed::getSizeShift
   //
   Core::FastU Type_Fixed::getSizeShift() const
   {
      return 1;
   }

   //
   // Type_Fixed::getSizeWords
   //
   Core::FastU Type_Fixed::getSizeWords() const
   {
      if(Platform::IsFamily_ZDACS())
         return size <= 1 ? 1 : 2;

      return size + 1;
   }

   //
   // Type_Fixed::getUnsiType
   //
   SR::Type::CRef Type_Fixed::getUnsiType() const
   {
      return FixedTable_BaseType[(size << 1) | 0];
   }

   //
   // Type_Fract constructor
   //
   Type_Fract::Type_Fract(unsigned size_, bool prim_, bool sign_) :
      Super{size_, prim_, sign_}
   {
   }

   //
   // Type_Fract::getBaseType
   //
   SR::Type::CRef Type_Fract::getBaseType() const
   {
      if(prim) throw SR::TypeError();
      return FractTable_BaseType[(size << 1) | sign];
   }

   //
   // Type_Fract::getRankC
   //
   SR::TypeRankC Type_Fract::getRankC() const
   {
      switch(size)
      {
      case 0: return SR::TypeRankC::FractH;
      case 1: return SR::TypeRankC::Fract;
      case 2: return SR::TypeRankC::FractL;
      }

      return SR::TypeRankC::None;
   }

   //
   // Type_Fract::getSignType
   //
   SR::Type::CRef Type_Fract::getSignType() const
   {
      return FractTable_BaseType[(size << 1) | 1];
   }

   //
   // Type_Fract::getSizeAlign
   //
   Core::FastU Type_Fract::getSizeAlign() const
   {
      return Platform::GetWordAlign();
   }

   //
   // Type_Fract::getSizeBitsF
   //
   Core::FastU Type_Fract::getSizeBitsF() const
   {
      return (8 << size) - sign;
   }

   //
   // Type_Fract::getSizeBytes
   //
   Core::FastU Type_Fract::getSizeBytes() const
   {
      if(Platform::IsFamily_ZDACS())
         return 1;

      return 1 << size;
   }

   //
   // Type_Fract::getSizePoint
   //
   Core::FastU Type_Fract::getSizePoint() const
   {
      if(Platform::IsFamily_ZDACS())
         return 1;

      return 1 << size;
   }

   //
   // Type_Fract::getSizeShift
   //
   Core::FastU Type_Fract::getSizeShift() const
   {
      return 1;
   }

   //
   // Type_Fract::getSizeWords
   //
   Core::FastU Type_Fract::getSizeWords() const
   {
      return 1;
   }

   //
   // Type_Fract::getUnsiType
   //
   SR::Type::CRef Type_Fract::getUnsiType() const
   {
      return FractTable_BaseType[(size << 1) | 0];
   }

   //
   // Type_Fract::isTypeSubWord
   //
   bool Type_Fract::isTypeSubWord() const
   {
      if(Platform::IsFamily_ZDACS())
         return false;

      return size < 2;
   }

   //
   // Type_Integ constructor
   //
   Type_Integ::Type_Integ(unsigned size_, bool prim_, bool sign_) :
      Super{size_, prim_, sign_}
   {
   }

   //
   // Type_Integ::getRankC
   //
   SR::TypeRankC Type_Integ::getRankC() const
   {
      switch(size)
      {
      case 0: return SR::TypeRankC::IntegHH;
      case 1: return SR::TypeRankC::IntegH;
      case 2: return SR::TypeRankC::Integ;
      case 3: return SR::TypeRankC::IntegL;
      case 4: return SR::TypeRankC::IntegLL;
      }

      return SR::TypeRankC::None;
   }

   //
   // Type_Integ::getSignType
   //
   SR::Type::CRef Type_Integ::getSignType() const
   {
      return IntegTable_BaseType[(size << 1) | 1];
   }

   //
   // Type_Integ::getSizeAlign
   //
   Core::FastU Type_Integ::getSizeAlign() const
   {
      if(Platform::IsFamily_ZDACS())
         return 1;

      return size <= 1 ? 1 << size : Platform::GetWordAlign();
   }

   //
   // Type_Integ::getSizeBitsI
   //
   Core::FastU Type_Integ::getSizeBitsI() const
   {
      if(Platform::IsFamily_ZDACS())
         return IntegTable_SizeBitsI_ZDACS[size] - sign;

      return (size <= 3 ? 8 << size : 96) - sign;
   }

   //
   // Type_Integ::getSizeBytes
   //
   Core::FastU Type_Integ::getSizeBytes() const
   {
      if(Platform::IsFamily_ZDACS())
         return IntegTable_SizeBytes_ZDACS[size];

      return size <= 3 ? 1 << size : 12;
   }

   //
   // Type_Integ::getSizePoint
   //
   Core::FastU Type_Integ::getSizePoint() const
   {
      if(Platform::IsFamily_ZDACS())
         return IntegTable_SizeBytes_ZDACS[size];

      return size <= 3 ? 1 << size : 12;
   }

   //
   // Type_Integ::getSizeShift
   //
   Core::FastU Type_Integ::getSizeShift() const
   {
      return 1;
   }

   //
   // Type_Integ::getSizeWords
   //
   Core::FastU Type_Integ::getSizeWords() const
   {
      if(Platform::IsFamily_ZDACS())
         return IntegTable_SizeBytes_ZDACS[size] ;

      return size <= 2 ? 1 : size - 1;
   }

   //
   // Type_Integ::getUnsiType
   //
   SR::Type::CRef Type_Integ::getUnsiType() const
   {
      return IntegTable_BaseType[(size << 1) | 0];
   }

   //
   // Type_Integ::isTypeSubWord
   //
   bool Type_Integ::isTypeSubWord() const
   {
      if(Platform::IsFamily_ZDACS())
         return false;

      return size < 2;
   }
}

// EOF

