//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../../AST/Type.hpp"


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
      class Type_FixedBase : public AST::Type
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::CC::Type_FixedBase, GDCC::AST::Type);

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
         virtual TypeRankC   getRankC()     const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const;
         virtual Core::FastU getSizeBitsI() const;
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;

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


         friend AST::Type::CRef GetTypeFixedPrS();
         friend AST::Type::CRef GetTypeFixedPrSH();
         friend AST::Type::CRef GetTypeFixedPrSL();
         friend AST::Type::CRef GetTypeFixedPrU();
         friend AST::Type::CRef GetTypeFixedPrUH();
         friend AST::Type::CRef GetTypeFixedPrUL();

         friend AST::Type::CRef GetTypeFixedSaS();
         friend AST::Type::CRef GetTypeFixedSaSH();
         friend AST::Type::CRef GetTypeFixedSaSL();
         friend AST::Type::CRef GetTypeFixedSaU();
         friend AST::Type::CRef GetTypeFixedSaUH();
         friend AST::Type::CRef GetTypeFixedSaUL();

      protected:
         Type_Fixed(unsigned size, bool prim, bool sign);
      };

      //
      // Type_Fract
      //
      class Type_Fract final : public Type_FixedBase
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Fract,
            GDCC::CC::Type_FixedBase);

      public:
         // Type information.
         virtual Type::CRef  getBaseType()  const;
         virtual TypeRankC   getRankC()     const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const;
         virtual Core::FastU getSizeBitsI() const {return 0;}
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;

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


         friend AST::Type::CRef GetTypeFractPrS();
         friend AST::Type::CRef GetTypeFractPrSH();
         friend AST::Type::CRef GetTypeFractPrSL();
         friend AST::Type::CRef GetTypeFractPrU();
         friend AST::Type::CRef GetTypeFractPrUH();
         friend AST::Type::CRef GetTypeFractPrUL();

         friend AST::Type::CRef GetTypeFractSaS();
         friend AST::Type::CRef GetTypeFractSaSH();
         friend AST::Type::CRef GetTypeFractSaSL();
         friend AST::Type::CRef GetTypeFractSaU();
         friend AST::Type::CRef GetTypeFractSaUH();
         friend AST::Type::CRef GetTypeFractSaUL();

      protected:
         Type_Fract(unsigned size, bool prim, bool sign);
      };

      //
      // Type_Integ
      //
      class Type_Integ final : public Type_FixedBase
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Integ,
            GDCC::CC::Type_FixedBase);

      public:
         // Type information.
         virtual TypeRankC   getRankC()     const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const {return 0;}
         virtual Core::FastU getSizeBitsI() const;
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;

         // Type classification: C/C++ classifications.
         virtual bool isCTypeChar()   const {return size == 0;}
         virtual bool isCTypeInteg()  const {return true;}
         virtual bool isCTypeIntegS() const {return sign;}
         virtual bool isCTypeIntegU() const {return unsi;}


         friend AST::Type::CRef GetTypeIntegPrS();
         friend AST::Type::CRef GetTypeIntegPrSH();
         friend AST::Type::CRef GetTypeIntegPrSHH();
         friend AST::Type::CRef GetTypeIntegPrSL();
         friend AST::Type::CRef GetTypeIntegPrSLL();
         friend AST::Type::CRef GetTypeIntegPrU();
         friend AST::Type::CRef GetTypeIntegPrUH();
         friend AST::Type::CRef GetTypeIntegPrUHH();
         friend AST::Type::CRef GetTypeIntegPrUL();
         friend AST::Type::CRef GetTypeIntegPrULL();

      protected:
         Type_Integ(unsigned size, bool prim, bool sign);
      };
   }
}

#endif//GDCC__CC__Type__Fixed_H__

