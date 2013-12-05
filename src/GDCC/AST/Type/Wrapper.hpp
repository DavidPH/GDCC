//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression wrapping type base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Type__Wrapper_H__
#define GDCC__AST__Type__Wrapper_H__

#include "../Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type_Wrapper
      //
      class Type_Wrapper : public Type
      {
         GDCC_CounterPreambleAbstract(GDCC::AST::Type_Wrapper, GDCC::AST::Type);

      public:
         // Type information.
         virtual Type::CRef   getBaseType()    const {return base;}
         virtual IR::CallType getCallType()    const {return base->getCallType();}
         virtual IR::Type     getIRType()      const;
         virtual String       getName()        const {return base->getName();}
         virtual FastU        getSizeAlign()   const {return base->getSizeAlign();}
         virtual FastU        getSizeBitsF()   const {return base->getSizeBitsF();}
         virtual FastU        getSizeBitsI()   const {return base->getSizeBitsI();}
         virtual FastU        getSizeBitsO()   const {return base->getSizeBitsO();}
         virtual bool         getSizeBitsS()   const {return base->getSizeBitsS();}
         virtual FastU        getSizeBytes()   const {return base->getSizeBytes();}
         virtual ExpCRef      getSizeBytesVM() const;
         virtual FastU        getSizePoint()   const {return base->getSizePoint();}
         virtual ExpCRef      getSizePointVM() const;
         virtual FastU        getSizeShift()   const {return base->getSizeShift();}
         virtual FastU        getSizeWords()   const {return base->getSizeWords();}
         virtual ExpCRef      getSizeWordsVM() const;

         // Type classification: General classifications.
         virtual bool isTypeArray()    const {return base->isTypeArray();}
         virtual bool isTypeBitfield() const {return base->isTypeBitfield();}
         virtual bool isTypeBoolean()  const {return base->isTypeBoolean();}
         virtual bool isTypeComplete() const {return base->isTypeComplete();}
         virtual bool isTypeComplex()  const {return base->isTypeComplex();}
         virtual bool isTypeFunction() const {return base->isTypeFunction();}
         virtual bool isTypeImagin()   const {return base->isTypeImagin();}
         virtual bool isTypeLabel()    const {return base->isTypeLabel();}
         virtual bool isTypeNullptr()  const {return base->isTypeNullptr();}
         virtual bool isTypePointer()  const {return base->isTypePointer();}
         virtual bool isTypeRef()      const {return base->isTypeRef();}
         virtual bool isTypeRefL()     const {return base->isTypeRefL();}
         virtual bool isTypeRefR()     const {return base->isTypeRefR();}
         virtual bool isTypeSaturate() const {return base->isTypeSaturate();}
         virtual bool isTypeSizeVM()   const {return base->isTypeSizeVM();}
         virtual bool isTypeStrEnt()   const {return base->isTypeStrEnt();}
         virtual bool isTypeVM()       const {return base->isTypeVM();}
         virtual bool isTypeVoid()     const {return base->isTypeVoid();}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeAccum()    const {return base->isCTypeAccum();}
         virtual bool isCTypeAccumS()   const {return base->isCTypeAccumS();}
         virtual bool isCTypeAccumU()   const {return base->isCTypeAccumU();}
         virtual bool isCTypeAggregat() const {return base->isCTypeAggregat();}
         virtual bool isCTypeArith()    const {return base->isCTypeArith();}
         virtual bool isCTypeChar()     const {return base->isCTypeChar();}
         virtual bool isCTypeComplex()  const {return base->isCTypeComplex();}
         virtual bool isCTypeEnum()     const {return base->isCTypeEnum();}
         virtual bool isCTypeEnumS()    const {return base->isCTypeEnumS();}
         virtual bool isCTypeEnumU()    const {return base->isCTypeEnumU();}
         virtual bool isCTypeFixed()    const {return base->isCTypeFixed();}
         virtual bool isCTypeFixedPr()  const {return base->isCTypeFixedPr();}
         virtual bool isCTypeFixedPrS() const {return base->isCTypeFixedPrS();}
         virtual bool isCTypeFixedPrU() const {return base->isCTypeFixedPrU();}
         virtual bool isCTypeFixedSa()  const {return base->isCTypeFixedSa();}
         virtual bool isCTypeFixedSaS() const {return base->isCTypeFixedSaS();}
         virtual bool isCTypeFixedSaU() const {return base->isCTypeFixedSaU();}
         virtual bool isCTypeFloat()    const {return base->isCTypeFloat();}
         virtual bool isCTypeFract()    const {return base->isCTypeFract();}
         virtual bool isCTypeFractS()   const {return base->isCTypeFractS();}
         virtual bool isCTypeFractU()   const {return base->isCTypeFractU();}
         virtual bool isCTypeFunction() const {return base->isCTypeFunction();}
         virtual bool isCTypeImagin()   const {return base->isCTypeImagin();}
         virtual bool isCTypeInteg()    const {return base->isCTypeInteg();}
         virtual bool isCTypeIntegS()   const {return base->isCTypeIntegS();}
         virtual bool isCTypeIntegU()   const {return base->isCTypeIntegU();}
         virtual bool isCTypeMemFn()    const {return base->isCTypeMemFn();}
         virtual bool isCTypeMemPt()    const {return base->isCTypeMemPt();}
         virtual bool isCTypeObject()   const {return base->isCTypeObject();}
         virtual bool isCTypeReal()     const {return base->isCTypeReal();}
         virtual bool isCTypeRealFlt()  const {return base->isCTypeRealFlt();}
         virtual bool isCTypeScalar()   const {return base->isCTypeScalar();}
         virtual bool isCTypeStruct()   const {return base->isCTypeStruct();}
         virtual bool isCTypeUnion()    const {return base->isCTypeUnion();}

      protected:
         Type_Wrapper(Type const *base_) : base{base_} {}

         Type::CRef const base;
      };
   }
}

#endif//GDCC__AST__Type__Wrapper_H__

