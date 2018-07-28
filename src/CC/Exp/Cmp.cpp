//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

namespace GDCC::CC
{
   //
   // ExpCreate_CmpEQ
   //
   SR::Exp::CRef ExpCreate_CmpEQ(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = ExpPromo_CmpEqu(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpEQ, IR::CodeSet_CmpEQ>(
            TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator =='");
   }

   //
   // ExpCreate_CmpGE
   //
   SR::Exp::CRef ExpCreate_CmpGE(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = ExpPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpGE, IR::CodeSet_CmpGE>(
            TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // ExpCreate_CmpGT
   //
   SR::Exp::CRef ExpCreate_CmpGT(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = ExpPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpGT, IR::CodeSet_CmpGT>(
            TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // ExpCreate_CmpLE
   //
   SR::Exp::CRef ExpCreate_CmpLE(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = ExpPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpLE, IR::CodeSet_CmpLE>(
            TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // ExpCreate_CmpLT
   //
   SR::Exp::CRef ExpCreate_CmpLT(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = ExpPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpLT, IR::CodeSet_CmpLT>(
            TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // ExpCreate_CmpNE
   //
   SR::Exp::CRef ExpCreate_CmpNE(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = ExpPromo_CmpEqu(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpNE, IR::CodeSet_CmpNE>(
            TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator !='");
   }
}

// EOF

