//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C pointer conversion expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Convert__Pointer_H__
#define GDCC__CC__Exp__Convert__Pointer_H__

#include "../../../CC/Exp.hpp"

#include "../../../SR/Exp/Convert.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_ConvertPtr
      //
      // Changes the type of a pointer without bitwise conversion.
      //
      class Exp_ConvertPtr : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtr, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertPtr);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;
      };

      //
      // Exp_ConvertPtrInt
      //
      // Converts between integer and pointer.
      //
      class Exp_ConvertPtrInt : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtrInt, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertPtrInt);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual bool v_isIRExp() const;
      };

      //
      // Exp_ConvertPtrInv
      //
      // Converts between address spaces with inverted nulls.
      //
      class Exp_ConvertPtrInv : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtrInv, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertPtrInv);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;
      };

      //
      // Exp_ConvertPtrLoc
      //
      // Converts a pointer-to-loc to a pointer-to-loc_ars.
      //
      class Exp_ConvertPtrLoc : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtrLoc, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertPtrLoc);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };

      //
      // Exp_ConvertPtrSh
      //
      class Exp_ConvertPtrSh : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtrSh, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertPtrSh);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };
   }
}

#endif//GDCC__CC__Exp__Convert__Pointer_H__

