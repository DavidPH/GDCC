//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation primary/primitive expression types.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Type__Primary_H__
#define GDCC__SR__Type__Primary_H__

#include "../../SR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Type_Label
      //
      class Type_Label final : public SR::Type
      {
         GDCC_Core_CounterPreamble(GDCC::SR::Type_Label, GDCC::SR::Type);

      public:
         // Type classification: General classifications.
         virtual bool isTypeLabel() const {return true;}


         friend Type::CRef Type::GetLabel();

      protected:
         Type_Label() = default;
      };

      //
      // Type_None
      //
      class Type_None final : public Type
      {
         GDCC_Core_CounterPreamble(GDCC::SR::Type_None, GDCC::SR::Type);

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
         GDCC_Core_CounterPreamble(GDCC::SR::Type_Size, GDCC::SR::Type);

      public:
         // Type information.
         virtual IR::Type    getIRType()    const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBitsF() const {return 0;}
         virtual Core::FastU getSizeBitsI() const;
         virtual Core::FastU getSizeBitsO() const {return 0;}
         virtual bool        getSizeBitsS() const {return false;}
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const;

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

      //
      // Type_StrEnt
      //
      class Type_StrEnt final : public SR::Type
      {
         GDCC_Core_CounterPreamble(GDCC::SR::Type_StrEnt, GDCC::SR::Type);

      public:
         // Type classification: General classifications.
         virtual bool isTypeStrEnt() const {return true;}


         friend Type::CRef Type::GetStrEnt();

      protected:
         Type_StrEnt() = default;
      };

      //
      // Type_Void
      //
      class Type_Void final : public SR::Type
      {
         GDCC_Core_CounterPreamble(GDCC::SR::Type_Void, GDCC::SR::Type);

      public:
         // Type information.
         virtual IR::Type    getIRType()    const;
         virtual Core::FastU getSizeShift() const {return 1;}

         // Type classification: General classifications.
         virtual bool isTypeVoid() const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeObject() const {return true;}


         friend Type::CRef Type::GetVoid();

      protected:
         Type_Void() = default;
      };
   }
}

#endif//GDCC__SR__Type__Primary_H__

