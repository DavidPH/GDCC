//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "../../AST/Exp/Arith.hpp"
#include "../../AST/ExpCode.hpp"

#include "../../Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_ArithFixed
      //
      // Uses a base type and code set to generalize basic arithmetic
      // fixed-point expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_ArithFixed(AST::Type const *t,
         AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         // Fixed-point arithmetic promotion doesn't always convert to the
         // result type. For generic handling, do so.
         auto expL = ExpConvert_Arith(t, l, pos);
         auto expR = ExpConvert_Arith(t, r, pos);

         IR::Code code = AST::ExpCode_ArithFixed<Codes>(t);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported fixed-point operand size");

         return AST::Exp_Arith<Base>::Create(code, t, expL, expR, pos);
      }

      //
      // ExpCreate_ArithFloat
      //
      // Uses a base type and code set to generalize basic arithmetic
      // floating expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_ArithFloat(AST::Type const *t,
         AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         IR::Code code = AST::ExpCode_ArithFloat<Codes>(t);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported floating operand size");

         return AST::Exp_Arith<Base>::Create(code, t, l, r, pos);
      }

      //
      // ExpCreate_ArithInteg
      //
      // Uses a base type and code set to generalize basic arithmetic
      // integer expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_ArithInteg(AST::Type const *t,
         AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         IR::Code code = AST::ExpCode_ArithInteg<Codes>(t);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported integer operand size");

         return AST::Exp_Arith<Base>::Create(code, t, l, r, pos);
      }

      //
      // ExpCreate_Arith
      //
      // Uses a base type and code set to generalize basic arithmetic
      // expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_Arith(AST::Type const *t, AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos)
      {
         // Floating types.
         if(t->isCTypeFloat())
            return ExpCreate_ArithFloat<Base, Codes>(t, l, r, pos);

         // Fixed-point types.
         if(t->isCTypeFixed())
            return ExpCreate_ArithFixed<Base, Codes>(t, l, r, pos);

         // Integer types.
         if(t->isCTypeInteg())
            return ExpCreate_ArithInteg<Base, Codes>(t, l, r, pos);

         // ???
         throw Core::ExceptStr(pos, "unsupported arithmetic type");
      }

      //
      // ExpCreate_ArithEqFixed
      //
      // Uses a base type and code set to generalize basic arithmetic
      // fixed-point expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_ArithEqFixed(AST::Type const *evalT,
         AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos, bool post = false)
      {
         // Fixed-point arithmetic promotion doesn't always convert to the
         // result type. For generic handling, do so.
         auto expR = ExpConvert_Arith(evalT, r, pos);

         IR::Code code = AST::ExpCode_ArithFixed<Codes>(evalT);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported fixed-point operand size");

         return AST::Exp_ArithEq<Base>::Create(evalT, code, post, t, l, expR, pos);
      }

      //
      // ExpCreate_ArithEqFloat
      //
      // Uses a base type and code set to generalize basic arithmetic
      // floating expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_ArithEqFloat(AST::Type const *evalT,
         AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos, bool post = false)
      {
         IR::Code code = AST::ExpCode_ArithFloat<Codes>(evalT);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported floating operand size");

         return AST::Exp_ArithEq<Base>::Create(evalT, code, post, t, l, r, pos);
      }

      //
      // ExpCreate_ArithEqInteg
      //
      // Uses a base type and code set to generalize basic arithmetic
      // integer expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_ArithEqInteg(AST::Type const *evalT,
         AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos, bool post = false)
      {
         IR::Code code = AST::ExpCode_ArithInteg<Codes>(evalT);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported integer operand size");

         return AST::Exp_ArithEq<Base>::Create(evalT, code, post, t, l, r, pos);
      }

      //
      // ExpCreate_ArithEq
      //
      // Uses a base type and code set to generalize basic arithmetic
      // assignment expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_ArithEq(AST::Type const *evalT,
         AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos, bool post = false)
      {
         // Floating types.
         if(evalT->isCTypeFloat())
            return ExpCreate_ArithEqFloat<Base, Codes>(evalT, t, l, r, pos, post);

         // Fixed-point types.
         if(evalT->isCTypeFixed())
            return ExpCreate_ArithEqFixed<Base, Codes>(evalT, t, l, r, pos, post);

         // Integer types.
         if(evalT->isCTypeInteg())
            return ExpCreate_ArithEqInteg<Base, Codes>(evalT, t, l, r, pos, post);

         // ???
         throw Core::ExceptStr(pos, "unsupported arithmetic type");
      }
   }
}

#endif//GDCC__CC__Exp__Arith_H__

