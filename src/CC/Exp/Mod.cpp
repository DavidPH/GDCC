//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator %" expressions.
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
      // ExpCreate_Mod
      //
      AST::Exp::CRef ExpCreate_Mod(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // arithmetic % arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_Arith<AST::Exp_Mod, IR::CodeSet_Mod>(type, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator %'");
      }

      //
      // ExpCreate_ModEq
      //
      AST::Exp::CRef ExpCreate_ModEq(AST::Exp const *expL, AST::Exp const *r,
         Core::Origin pos)
      {
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // arithmetic %= arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            // Promote to type of left operand. This should work in most cases.
            expR = ExpConvert_Arith(typeL, expR, pos);

            return ExpCreate_ArithEq<AST::Exp_Mod, IR::CodeSet_Mod>(
               typeL, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator %='");
      }
   }
}

// EOF

