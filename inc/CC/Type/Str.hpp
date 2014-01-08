//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C __str type.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Str_H__
#define GDCC__CC__Type__Str_H__

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
      // Type_Str
      //
      class Type_Str final : public AST::Type
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Str, GDCC::AST::Type);

      public:
         // Type information.
         virtual IR::Type    getIRType()    const;
         virtual Core::FastU getSizeAlign() const;
         virtual Core::FastU getSizeBytes() const;
         virtual Core::FastU getSizePoint() const;
         virtual Core::FastU getSizeShift() const;
         virtual Core::FastU getSizeWords() const {return 1;}

         // Type classification: General classifications.
         virtual bool isTypeComplete() const {return true;}
         virtual bool isTypeStrEnt()   const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeObject() const {return true;}
         virtual bool isCTypeScalar() const {return true;}


         friend AST::Type::CRef GetTypeStr();

      protected:
         Type_Str() = default;
      };
   }
}

#endif//GDCC__CC__Type__Str_H__

