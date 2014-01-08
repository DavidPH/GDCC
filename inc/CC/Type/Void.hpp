//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C void type.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Void_H__
#define GDCC__CC__Type__Void_H__

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
      // Type_Void
      //
      class Type_Void final : public AST::Type
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Void, GDCC::AST::Type);

      public:
         // Type information.
         virtual IR::Type getIRType() const;

         // Type classification: General classifications.
         virtual bool isTypeVoid() const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeObject() const {return true;}


         friend AST::Type::CRef GetTypeVoid();

      protected:
         Type_Void() = default;
      };
   }
}

#endif//GDCC__CC__Type__Void_H__

