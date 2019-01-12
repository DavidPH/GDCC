//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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

namespace GDCC::SR
{
   //
   // Exp_Arg
   //
   class Exp_Arg : public Exp
   {
      GDCC_Core_CounterPreamble(GDCC::SR::Exp_Arg, GDCC::SR::Exp);

   public:
      Arg const arg;


      static CRef Create(Arg const &arg, Core::Origin pos)
         {return CRef(new This(arg, pos));}

   protected:
      Exp_Arg(Arg const &arg_, Core::Origin pos_) :
         Super{pos_}, arg{arg_} {}

      virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

      virtual Arg v_getArg() const;

      virtual IRExpCRef v_getIRExp() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;
   };
}

#endif//GDCC__SR__Exp__Arg_H__

