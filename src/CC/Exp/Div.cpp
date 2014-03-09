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

#include "AST/Exp/Arith.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Code.hpp"


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

         auto typeL = l->getType();
         auto typeR = r->getType();

         // arithmetic / arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);
            typeL = expL->getType();
            typeR = expR->getType();

            if(type->isCTypeFloat())
               throw Core::ExceptStr(pos, "float / float stub");

            if(type->isCTypeFixed())
               throw Core::ExceptStr(pos, "fixed / fixed stub");

            if(type->isCTypeInteg())
            {
               if(type->getSizeWords() == 1)
               {
                  return AST::Exp_Arith<AST::Exp_Mul>::Create(
                     type->getSizeBitsS() ? IR::Code::DivI_W : IR::Code::DivU_W,
                     type, expL, expR, pos);
               }

               throw Core::ExceptStr(pos, "integer / integer stub");
            }
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator /'");
      }
   }
}

// EOF

