//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C binary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Mul.hpp"

#include "CC/Type.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Mul constructor
      //
      Exp_Mul::Exp_Mul(AST::Type const *t, AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos_) :
         Super{l, r, pos_}, type{t}
      {
      }

      //
      // Exp_Mul destructor
      //
      Exp_Mul::~Exp_Mul()
      {
      }

      //
      // Exp_Mul::v_getIRExp
      //
      IR::Exp::CRef Exp_Mul::v_getIRExp() const
      {
         return IR::ExpCreate_BinaryMul(expL->getIRExp(), expR->getIRExp(), pos);
      }

      //
      // Exp_Mul::v_getType
      //
      AST::Type::CRef Exp_Mul::v_getType() const
      {
         return type;
      }

      //
      // ExpCreate_Mul
      //
      AST::Exp::CRef ExpCreate_Mul(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = l->getType();
         auto typeR = r->getType();

         // arithmetic + arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);
            typeL = expL->getType();
            typeR = expR->getType();

            if(type->isCTypeFloat())
               throw Core::ExceptStr(pos, "float * float stub");

            if(type->isCTypeFixed())
               throw Core::ExceptStr(pos, "fixed * fixed stub");

            if(type->isCTypeInteg())
            {
               return static_cast<AST::Exp::CRef>(
                  new Exp_MulInteg(type, expL, expR, pos));
            }
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator +'");
      }
   }
}

// EOF

