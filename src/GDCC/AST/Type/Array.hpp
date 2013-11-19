//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../Type.hpp"


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
         GDCC_CounterPreambleAbstract(GDCC::AST::Type_ArrBase, GDCC::AST::Type);

      public:
         // Type information.
         virtual Type::CRef getBaseType()  const {return base;}
         virtual FastU      getSizeAlign() const {return base->getSizeAlign();}
         virtual FastU      getSizeShift() const {return base->getSizeShift();}

         // Type classification.
         virtual bool isTypeArray() const {return true;}

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
         GDCC_CounterPreamble(GDCC::AST::Type_ArrVM, GDCC::AST::Type_ArrBase);

      public:
         // Type information.
         virtual ExpRef getSizeBytesVM() const;
         virtual ExpRef getSizePointVM() const;
         virtual ExpRef getSizeWordsVM() const;

         // Type classification.
         virtual bool isTypeComplete() const {return true;}
         virtual bool isTypeSizeVM()   const {return true;}
         virtual bool isTypeVM()       const {return true;}


         friend Type::CRef Type::getTypeArray(Exp *size) const;

      protected:
         Type_ArrVM(Type_ArrVM const &type);
         Type_ArrVM(Type const *base, Exp *size);
         virtual ~Type_ArrVM();

         ExpRef const size;

      private:
         mutable Type_ArrVM const *avmNext, *avmPrev;
      };

      //
      // Type_ArrVM0
      //
      class Type_ArrVM0 final : public Type_ArrBase
      {
         GDCC_CounterPreamble(GDCC::AST::Type_ArrVM0, GDCC::AST::Type_ArrBase);

      public:
         // Type classification.
         virtual bool isTypeSizeVM() const {return true;}
         virtual bool isTypeVM()     const {return true;}


         friend Type::CRef Type::getTypeArray(Exp *size) const;

      protected:
         explicit Type_ArrVM0(Type const *base);
         virtual ~Type_ArrVM0();
      };

      //
      // Type_Array
      //
      class Type_Array final : public Type_ArrBase
      {
         GDCC_CounterPreamble(GDCC::AST::Type_Array, GDCC::AST::Type_ArrBase);

      public:
         // Type information.
         virtual IR::Type getIRType()    const;
         virtual FastU    getSizeBytes() const;
         virtual FastU    getSizePoint() const;
         virtual FastU    getSizeWords() const;

         // Type classification.
         virtual bool isTypeComplete() const {return true;}
         virtual bool isTypeSizeVM()   const;
         virtual bool isTypeVM()       const;


         friend Type::CRef Type::getTypeArray(FastU size) const;

      protected:
         Type_Array(Type_Array const &type);
         Type_Array(Type const *base, FastU size);
         virtual ~Type_Array();

         FastU const size;

      private:
         mutable Type_Array const *arrNext, *arrPrev;
      };

      //
      // Type_Array0
      //
      class Type_Array0 final : public Type_ArrBase
      {
         GDCC_CounterPreamble(GDCC::AST::Type_Array0, GDCC::AST::Type_ArrBase);

      public:
         // Type classification.
         virtual bool isTypeSizeVM() const;
         virtual bool isTypeVM()     const;


         friend Type::CRef Type::getTypeArray() const;

      protected:
         explicit Type_Array0(Type const *base);
         virtual ~Type_Array0();
      };
   }
}

#endif//GDCC__AST__Type__Array_H__
