//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression pointer/reference type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Type__Pointer_H__
#define GDCC__SR__Type__Pointer_H__

#include "../../SR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Type_PtrBase
      //
      class Type_PtrBase : public Type
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::SR::Type_PtrBase, GDCC::SR::Type);

      public:
         // Type information.
         virtual Type::CRef  getBaseType()  const {return base;}
         virtual IR::Type    getIRType()    const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;

         // Type classification.
         virtual bool isTypeComplete() const {return true;}
         virtual bool isTypeVM()       const;

      protected:
         Type_PtrBase(Type const *base_) : base{base_} {}

         Type::CRef const base;
      };

      //
      // Type_Pointer
      //
      class Type_Pointer final : public Type_PtrBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::SR::Type_Pointer, GDCC::SR::Type_PtrBase);

      public:
         // Type classification.
         virtual bool isTypePointer() const {return true;}

         virtual bool isCTypeObject() const {return true;}
         virtual bool isCTypeScalar() const {return true;}


         friend Type::CRef Type::getTypePointer() const;

      protected:
         explicit Type_Pointer(Type const *base);
         virtual ~Type_Pointer();
      };

      //
      // Type_RefL
      //
      class Type_RefL final : public Type_PtrBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::SR::Type_RefL, GDCC::SR::Type_PtrBase);

      public:
         // Type classification.
         virtual bool isTypeRef()  const {return true;}
         virtual bool isTypeRefL() const {return true;}


         friend Type::CRef Type::getTypeRefL() const;

      protected:
         explicit Type_RefL(Type const *base);
         virtual ~Type_RefL();
      };

      //
      // Type_RefR
      //
      class Type_RefR final : public Type_PtrBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::SR::Type_RefR, GDCC::SR::Type_PtrBase);

      public:
         // Type classification.
         virtual bool isTypeRef()  const {return true;}
         virtual bool isTypeRefR() const {return true;}


         friend Type::CRef Type::getTypeRefR() const;

      protected:
         explicit Type_RefR(Type const *base);
         virtual ~Type_RefR();
      };
   }
}

#endif//GDCC__SR__Type__Pointer_H__

