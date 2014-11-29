//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree Arg values.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Arg.hpp"

#include "AST/Type.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Arg constructor
      //
      Exp_Arg::Exp_Arg(Arg const &arg_, Core::Origin pos_) :
         Super{pos_}, arg{arg_}
      {
      }

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
         return arg.type->getQualAddr().base != IR::AddrBase::Loc;
      }

      //
      // ExpCreate_Arg
      //
      Exp::CRef ExpCreate_Arg(Arg const &arg, Core::Origin pos)
      {
         return static_cast<Exp::CRef>(new Exp_Arg(arg, pos));
      }
   }
}

// EOF

