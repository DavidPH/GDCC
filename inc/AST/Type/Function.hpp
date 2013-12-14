//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression function type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Type__Function_H__
#define GDCC__AST__Type__Function_H__

#include "../../AST/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type_Function
      //
      class Type_Function : public Type
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Type_Function, GDCC::AST::Type);

      public:
         // Type information.
         virtual Type::CRef    getBaseType()   const {return base;}
         virtual IR::CallType  getCallType()   const {return ctype;}
         virtual Core::FastU   getCallWords()  const;
         virtual TypeSet::CRef getParameters() const {return param;}

         // Type classification: General classifications.
         virtual bool isTypeFunction() const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeFunction() const {return true;}


         friend Type::CRef Type::getTypeFunction(TypeSet const *param, IR::CallType ctype) const;

      protected:
         Type_Function(Type_Function const &type);
         Type_Function(Type const *base, TypeSet const *param, IR::CallType ctype);
         virtual ~Type_Function();

         Type::CRef    const base;
         TypeSet::CRef const param;
         IR::CallType  const ctype;

      private:
         mutable Type_Function const *funNext, *funPrev;
      };
   }
}

#endif//GDCC__AST__Type__Function_H__

