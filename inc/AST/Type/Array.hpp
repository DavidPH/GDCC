//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression array type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Type__Array_H__
#define GDCC__AST__Type__Array_H__

#include "../../AST/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type_ArrBase
      //
      class Type_ArrBase : public Type
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Type_ArrBase, GDCC::AST::Type);

      public:
         // Type information.
         virtual Type::CRef  getBaseType()  const {return base;}
         virtual Core::FastU getSizeAlign() const {return base->getSizeAlign();}
         virtual Core::FastU getSizeShift() const {return base->getSizeShift();}

         // Type classification.
         virtual bool isTypeComplete() const;
         virtual bool isTypeArray()    const {return true;}

         virtual bool isCTypeAggregat() const {return true;}
         virtual bool isCTypeObject()   const {return true;}

      protected:
         Type_ArrBase(Type const *base_) : base{base_} {}

         Type::CRef const base;
      };

      //
      // Type_ArrVM
      //
      class Type_ArrVM final : public Type_ArrBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Type_ArrVM, GDCC::AST::Type_ArrBase);

      public:
         // Type creation.
         virtual Type::CRef getTypeArrayQual() const;
         virtual Type::CRef getTypeArrayQualAddr(IR::AddrSpace addr) const;

         // Type information.
         virtual ExpCRef getSizeBytesVM() const;
         virtual ExpCRef getSizePointVM() const;
         virtual ExpCRef getSizeWordsVM() const;

         // Type classification.
         virtual bool isTypeSizeVM() const {return true;}
         virtual bool isTypeVM()     const {return true;}


         friend Type::CRef Type::getTypeArray(Exp const *size) const;

      protected:
         Type_ArrVM(Type_ArrVM const &type);
         Type_ArrVM(Type const *base, Exp const *size);
         virtual ~Type_ArrVM();

         ExpCRef const size;

      private:
         mutable Type_ArrVM const *avmNext, *avmPrev;
      };

      //
      // Type_ArrVM0
      //
      class Type_ArrVM0 final : public Type_ArrBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Type_ArrVM0, GDCC::AST::Type_ArrBase);

      public:
         // Type creation.
         virtual Type::CRef getTypeArrayQual() const;
         virtual Type::CRef getTypeArrayQualAddr(IR::AddrSpace addr) const;

         // Type classification.
         virtual bool isTypeSizeVM() const {return true;}
         virtual bool isTypeVM()     const {return true;}


         friend Type::CRef Type::getTypeArray(Exp const *size) const;

      protected:
         explicit Type_ArrVM0(Type const *base);
         virtual ~Type_ArrVM0();
      };

      //
      // Type_Array
      //
      class Type_Array final : public Type_ArrBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Type_Array, GDCC::AST::Type_ArrBase);

      public:
         // Type creation.
         virtual Type::CRef getTypeArrayQual() const;
         virtual Type::CRef getTypeArrayQualAddr(IR::AddrSpace addr) const;

         // Type information.
         virtual IR::Type    getIRType()      const;
         virtual Core::FastU getSizeBytes()   const;
         virtual ExpCRef     getSizeBytesVM() const;
         virtual Core::FastU getSizePoint()   const;
         virtual ExpCRef     getSizePointVM() const;
                 Core::FastU getSizeWidth()   const {return size;}
         virtual Core::FastU getSizeWords()   const;
         virtual ExpCRef     getSizeWordsVM() const;

         // Type classification.
         virtual bool isTypeSizeVM() const;
         virtual bool isTypeVM()     const;


         friend Type::CRef Type::getTypeArray(Core::FastU size) const;

      protected:
         Type_Array(Type_Array const &type);
         Type_Array(Type const *base, Core::FastU size);
         virtual ~Type_Array();

         Core::FastU const size;

      private:
         mutable Type_Array const *arrNext, *arrPrev;
      };

      //
      // Type_Array0
      //
      class Type_Array0 final : public Type_ArrBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Type_Array0, GDCC::AST::Type_ArrBase);

      public:
         // Type creation.
         virtual Type::CRef getTypeArrayQual() const;
         virtual Type::CRef getTypeArrayQualAddr(IR::AddrSpace addr) const;

         // Type classification.
         virtual bool isTypeComplete() const {return false;}
         virtual bool isTypeSizeVM()   const;
         virtual bool isTypeVM()       const;


         friend Type::CRef Type::getTypeArray() const;

      protected:
         explicit Type_Array0(Type const *base);
         virtual ~Type_Array0();
      };
   }
}

#endif//GDCC__AST__Type__Array_H__

