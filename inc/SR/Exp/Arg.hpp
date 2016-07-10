//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation Arg values.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__Arg_H__
#define GDCC__SR__Exp__Arg_H__

#include "../../SR/Exp.hpp"

#include "../../SR/Arg.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Exp_Arg
      //
      class Exp_Arg : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::SR::Exp_Arg, GDCC::SR::Exp);

      public:
         Arg const arg;


         friend Exp::CRef ExpCreate_Arg(Arg const &arg, Core::Origin pos);

      protected:
         Exp_Arg(Arg const &arg, Core::Origin pos);

         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual Arg v_getArg() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;

         virtual bool v_isNoAuto() const;
      };
   }
}

#endif//GDCC__SR__Exp__Arg_H__

