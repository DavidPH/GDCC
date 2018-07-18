//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression function type handling.
//
//-----------------------------------------------------------------------------

#include "SR/Type/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Type::getTypeFunction
   //
   Type::CRef Type::getTypeFunction(TypeSet const *param, IR::CallType ctype) const
   {
      // Search for existing function type.
      if(auto type = funType) do
      {
         if(type->param == param && type->ctype == ctype)
            return static_cast<CRef>(type);

         type = type->funNext;
      }
      while(type != funType);

      // Otherwise, create one and return it.
      return static_cast<CRef>(new Type_Function(this, param, ctype));
   }

   //
   // Type_Function copy constructor
   //
   Type_Function::Type_Function(Type_Function const &type) : Super{type},
      base{type.base}, param{type.param}, ctype{type.ctype}, funNext{this},
      funPrev{this}
   {
   }

   //
   // Type_Function constructor
   //
   Type_Function::Type_Function(Type const *base_, TypeSet const *param_,
      IR::CallType ctype_) : base{base_}, param{param_}, ctype{ctype_}
   {
      GDCC_SR_Type_Insert(fun);
   }

   //
   // Type_Function destructor
   //
   Type_Function::~Type_Function()
   {
      GDCC_SR_Type_Unlink(fun);
   }

   //
   // Type_Function::getCallWords
   //
   Core::FastU Type_Function::getCallWords() const
   {
      Core::FastU w = 0;
      for(auto const &t : *param)
         w += t->getSizeWords();

      return w;
   }
}

// EOF

