//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C initializer expression.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Init_H__
#define GDCC__CC__Exp__Init_H__

#include "../../CC/Types.hpp"

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_Init
   //
   class Exp_Init : public SR::Exp
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_Init, GDCC::SR::Exp);

   public:
      std::shared_ptr<Init> const init;
      bool                  const skipZero;


      static CRef Create(std::unique_ptr<Init> &&init, bool skipZero);

      static CRef Create(std::unique_ptr<Init> &&init, bool skipZero,
         Core::Origin pos);

   protected:
      Exp_Init(std::unique_ptr<Init> &&init, bool skipZero, Core::Origin pos);
      virtual ~Exp_Init();

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual IRExpCRef v_getIRExp() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isGenStmnt() const {return true;}

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;
   };
}

#endif//GDCC__CC__Exp__Init_H__

