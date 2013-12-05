//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree primary/primitive expression types.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Type__Primary_H__
#define GDCC__AST__Type__Primary_H__

#include "../Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type_None
      //
      class Type_None final : public Type
      {
         GDCC_CounterPreamble(GDCC::AST::Type_None, GDCC::AST::Type);

      public:
         friend Type::CRef Type::GetNone();

      protected:
         Type_None() {}
      };

      //
      // Type_Size
      //
      class Type_Size final : public Type
      {
         GDCC_CounterPreamble(GDCC::AST::Type_Size, GDCC::AST::Type);

      public:
         // Type information.
         virtual IR::Type getIRType()    const;
         virtual FastU    getSizeAlign() const;
         virtual FastU    getSizeBitsF() const {return 0;}
         virtual FastU    getSizeBitsI() const;
         virtual FastU    getSizeBitsO() const {return 0;}
         virtual bool     getSizeBitsS() const {return false;}
         virtual FastU    getSizeBytes() const;
         virtual FastU    getSizePoint() const;
         virtual FastU    getSizeShift() const;
         virtual FastU    getSizeWords() const;

         // Type classification: General classifications.
         virtual bool isTypeComplete() const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeArith()    const {return true;}
         virtual bool isCTypeInteg()    const {return true;}
         virtual bool isCTypeIntegU()   const {return true;}
         virtual bool isCTypeObject()   const {return true;}
         virtual bool isCTypeReal()     const {return true;}
         virtual bool isCTypeScalar()   const {return true;}


         friend Type::CRef Type::GetSize();

      protected:
         Type_Size() {}
      };
   }
}

#endif//GDCC__AST__Type__Primary_H__

