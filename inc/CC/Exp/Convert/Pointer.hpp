//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "../../../AST/Exp/Convert.hpp"


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
      class Exp_ConvertPtr : public AST::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtr, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertPtr);

      protected:
         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };

      //
      // Exp_ConvertPtrInv
      //
      // Converts between address spaces with inverted nulls.
      //
      class Exp_ConvertPtrInv : public AST::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtrInv, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertPtrInv);

      protected:
         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };

      //
      // Exp_ConvertPtrLoc
      //
      // Converts a pointer-to-loc to a pointer-to-loc_ars.
      //
      class Exp_ConvertPtrLoc : public AST::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtrLoc, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertPtrLoc);

      protected:
         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };

      //
      // Exp_ConvertPtrSh
      //
      class Exp_ConvertPtrSh : public AST::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertPtrSh, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertPtrSh);

      protected:
         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };
   }
}

#endif//GDCC__CC__Exp__Convert__Pointer_H__

