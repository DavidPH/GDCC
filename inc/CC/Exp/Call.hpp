//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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
      class Scope_Local;

      //
      // Exp_Call
      //
      class Exp_Call : public AST::Exp_Unary
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::CC::Exp_Call, GDCC::AST::Exp_Unary);

      public:
         Core::Array<AST::Exp::CRef> const args;
         TypeCRef                    const func;

      protected:
         Exp_Call(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> const &args);
         Exp_Call(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> &&args);

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;

         virtual bool v_isNoAuto() const;
      };

      //
      // Exp_CallLit
      //
      // Handles calling conventions that use all Lit arguments.
      //
      class Exp_CallLit : public Exp_Call
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_CallLit, GDCC::CC::Exp_Call);

      public:
         // Create
         static CRef Create(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> const &a)
            {return CRef(new This(e, pos, a));}

         // Create
         static CRef Create(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> &&a)
            {return CRef(new This(e, pos, std::move(a)));}

      protected:
         Exp_CallLit(AST::Exp const *e, Core::Origin pos_,
            Core::Array<AST::Exp::CRef> const &a) : Super{e, pos_, a} {}
         Exp_CallLit(AST::Exp const *e, Core::Origin pos_,
            Core::Array<AST::Exp::CRef> &&a) : Super{e, pos_, std::move(a)} {}

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
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
         // Create
         static CRef Create(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> const &a, Scope_Local &scope)
            {return CRef(new This(e, pos, a, scope));}

         // Create
         static CRef Create(AST::Exp const *e, Core::Origin pos,
            Core::Array<AST::Exp::CRef> &&a, Scope_Local &scope)
            {return CRef(new This(e, pos, std::move(a), scope));}

      protected:
         Exp_CallStk(AST::Exp const *e, Core::Origin pos_,
            Core::Array<AST::Exp::CRef> const &a, Scope_Local &scope_) :
            Super{e, pos_, a}, scope(scope_) {}
         Exp_CallStk(AST::Exp const *e, Core::Origin pos_,
            Core::Array<AST::Exp::CRef> &&a, Scope_Local &scope_) :
            Super{e, pos_, std::move(a)}, scope(scope_) {}

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         Scope_Local &scope;
      };
   }
}

#endif//GDCC__CC__Exp__Call_H__

