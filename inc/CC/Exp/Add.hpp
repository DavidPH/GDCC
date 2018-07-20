//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator +" expressions.
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
   // Exp_AddPoint
   //
   class Exp_AddPoint : public SR::Exp_Add
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_AddPoint, GDCC::SR::Exp_Add);

      GDCC_SR_Exp_BinaryCreator(Exp_AddPoint);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst) const;
   };

   //
   // Exp_AddPointEq
   //
   class Exp_AddPointEq : public SR::Exp_Add
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Exp_AddPointEq, GDCC::SR::Exp_Add);

   public:
      bool const post : 1;


      static CRef Create(bool post, SR::Type const *t,
         SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
         {return CRef(new This(post, t, l, r, pos));}

   protected:
      Exp_AddPointEq(bool post_, SR::Type const *t, SR::Exp const *l,
         SR::Exp const *r, Core::Origin pos_) :
         Super{t, l, r, pos_}, post{post_} {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isEffect() const {return true;}

      virtual bool v_isIRExp() const;
   };

   //
   // Exp_AddPtrRaw
   //
   class Exp_AddPtrRaw : public SR::Exp_Add
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Exp_AddPtrRaw, GDCC::SR::Exp_Add);

      GDCC_SR_Exp_BinaryCreator(Exp_AddPtrRaw);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual IRExpCRef v_getIRExp() const;
   };

   //
   // Exp_AddStrEntInt
   //
   class Exp_AddStrEntInt : public SR::Exp_Binary
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Exp_AddStrEntInt, GDCC::SR::Exp_Binary);

      GDCC_SR_Exp_BinaryCreator(Exp_AddStrEntInt);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual bool v_isIRExp() const {return false;}
   };
}

#endif//GDCC__CC__Exp__Add_H__

