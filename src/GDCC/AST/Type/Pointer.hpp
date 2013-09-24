//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression pointer/reference type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Type__Pointer_H__
#define GDCC__AST__Type__Pointer_H__

#include "../Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type_PtrBase
      //
      class Type_PtrBase : public Type
      {
         GDCC_CounterPreambleAbstract(GDCC::AST::Type_PtrBase, GDCC::AST::Type);

      public:
         // Type information.
         virtual Type::CRef getBaseType()  const {return base;}
         virtual IR::Type   getIRType()    const;
         virtual FastU      getSizeAlign() const;
         virtual FastU      getSizeBytes() const;
         virtual FastU      getSizePoint() const;
         virtual FastU      getSizeShift() const;
         virtual FastU      getSizeWords() const;

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
         GDCC_CounterPreamble(GDCC::AST::Type_Pointer, GDCC::AST::Type_PtrBase);

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
         GDCC_CounterPreamble(GDCC::AST::Type_RefL, GDCC::AST::Type_PtrBase);

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
         GDCC_CounterPreamble(GDCC::AST::Type_RefR, GDCC::AST::Type_PtrBase);

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

#endif//GDCC__AST__Type__Pointer_H__

