//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C arithmetic expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Arith_H__
#define GDCC__CC__Exp__Arith_H__

#include "../../CC/Exp.hpp"

#include "../../CC/Factory.hpp"

#include "../../Core/Exception.hpp"

#include "../../SR/Exp/Arith.hpp"
#include "../../SR/ExpCode.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ExpCreate_ArithFixed
   //
   // Uses a base type and code set to generalize basic arithmetic
   // fixed-point expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_ArithFixed(Factory &fact, IR::CodeBase base,
      SR::Type const *t, SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      // Fixed-point arithmetic promotion doesn't always convert to the
      // result type. For generic handling, do so.
      auto expL = fact.expConvert_Arith(t, l, pos);
      auto expR = fact.expConvert_Arith(t, r, pos);

      auto code = SR::ExpCode_ArithFixed(base, t);

      return SR::Exp_Arith<Base>::Create(code, t, expL, expR, pos);
   }

   //
   // ExpCreate_ArithFloat
   //
   // Uses a base type and code set to generalize basic arithmetic
   // floating expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_ArithFloat(Factory &, IR::CodeBase base,
      SR::Type const *t, SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      auto code = SR::ExpCode_ArithFloat(base, t);

      return SR::Exp_Arith<Base>::Create(code, t, l, r, pos);
   }

   //
   // ExpCreate_ArithInteg
   //
   // Uses a base type and code set to generalize basic arithmetic
   // integer expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_ArithInteg(Factory &, IR::CodeBase base,
      SR::Type const *t, SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      auto code = SR::ExpCode_ArithInteg(base, t);

      return SR::Exp_Arith<Base>::Create(code, t, l, r, pos);
   }

   //
   // ExpCreate_Arith
   //
   // Uses a base type and code set to generalize basic arithmetic
   // expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_Arith(Factory &fact, IR::CodeBase base,
      SR::Type const *t, SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      // Floating types.
      if(t->isCTypeFloat())
         return ExpCreate_ArithFloat<Base>(fact, base, t, l, r, pos);

      // Fixed-point types.
      if(t->isCTypeFixed())
         return ExpCreate_ArithFixed<Base>(fact, base, t, l, r, pos);

      // Integer types.
      if(t->isCTypeInteg())
         return ExpCreate_ArithInteg<Base>(fact, base, t, l, r, pos);

      // ???
      Core::Error(pos, "unsupported arithmetic type");
   }

   //
   // ExpCreate_ArithEqFixed
   //
   // Uses a base type and code set to generalize basic arithmetic
   // fixed-point expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_ArithEqFixed(Factory &fact, IR::CodeBase base,
      SR::Type const *evalT, SR::Type const *t, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos, bool post = false)
   {
      // Fixed-point arithmetic promotion doesn't always convert to the
      // result type. For generic handling, do so.
      auto expR = fact.expConvert_Arith(evalT, r, pos);

      auto code = SR::ExpCode_ArithFixed(base, evalT);

      return SR::Exp_ArithEq<Base>::Create(evalT, code, post, t, l, expR, pos);
   }

   //
   // ExpCreate_ArithEqFloat
   //
   // Uses a base type and code set to generalize basic arithmetic
   // floating expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_ArithEqFloat(Factory &, IR::CodeBase base,
      SR::Type const *evalT, SR::Type const *t, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos, bool post = false)
   {
      auto code = SR::ExpCode_ArithFloat(base, evalT);

      return SR::Exp_ArithEq<Base>::Create(evalT, code, post, t, l, r, pos);
   }

   //
   // ExpCreate_ArithEqInteg
   //
   // Uses a base type and code set to generalize basic arithmetic
   // integer expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_ArithEqInteg(Factory &, IR::CodeBase base,
      SR::Type const *evalT, SR::Type const *t, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos, bool post = false)
   {
      auto code = SR::ExpCode_ArithInteg(base, evalT);

      return SR::Exp_ArithEq<Base>::Create(evalT, code, post, t, l, r, pos);
   }

   //
   // ExpCreate_ArithEq
   //
   // Uses a base type and code set to generalize basic arithmetic
   // assignment expression creation.
   //
   template<typename Base>
   SR::Exp::CRef ExpCreate_ArithEq(Factory &fact, IR::CodeBase base,
      SR::Type const *evalT, SR::Type const *t, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos, bool post = false)
   {
      // Floating types.
      if(evalT->isCTypeFloat())
         return ExpCreate_ArithEqFloat<Base>(fact, base, evalT, t, l, r, pos, post);

      // Fixed-point types.
      if(evalT->isCTypeFixed())
         return ExpCreate_ArithEqFixed<Base>(fact, base, evalT, t, l, r, pos, post);

      // Integer types.
      if(evalT->isCTypeInteg())
         return ExpCreate_ArithEqInteg<Base>(fact, base, evalT, t, l, r, pos, post);

      // ???
      Core::Error(pos, "unsupported arithmetic type");
   }
}

#endif//GDCC__CC__Exp__Arith_H__

