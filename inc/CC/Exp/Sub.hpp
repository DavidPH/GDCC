//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator -" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Add_H__
#define GDCC__CC__Exp__Add_H__

#include "../../CC/Exp.hpp"

#include "../../SR/Exp/Arith.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_SubPtrInt
   //
   class Exp_SubPtrInt : public SR::Exp_Sub
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Exp_SubPtrInt, GDCC::SR::Exp_Sub);

      GDCC_SR_Exp_BinaryCreator(Exp_SubPtrInt);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;
   };

   //
   // Exp_SubPtrIntEq
   //
   class Exp_SubPtrIntEq : public SR::Exp_Sub
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Exp_SubPtrIntEq, GDCC::SR::Exp_Sub);

   public:
      bool const post : 1;


      static CRef Create(bool post, SR::Type const *t,
         SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
         {return CRef(new This(post, t, l, r, pos));}

   protected:
      Exp_SubPtrIntEq(bool post_, SR::Type const *t, SR::Exp const *l,
         SR::Exp const *r, Core::Origin pos_) :
         Super{t, l, r, pos_}, post{post_} {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isEffect() const {return true;}

      virtual bool v_isIRExp() const;
   };

   //
   // Exp_SubPtrPtrW
   //
   class Exp_SubPtrPtrW : public SR::Exp_Sub
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Exp_SubPtrPtrW, GDCC::SR::Exp_Sub);

      GDCC_SR_Exp_BinaryCreator(Exp_SubPtrPtrW);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;
   };
}

#endif//GDCC__CC__Exp__Add_H__

