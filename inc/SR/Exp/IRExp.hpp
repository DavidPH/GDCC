//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation IR expression values.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__IRExp_H__
#define GDCC__SR__Exp__IRExp_H__

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Exp_IRExp
      //
      class Exp_IRExp : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::SR::Exp_IRExp, GDCC::SR::Exp);

      public:
         IRExpCRef const exp;
         TypeCRef  const type;


         friend Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type);
         friend Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type,
            Core::Origin pos);

      protected:
         Exp_IRExp(IR::Exp const *exp, Type const *type, Core::Origin pos);
         virtual ~Exp_IRExp();

         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const {return false;}

         virtual bool v_isIRExp() const {return true;}

         virtual bool v_isNoAuto() const {return true;}
      };
   }
}

#endif//GDCC__SR__Exp__IRExp_H__

