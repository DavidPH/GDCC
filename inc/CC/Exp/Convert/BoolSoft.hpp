//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C soft boolean conversion expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Convert__SoftBool_H__
#define GDCC__CC__Exp__Convert__SoftBool_H__

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
      // Exp_ConvertBoolSoft_Fixed
      //
      // Converts fixed-point and integer types to a single-word soft boolean.
      //
      class Exp_ConvertBoolSoft_Fixed : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertBoolSoft_Fixed, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertBoolSoft_Fixed);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };

      //
      // Exp_ConvertBoolSoft_Float
      //
      // Converts floating-point types to a single-word soft boolean.
      //
      class Exp_ConvertBoolSoft_Float : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertBoolSoft_Float, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertBoolSoft_Float);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };

      //
      // Exp_ConvertBoolSoft_PtrInv
      //
      // Converts nonzero null pointer types to a single-word soft boolean.
      //
      class Exp_ConvertBoolSoft_PtrInv : public SR::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertBoolSoft_PtrInv, GDCC::SR::Exp_Convert);

         GDCC_SR_Exp_UnaryCreator(Exp_ConvertBoolSoft_PtrInv);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };
   }
}

#endif//GDCC__CC__Exp__Convert__SoftBool_H__

