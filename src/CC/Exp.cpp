//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "AST/Arg.hpp"
#include "AST/Exp.hpp"
#include "AST/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsLValue
      //
      bool IsLValue(AST::Exp const *exp)
      {
         switch(exp->getArg().type->getQualAddr().base)
         {
         case IR::AddrBase::Cpy:
         case IR::AddrBase::Lit:
         case IR::AddrBase::Nul:
         case IR::AddrBase::Stk:
            return false;

         default:
            return exp->getType()->isCTypeObject();
         }
      }

      //
      // IsModLValue
      //
      bool IsModLValue(AST::Exp const *exp)
      {
         auto type = exp->getType();

         if(!IsLValue(exp))          return false;
         if(type->isTypeArray())     return false;
         if(!type->isTypeComplete()) return false;
         if(type->getQualCons())     return false;
         // TODO: Check struct/union members.

         return true;
      }
   }
}

// EOF

