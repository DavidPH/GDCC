//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C __va_arg expression.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/VaArg.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_VaArg::v_genStmnt
      //
      void Exp_VaArg::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         GenStmnt_Move(this, ctx, dst, getArg());
      }

      //
      // Exp_VaArg::v_getArg
      //
      AST::Arg Exp_VaArg::v_getArg() const
      {
         // *--*(type __sta *__adr_cpy *)&exp

         auto typeArg = type->getTypeQual({{IR::AddrBase::Sta, Core::STR_}});
         auto typeCst = typeArg->getTypePointer()
            ->getTypeQual({exp->getType()->getQualAddr()})->getTypePointer();

         auto expPtr = ExpConvert_Pointer(typeCst, exp->getArgDup().data, pos);

         return {typeArg, ExpCreate_DecPre(ExpCreate_Deref(expPtr, pos), pos)};
      }

      //
      // Exp_VaArg::v_isIRExp
      //
      bool Exp_VaArg::v_isIRExp() const
      {
         return false;
      }

      //
      // ExpCreate_VaArg
      //
      AST::Exp::CRef ExpCreate_VaArg(AST::Type const *type,
         AST::Exp const *exp, Core::Origin pos)
      {
         return Exp_VaArg::Create(type, exp, pos);
      }
   }
}

// EOF

