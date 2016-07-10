//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C relational and equality expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Cmp.hpp"

#include "CC/Type.hpp"

#include "IR/CodeSet/Cmp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_CmpEQ
      //
      SR::Exp::CRef ExpCreate_CmpEQ(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpEqu(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<SR::Exp_CmpEQ, IR::CodeSet_CmpEQ>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator =='");
      }

      //
      // ExpCreate_CmpGE
      //
      SR::Exp::CRef ExpCreate_CmpGE(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<SR::Exp_CmpGE, IR::CodeSet_CmpGE>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpGT
      //
      SR::Exp::CRef ExpCreate_CmpGT(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<SR::Exp_CmpGT, IR::CodeSet_CmpGT>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpLE
      //
      SR::Exp::CRef ExpCreate_CmpLE(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<SR::Exp_CmpLE, IR::CodeSet_CmpLE>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpLT
      //
      SR::Exp::CRef ExpCreate_CmpLT(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<SR::Exp_CmpLT, IR::CodeSet_CmpLT>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpNE
      //
      SR::Exp::CRef ExpCreate_CmpNE(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpEqu(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<SR::Exp_CmpNE, IR::CodeSet_CmpNE>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator !='");
      }
   }
}

// EOF

