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

#include "../../AST/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Add
      //
      class Exp_Add : public AST::Exp_Binary
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::CC::Exp_Add, GDCC::AST::Exp_Binary);

      public:
         TypeCRef const type;

      protected:
         Exp_Add(AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
            Core::Origin pos);
         virtual ~Exp_Add();

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;
      };

      //
      // Exp_AddInteg
      //
      class Exp_AddInteg : public Exp_Add
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_AddInteg, GDCC::CC::Exp_Add);

      public:
         friend AST::Exp::CRef ExpCreate_Add(AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos);

      protected:
         Exp_AddInteg(AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
            Core::Origin pos_) : Super{t, l, r, pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };

      //
      // Exp_AddPoint
      //
      class Exp_AddPoint : public Exp_Add
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_AddPoint, GDCC::CC::Exp_Add);

      public:
         friend AST::Exp::CRef ExpCreate_Add(AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos);

      protected:
         Exp_AddPoint(AST::Type const *t, AST::Exp const *l, AST::Exp const *r,
            Core::Origin pos_) : Super{t, l, r, pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };
   }
}

#endif//GDCC__CC__Exp__Add_H__

