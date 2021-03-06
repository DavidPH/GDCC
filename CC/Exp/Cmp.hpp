//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C relational and equality expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Cmp_H__
#define GDCC__CC__Exp__Cmp_H__

#include "../../CC/Exp/Arith.hpp"

#include "../../SR/Exp/Cmp.hpp"
#include "../../SR/ExpCode.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ExpCreate_CmpArith
   //
   // Uses a base type and code set to generalize basic arithmetic
   // comparator expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_CmpArith(Factory &, IR::CodeBase base, SR::Type const *bt,
      SR::Type const *t, SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      IR::Code code;

      // Floating types.
      if(t->isCTypeFloat())
         code = SR::ExpCode_ArithFloat(base, t);

      // Fixed-point types.
      else if(t->isCTypeFixed())
         code = SR::ExpCode_ArithFixed(base, t);

      // Integer types.
      else if(t->isCTypeInteg())
         code = SR::ExpCode_ArithInteg(base, t);

      // Pointer types.
      else if(t->isTypePointer())
         code = SR::ExpCode_ArithPoint(base, t);

      // ???
      else
         Core::Error(pos, "unsupported arithmetic type");

      return SR::Exp_Arith<Base>::Create(code, bt, l, r, pos);
   }
}

#endif//GDCC__CC__Exp__Cmp_H__

