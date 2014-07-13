//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "../../AST/Exp/Arith.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_SubPtrInt
      //
      class Exp_SubPtrInt : public AST::Exp_Sub
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_SubPtrInt, GDCC::AST::Exp_Sub);

         GDCC_AST_Exp_BinaryCreator(Exp_SubPtrInt);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
      };

      //
      // Exp_SubPtrPtrW
      //
      class Exp_SubPtrPtrW : public AST::Exp_Sub
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_SubPtrPtrW, GDCC::AST::Exp_Sub);

         GDCC_AST_Exp_BinaryCreator(Exp_SubPtrPtrW);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
      };
   }
}

#endif//GDCC__CC__Exp__Add_H__

