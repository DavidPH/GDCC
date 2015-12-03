//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Exp_H__
#define GDCC__ACC__Exp_H__

#include "ACC/Exp.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "CC/Exp.hpp"
#include "CC/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // ExpPromo_Assign
      //
      // Allows additional implicit conversions over C.
      //
      AST::Exp::CRef ExpPromo_Assign(AST::Type const *typeL, AST::Exp const *e,
         Core::Origin pos)
      {
         auto exp   = CC::ExpPromo_LValue(e, pos);
         auto typeR = exp->getType();

         // integer = str
         if(typeL->isCTypeInteg() &&
            typeR->isTypePointer() && typeR->getBaseType()->isTypeStrEnt())
         {
            return CC::ExpConvert_ArithPtr(typeL, exp, pos);
         }

         // str = integer
         if(typeL->isTypePointer() && typeL->getBaseType()->isTypeStrEnt() &&
            typeR->isCTypeInteg())
         {
            return CC::ExpConvert_PtrArith(typeL, exp, pos);
         }

         // Defer to C rules.
         return CC::ExpPromo_AssignBase(typeL, exp, pos);
      }
   }
}

#endif//GDCC__ACC__Exp_H__

