//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "../../AST/Exp/Arith.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_AddPoint
      //
      class Exp_AddPoint : public AST::Exp_Add
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_AddPoint, GDCC::AST::Exp_Add);

         GDCC_AST_Exp_BinaryCreator(Exp_AddPoint);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
      };

      //
      // Exp_AddPtrRaw
      //
      class Exp_AddPtrRaw : public AST::Exp_Add
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_AddPtrRaw, GDCC::AST::Exp_Add);

         GDCC_AST_Exp_BinaryCreator(Exp_AddPtrRaw);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Add_H__

