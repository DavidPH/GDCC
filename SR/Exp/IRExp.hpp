//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
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


      static CRef Create(IR::Exp const *exp, Type const *type, Core::Origin pos)
         {return CRef(new This(exp, type, pos));}

      static CRef Create_Size(Core::FastU size);

   protected:
      Exp_IRExp(IR::Exp const *exp_, Type const *type_, Core::Origin pos_) :
         Super{pos_}, exp{exp_}, type{type_} {}

      virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

      virtual IRExpCRef v_getIRExp() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const {return false;}

      virtual bool v_isIRExp() const {return true;}

      virtual bool v_isNoAuto() const {return true;}
   };
}

#endif//GDCC__SR__Exp__IRExp_H__

