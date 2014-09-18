//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_CmpEQ
      //
      AST::Exp::CRef ExpCreate_CmpEQ(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpEqu(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<AST::Exp_CmpEQ, IR::CodeSet_CmpEQ>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator =='");
      }

      //
      // ExpCreate_CmpGE
      //
      AST::Exp::CRef ExpCreate_CmpGE(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<AST::Exp_CmpGE, IR::CodeSet_CmpGE>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpGT
      //
      AST::Exp::CRef ExpCreate_CmpGT(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<AST::Exp_CmpGT, IR::CodeSet_CmpGT>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpLE
      //
      AST::Exp::CRef ExpCreate_CmpLE(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<AST::Exp_CmpLE, IR::CodeSet_CmpLE>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpLT
      //
      AST::Exp::CRef ExpCreate_CmpLT(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpRel(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<AST::Exp_CmpLT, IR::CodeSet_CmpLT>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator >='");
      }

      //
      // ExpCreate_CmpNE
      //
      AST::Exp::CRef ExpCreate_CmpNE(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto promo = ExpPromo_CmpEqu(l, r, pos);
         auto &type = std::get<0>(promo);
         auto &expL = std::get<1>(promo);
         auto &expR = std::get<2>(promo);

         if(type->isCTypeArith() || type->isTypePointer())
            return ExpCreate_CmpArith<AST::Exp_CmpNE, IR::CodeSet_CmpNE>(
               TypeIntegPrS, type, expL, expR, pos);

         throw Core::ExceptStr(pos, "invalid operands to 'operator !='");
      }
   }
}

// EOF

