//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "../../SR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Type_Float
   //
   class Type_Float final : public SR::Type
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Type_Float, GDCC::SR::Type);

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


      friend SR::Type::CRef GetTypeFloatCS();
      friend SR::Type::CRef GetTypeFloatCSL();
      friend SR::Type::CRef GetTypeFloatCSLL();

      friend SR::Type::CRef GetTypeFloatIS();
      friend SR::Type::CRef GetTypeFloatISL();
      friend SR::Type::CRef GetTypeFloatISLL();

      friend SR::Type::CRef GetTypeFloatRS();
      friend SR::Type::CRef GetTypeFloatRSL();
      friend SR::Type::CRef GetTypeFloatRSLL();

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

#endif//GDCC__CC__Type__Float_H__

