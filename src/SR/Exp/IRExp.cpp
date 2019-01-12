//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation IR expression values.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/IRExp.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Exp_IRExp::v_genStmnt
   //
   void Exp_IRExp::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
   {
      GenStmnt_Move(this, ctx, dst, getArg());
   }

   //
   // Exp_IRExp::v_getIRExp
   //
   IR::Exp::CRef Exp_IRExp::v_getIRExp() const
   {
      return exp;
   }

   //
   // Exp_IRExp::v_getType
   //
   Type::CRef Exp_IRExp::v_getType() const
   {
      return type;
   }

   //
   // Exp_IRExp::Create_Size
   //
   Exp_IRExp::CRef Exp_IRExp::Create_Size(Core::FastU size)
   {
      auto e = IR::ExpCreate_Value(
         IR::Value_Fixed(Core::NumberCast<Core::Integ>(size),
            Type::Size->getIRType().tFixed), {});

      return Create(e, Type::Size, e->pos);
   }
}

// EOF

