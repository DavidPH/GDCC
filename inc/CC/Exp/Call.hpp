//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ()" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Call_H__
#define GDCC__CC__Exp__Call_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp/Unary.hpp"

#include "../../Core/Array.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Call
      //
      class Exp_Call : public AST::Exp_Unary
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::CC::Exp_Call, GDCC::AST::Exp_Unary);

      public:
         Core::Array<AST::Exp::CRef>       const args;
         Core::CounterRef<AST::Type const> const type;

      protected:
         Exp_Call(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> const &args);
         Exp_Call(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> &&args);

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };

      //
      // Exp_CallStk
      //
      // Handles calling conventions that put all the arguments on the stack.
      //
      class Exp_CallStk : public Exp_Call
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_CallStk, GDCC::CC::Exp_Call);

      public:
         friend AST::Exp::CRef ExpCreate_Call(AST::Exp const *e,
            Core::Array<AST::Exp::CRef> const &args, Core::Origin pos);
         friend AST::Exp::CRef ExpCreate_Call(AST::Exp const *e,
            Core::Array<AST::Exp::CRef>      &&args, Core::Origin pos);

      protected:
         Exp_CallStk(AST::Exp const *e, Core::Origin pos_,
            Core::Array<AST::Exp::CRef> const &a) : Super{e, pos_, a} {}
         Exp_CallStk(AST::Exp const *e, Core::Origin pos_,
            Core::Array<AST::Exp::CRef> &&a) : Super{e, pos_, std::move(a)} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;
      };
   }
}

#endif//GDCC__CC__Exp__Call_H__

