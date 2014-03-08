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

#include "../../AST/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Sub
      //
      class Exp_Sub : public AST::Exp_Binary
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::CC::Exp_Sub, GDCC::AST::Exp_Binary);

      public:
         TypeCRef const type;

      protected:
         Exp_Sub(AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
            Core::Origin pos);
         virtual ~Exp_Sub();

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;
      };

      //
      // Exp_SubInteg
      //
      class Exp_SubInteg : public Exp_Sub
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_SubInteg, GDCC::CC::Exp_Sub);

      public:
         friend AST::Exp::CRef ExpCreate_Sub(AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos);

      protected:
         Exp_SubInteg(AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
            Core::Origin pos_) : Super{t, l, r, pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };

      //
      // Exp_SubPtrInt
      //
      class Exp_SubPtrInt : public Exp_Sub
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_SubPtrInt, GDCC::CC::Exp_Sub);

      public:
         friend AST::Exp::CRef ExpCreate_Sub(AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos);

      protected:
         Exp_SubPtrInt(AST::Type const *t, AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos_) : Super{t, l, r, pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };

      //
      // Exp_SubPtrPtrW
      //
      class Exp_SubPtrPtrW : public Exp_Sub
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_SubPtrPtrW, GDCC::CC::Exp_Sub);

      public:
         friend AST::Exp::CRef ExpCreate_Sub(AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos);

      protected:
         Exp_SubPtrPtrW(AST::Type const *t, AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos_) : Super{t, l, r, pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };
   }
}

#endif//GDCC__CC__Exp__Add_H__

