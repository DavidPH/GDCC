//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C binary "operator +" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Mul_H__
#define GDCC__CC__Exp__Mul_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Mul
      //
      class Exp_Mul : public AST::Exp_Binary
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::CC::Exp_Mul, GDCC::AST::Exp_Binary);

      public:
         TypeCRef const type;

      protected:
         Exp_Mul(AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
            Core::Origin pos);
         virtual ~Exp_Mul();

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;
      };

      //
      // Exp_MulInteg
      //
      class Exp_MulInteg : public Exp_Mul
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_MulInteg, GDCC::CC::Exp_Mul);

      public:
         friend AST::Exp::CRef ExpCreate_Mul(AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos);

      protected:
         Exp_MulInteg(AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
            Core::Origin pos_) : Super{t, l, r, pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };
   }
}

#endif//GDCC__CC__Exp__Mul_H__

