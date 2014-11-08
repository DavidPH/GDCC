//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree conversion expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Convert_H__
#define GDCC__AST__Exp__Convert_H__

#include "../../AST/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Convert
      //
      class Exp_Convert : public Exp_Unary
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Exp_Convert, GDCC::AST::Exp_Unary);

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
            GDCC::AST::Exp_ConvertArith, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertArith);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      // Converts an arithmetic value from srcT to dstT on the stack.
      void GenStmnt_ConvertArith(Exp const *exp, Type const *dstT,
         Type const *srcT, GenStmntCtx const &ctx);

      // Converts a fixed-point value to a floating-point type on the stack.
      void GenStmnt_ConvertFixFlt(Exp const *exp, Type const *dstT,
         Type const *srcT, GenStmntCtx const &ctx);

      // Converts a fixed-point value to a fixed-point type on the stack.
      void GenStmnt_ConvertFixed(Exp const *exp, Type const *dstT,
         Type const *srcT, GenStmntCtx const &ctx);

      // Converts a floating-point value to a floating-point type on the stack.
      void GenStmnt_ConvertFloat(Exp const *exp, Type const *dstT,
         Type const *srcT, GenStmntCtx const &ctx);

      // Converts a floating-point value to a fixed-point type on the stack.
      void GenStmnt_ConvertFltFix(Exp const *exp, Type const *dstT,
         Type const *srcT, GenStmntCtx const &ctx);
   }
}

#endif//GDCC__AST__Exp__Convert_H__

