//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Deref_H__
#define GDCC__CC__Exp__Deref_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Deref
      //
      class Exp_Deref : public AST::Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Deref, GDCC::AST::Exp_Unary);

      public:
         friend AST::Exp::CRef ExpCreate_Deref(AST::Exp const *e,
            Core::Origin pos);

      protected:
         Exp_Deref(AST::Type const *t, AST::Exp const *e, Core::Origin pos_) :
            Super{t, e, pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;

         virtual AST::Arg v_getArg() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Deref_H__

