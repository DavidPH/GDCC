//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C _Bool and __fastbool type.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Bool_H__
#define GDCC__CC__Type__Bool_H__

#include "../../CC/Type.hpp"

#include "../../SR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Type_Bool
   //
   class Type_Bool final : public SR::Type
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Type_Bool, GDCC::SR::Type);

   public:
      // Type information.
      virtual IR::Type    getIRType()    const;
      virtual RankC       getRankC()     const;
      virtual Core::FastU getSizeAlign() const {return 1;}
      virtual Core::FastU getSizeBitsF() const {return 0;}
      virtual Core::FastU getSizeBitsI() const {return 1;}
      virtual Core::FastU getSizeBitsO() const {return 0;}
      virtual bool        getSizeBitsS() const {return 0;}
      virtual Core::FastU getSizeBytes() const {return 1;}
      virtual Core::FastU getSizePoint() const {return 1;}
      virtual Core::FastU getSizeShift() const {return 1;}
      virtual Core::FastU getSizeWords() const {return 1;}

      // Type classification: General classifications.
      virtual bool isTypeBoolean()  const {return true;}
      virtual bool isTypeComplete() const {return true;}

      // Type classification: C/C++ classifications.
      virtual bool isCTypeArith()  const {return true;}
      virtual bool isCTypeInteg()  const {return true;}
      virtual bool isCTypeIntegU() const {return true;}
      virtual bool isCTypeObject() const {return true;}
      virtual bool isCTypeReal()   const {return true;}
      virtual bool isCTypeScalar() const {return true;}


      friend SR::Type::CRef GetTypeBool();

   protected:
      Type_Bool() = default;
   };

   //
   // Type_BoolSoft
   //
   class Type_BoolSoft final : public SR::Type
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Type_BoolSoft, GDCC::SR::Type);

   public:
      // Type information.
      virtual Core::FastU getSizeAlign() const;
      virtual Core::FastU getSizeBytes() const;
      virtual Core::FastU getSizePoint() const;
      virtual Core::FastU getSizeShift() const;
      virtual Core::FastU getSizeWords() const;

      // Type classification: General classifications.
      virtual bool isTypeBoolean()  const {return true;}
      virtual bool isTypeComplete() const {return true;}

      // Type classification: C/C++ classifications.
      virtual bool isCTypeObject() const {return true;}
      virtual bool isCTypeScalar() const {return true;}


      friend SR::Type::CRef GetTypeBoolSoft();

   protected:
      Type_BoolSoft() = default;
   };
}

#endif//GDCC__CC__Type__Bool_H__

