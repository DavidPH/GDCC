//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator /" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Exp/Arith.hpp"

#include "IR/CodeSet/Arith.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_Div
      //
      AST::Exp::CRef ExpCreate_Div(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // arithmetic / arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            // TODO: fixed / integer doesn't require conversion.

            return ExpCreate_Arith<AST::Exp_Div, IR::CodeSet_Div>(type, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator /'");
      }

      //
      // ExpCreate_DivEq
      //
      AST::Exp::CRef ExpCreate_DivEq(AST::Exp const *expL, AST::Exp const *r,
         Core::Origin pos)
      {
         if(!IsModLValue(expL))
            throw Core::ExceptStr(pos, "expected modifiable lvalue");

         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // arithmetic /= arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            AST::Type::CPtr evalT;
            std::tie(evalT, std::ignore, expR) = ExpPromo_Arith(expL, expR, pos);

            // TODO: fixed /= integer doesn't require conversion.

            return ExpCreate_ArithEq<AST::Exp_Div, IR::CodeSet_Div>(
               evalT, typeL, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator /='");
      }
   }
}

// EOF

