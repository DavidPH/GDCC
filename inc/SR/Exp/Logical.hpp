//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation logical expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__Logical_H__
#define GDCC__SR__Exp__Logical_H__

#include "../../SR/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Exp_LogAnd
      //
      class Exp_LogAnd : public Exp_Binary
      {
         GDCC_Core_CounterPreamble(
            GDCC::SR::Exp_LogAnd, GDCC::SR::Exp_Binary);

         GDCC_SR_Exp_BinaryCreator(Exp_LogAnd);

      protected:
         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual bool v_isBoolean() const;

         virtual bool v_isIRExp() const;
      };

      //
      // Exp_LogOrI
      //
      class Exp_LogOrI : public Exp_Binary
      {
         GDCC_Core_CounterPreamble(
            GDCC::SR::Exp_LogOrI, GDCC::SR::Exp_Binary);

         GDCC_SR_Exp_BinaryCreator(Exp_LogOrI);

      protected:
         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual bool v_isBoolean() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__SR__Exp__Logical_H__

