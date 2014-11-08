//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C floating-point types.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Float_H__
#define GDCC__CC__Type__Float_H__

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
      // Type_Float
      //
      class Type_Float final : public AST::Type
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Float, GDCC::AST::Type);

      public:
         // Type information.
         virtual Core::FastU getBias()      const;
         virtual Type::CRef  getBaseType()  const;
         virtual IR::Type    getIRType()    const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const;
         virtual Core::FastU getSizeBitsI() const;
         virtual bool        getSizeBitsS() const {return sign;}
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;

         // Type classification: General classifications.
         virtual bool isTypeComplete() const {return true;}
         virtual bool isTypeComplex()  const {return cplx;}
         virtual bool isTypeImagin()   const {return imag;}
         virtual bool isTypeSaturate() const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeArith()   const {return true;}
         virtual bool isCTypeComplex() const {return cplx;}
         virtual bool isCTypeFloat()   const {return true;}
         virtual bool isCTypeImagin()  const {return imag;}
         virtual bool isCTypeObject()  const {return true;}
         virtual bool isCTypeReal()    const {return real;}
         virtual bool isCTypeRealFlt() const {return real;}
         virtual bool isCTypeScalar()  const {return true;}


         friend AST::Type::CRef GetTypeFloatCS();
         friend AST::Type::CRef GetTypeFloatCSL();
         friend AST::Type::CRef GetTypeFloatCSLL();

         friend AST::Type::CRef GetTypeFloatIS();
         friend AST::Type::CRef GetTypeFloatISL();
         friend AST::Type::CRef GetTypeFloatISLL();

         friend AST::Type::CRef GetTypeFloatRS();
         friend AST::Type::CRef GetTypeFloatRSL();
         friend AST::Type::CRef GetTypeFloatRSLL();

      protected:
         Type_Float(unsigned size, bool cplx, bool imag, bool sign);

         unsigned const size : 2;
         bool     const cplx : 1;
         bool     const imag : 1;
         bool     const real : 1;
         bool     const sign : 1;
         bool     const unsi : 1;
      };
   }
}

#endif//GDCC__CC__Type__Float_H__

