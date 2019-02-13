//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation conversion expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__Convert_H__
#define GDCC__SR__Exp__Convert_H__

#include "../../SR/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Exp_Convert
   //
   class Exp_Convert : public Exp_Unary
   {
      GDCC_Core_CounterPreambleAbstract(
         GDCC::SR::Exp_Convert, GDCC::SR::Exp_Unary);

   protected:
      Exp_Convert(Type const *t, Exp const *e, Core::Origin pos_) :
         Super{t, e, pos_} {}

      virtual IRExpCRef v_getIRExp() const;
   };

   //
   // Exp_ConvertArith
   //
   class Exp_ConvertArith : public Exp_Convert
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Exp_ConvertArith, GDCC::SR::Exp_Convert);

      GDCC_SR_Exp_UnaryCreator(Exp_ConvertArith);

   protected:
      virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;
   };

   //
   // Exp_ConvertBitfield
   //
   class Exp_ConvertBitfield : public Exp_Convert
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Exp_ConvertBitfield, GDCC::SR::Exp_Convert);

      GDCC_SR_Exp_UnaryCreator(Exp_ConvertBitfield);

   protected:
      virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   // Converts an arithmetic value from srcT to dstT on the stack.
   void GenStmnt_ConvertArith(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx);

   void GenStmnt_ConvertBool(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx);

   // Converts a fixed-point value to a fixed-point type on the stack.
   void GenStmnt_ConvertFixed(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx);
}

#endif//GDCC__SR__Exp__Convert_H__

