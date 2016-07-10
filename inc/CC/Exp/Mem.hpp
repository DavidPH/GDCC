//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ." and "operator ->" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Mem_H__
#define GDCC__CC__Exp__Mem_H__

#include "../../CC/Exp.hpp"

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Mem
      //
      class Exp_Mem : public SR::Exp
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Mem, GDCC::SR::Exp);

      public:
         SR::Exp::CRef const expL;
         Core::String  const expR;


         // Create
         static CRef Create(SR::Exp const *l, Core::String r, Core::Origin pos)
            {return CRef(new This(l, r, pos));}

      protected:
         Exp_Mem(SR::Exp const *l, Core::String r, Core::Origin pos_) :
            Super{pos_}, expL{l}, expR{r} {}

         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual SR::Arg v_getArg() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const {return false;}

         virtual bool v_isNoAuto() const;

         virtual void v_setRefer() const;
      };
   }
}

#endif//GDCC__CC__Exp__Mem_H__

