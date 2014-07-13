//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Deref.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Deref::v_genStmnt
      //
      void Exp_Deref::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         GenStmnt_Move(this, ctx, dst, getArg());
      }

      //
      // Exp_Deref::v_getArg
      //
      AST::Arg Exp_Deref::v_getArg() const
      {
         return AST::Arg(type, exp);
      }

      //
      // Exp_Deref::v_isEffect
      //
      bool Exp_Deref::v_isEffect() const
      {
         return type->getQualVola();
      }

      //
      // Exp_Deref::v_isIRExp
      //
      bool Exp_Deref::v_isIRExp() const
      {
         return false;
      }

      //
      // ExpCreate_Deref
      //
      AST::Exp::CRef ExpCreate_Deref(AST::Exp const *e, Core::Origin pos)
      {
         auto exp  = ExpPromo_LValue(e, pos);
         auto type = exp->getType();

         if(!type->isTypePointer())
            throw Core::ExceptStr(pos, "expected pointer");

         type = type->getBaseType();

         return Exp_Deref::Create(type, exp, pos);
      }
   }
}

// EOF

