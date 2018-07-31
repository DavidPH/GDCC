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
   // Exp_IRExp constructor
   //
   Exp_IRExp::Exp_IRExp(IR::Exp const *exp_, Type const *type_,
      Core::Origin pos_) :
      Super{pos_}, exp{exp_}, type{type_}
   {
   }

   //
   // Exp_IRExp destructor
   //
   Exp_IRExp::~Exp_IRExp()
   {
   }

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
   // ExpCreate_IRExp
   //
   Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type)
   {
      return static_cast<Exp::CRef>(new Exp_IRExp(exp, type, exp->pos));
   }

   //
   // ExpCreate_IRExp
   //
   Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type,
      Core::Origin pos)
   {
      return static_cast<Exp::CRef>(new Exp_IRExp(exp, type, pos));
   }

   //
   // ExpCreate_Size
   //
   Exp::CRef ExpCreate_Size(Core::FastU size)
   {
      return ExpCreate_IRExp(
         IR::ExpCreate_Value(
            IR::Value_Fixed(Core::NumberCast<Core::Integ>(size),
               Type::Size->getIRType().tFixed), {}),
         Type::Size);
   }
}

// EOF

