//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation Arg values.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Arg.hpp"

#include "SR/Type.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Exp_Arg::v_genStmnt
   //
   void Exp_Arg::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
   {
      GenStmnt_Move(this, ctx, dst, arg);
   }

   //
   // Exp_Arg::v_getArg
   //
   Arg Exp_Arg::v_getArg() const
   {
      return arg;
   }

   //
   // Exp_Arg::v_getIRExp
   //
   IR::Exp::CRef Exp_Arg::v_getIRExp() const
   {
      if(arg.type->getQualAddr().base == IR::AddrBase::Lit)
         return arg.data->getIRExp();

      return Exp::v_getIRExp();
   }

   //
   // Exp_Arg::v_getType
   //
   Type::CRef Exp_Arg::v_getType() const
   {
      return arg.type;
   }

   //
   // Exp_Arg::v_isEffect
   //
   bool Exp_Arg::v_isEffect() const
   {
      return arg.data->isEffect();
   }

   //
   // Exp_Arg::v_isIRExp
   //
   bool Exp_Arg::v_isIRExp() const
   {
      if(arg.type->getQualAddr().base == IR::AddrBase::Lit)
         return arg.data->isIRExp();

      return false;
   }

   //
   // Exp_Arg::v_isNoAuto
   //
   bool Exp_Arg::v_isNoAuto() const
   {
      return arg.type->getQualAddr().base != IR::AddrBase::Aut;
   }
}

// EOF

