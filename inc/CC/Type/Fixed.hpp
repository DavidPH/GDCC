//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C fixed-point types.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Fixed_H__
#define GDCC__CC__Type__Fixed_H__

#include "../../CC/Type.hpp"

#include "../../SR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Type_FixedBase
      //
      class Type_FixedBase : public SR::Type
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::CC::Type_FixedBase, GDCC::SR::Type);

      public:
         // Type information.
         virtual IR::Type getIRType()    const;
         virtual bool     getSizeBitsS() const {return sign;}

         // Type classification: General classifications.
         virtual bool isTypeComplete() const {return true;}
         virtual bool isTypeSaturate() const {return satu;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeArith()  const {return true;}
         virtual bool isCTypeObject() const {return true;}
         virtual bool isCTypeReal()   const {return true;}
         virtual bool isCTypeScalar() const {return true;}

      protected:
         Type_FixedBase(unsigned size, bool prim, bool sign);

         unsigned const size : 3;
         bool     const prim : 1;
         bool     const satu : 1;
         bool     const sign : 1;
         bool     const unsi : 1;
      };

      //
      // Type_Fixed
      //
      class Type_Fixed final : public Type_FixedBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Type_Fixed, GDCC::CC::Type_FixedBase);

      public:
         // Type information.
         virtual Type::CRef  getBaseType()  const;
         virtual RankC       getRankC()     const;
         virtual Type::CRef  getSignType()  const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const;
         virtual Core::FastU getSizeBitsI() const;
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;
         virtual Type::CRef  getUnsiType()  const;

         // Type classification: C/C++ classifications.
         virtual bool isCTypeAccum()    const {return true;}
         virtual bool isCTypeAccumS()   const {return sign;}
         virtual bool isCTypeAccumU()   const {return unsi;}
         virtual bool isCTypeFixed()    const {return true;}
         virtual bool isCTypeFixedPr()  const {return prim;}
         virtual bool isCTypeFixedPrS() const {return prim && sign;}
         virtual bool isCTypeFixedPrU() const {return prim && unsi;}
         virtual bool isCTypeFixedSa()  const {return satu;}
         virtual bool isCTypeFixedSaS() const {return satu && sign;}
         virtual bool isCTypeFixedSaU() const {return satu && unsi;}


         friend SR::Type::CRef GetTypeFixedPrS();
         friend SR::Type::CRef GetTypeFixedPrSH();
         friend SR::Type::CRef GetTypeFixedPrSL();
         friend SR::Type::CRef GetTypeFixedPrU();
         friend SR::Type::CRef GetTypeFixedPrUH();
         friend SR::Type::CRef GetTypeFixedPrUL();

         friend SR::Type::CRef GetTypeFixedSaS();
         friend SR::Type::CRef GetTypeFixedSaSH();
         friend SR::Type::CRef GetTypeFixedSaSL();
         friend SR::Type::CRef GetTypeFixedSaU();
         friend SR::Type::CRef GetTypeFixedSaUH();
         friend SR::Type::CRef GetTypeFixedSaUL();

      protected:
         Type_Fixed(unsigned size, bool prim, bool sign);
      };

      //
      // Type_Fract
      //
      class Type_Fract final : public Type_FixedBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Type_Fract, GDCC::CC::Type_FixedBase);

      public:
         // Type information.
         virtual Type::CRef  getBaseType()  const;
         virtual RankC       getRankC()     const;
         virtual Type::CRef  getSignType()  const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const;
         virtual Core::FastU getSizeBitsI() const {return 0;}
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;
         virtual Type::CRef  getUnsiType()  const;

         // Type classification: General classifications.
         virtual bool isTypeSubWord() const;

         // Type classification: C/C++ classifications.
         virtual bool isCTypeFixed()    const {return true;}
         virtual bool isCTypeFixedPr()  const {return prim;}
         virtual bool isCTypeFixedPrS() const {return prim && sign;}
         virtual bool isCTypeFixedPrU() const {return prim && unsi;}
         virtual bool isCTypeFixedSa()  const {return satu;}
         virtual bool isCTypeFixedSaS() const {return satu && sign;}
         virtual bool isCTypeFixedSaU() const {return satu && unsi;}
         virtual bool isCTypeFract()    const {return true;}
         virtual bool isCTypeFractS()   const {return sign;}
         virtual bool isCTypeFractU()   const {return unsi;}


         friend SR::Type::CRef GetTypeFractPrS();
         friend SR::Type::CRef GetTypeFractPrSH();
         friend SR::Type::CRef GetTypeFractPrSL();
         friend SR::Type::CRef GetTypeFractPrU();
         friend SR::Type::CRef GetTypeFractPrUH();
         friend SR::Type::CRef GetTypeFractPrUL();

         friend SR::Type::CRef GetTypeFractSaS();
         friend SR::Type::CRef GetTypeFractSaSH();
         friend SR::Type::CRef GetTypeFractSaSL();
         friend SR::Type::CRef GetTypeFractSaU();
         friend SR::Type::CRef GetTypeFractSaUH();
         friend SR::Type::CRef GetTypeFractSaUL();

      protected:
         Type_Fract(unsigned size, bool prim, bool sign);
      };

      //
      // Type_Integ
      //
      class Type_Integ final : public Type_FixedBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Type_Integ, GDCC::CC::Type_FixedBase);

      public:
         // Type information.
         virtual RankC       getRankC()     const;
         virtual Type::CRef  getSignType()  const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const {return 0;}
         virtual Core::FastU getSizeBitsI() const;
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;
         virtual Type::CRef  getUnsiType()  const;

         // Type classification: General classifications.
         virtual bool isTypeSubWord() const;

         // Type classification: C/C++ classifications.
         virtual bool isCTypeChar()   const {return size == 0;}
         virtual bool isCTypeInteg()  const {return true;}
         virtual bool isCTypeIntegS() const {return sign;}
         virtual bool isCTypeIntegU() const {return unsi;}


         friend SR::Type::CRef GetTypeIntegPrS();
         friend SR::Type::CRef GetTypeIntegPrSH();
         friend SR::Type::CRef GetTypeIntegPrSHH();
         friend SR::Type::CRef GetTypeIntegPrSL();
         friend SR::Type::CRef GetTypeIntegPrSLL();
         friend SR::Type::CRef GetTypeIntegPrU();
         friend SR::Type::CRef GetTypeIntegPrUH();
         friend SR::Type::CRef GetTypeIntegPrUHH();
         friend SR::Type::CRef GetTypeIntegPrUL();
         friend SR::Type::CRef GetTypeIntegPrULL();

      protected:
         Type_Integ(unsigned size, bool prim, bool sign);
      };
   }
}

#endif//GDCC__CC__Type__Fixed_H__

