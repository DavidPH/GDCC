//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree arithmetic expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Arith_H__
#define GDCC__AST__Exp__Arith_H__

#include "../../AST/Exp/Binary.hpp"

#include "../../AST/Type.hpp"

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_AST_Exp_ArithPreamble
//
// Performs common declarations for all arithmetic base classes.
//
#define GDCC_AST_Exp_ArithPreamble(class) \
   GDCC_Core_CounterPreambleAbstract( \
      GDCC::AST::class, GDCC::AST::Exp_Binary); \
   \
protected: \
   class(Type const *t, Exp const *l, Exp const *r, \
      Core::Origin pos_) : Super{t, l, r, pos_} {} \
   \
   virtual IRExpCRef v_getIRExp() const


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Arith
      //
      // Implements statement generation as a call to GenStmnt_Arith. Uses a
      // template base class to implement other expression properties.
      //
      template<typename Base>
      class Exp_Arith : public Base
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_Arith<Base>, Base);

      public:
         IR::Code const code;


         //
         // Create
         //
         static Exp::CRef Create(IR::Code code, Type const *t, Exp const *l,
            Exp const *r, Core::Origin pos)
         {
            return Exp::CRef(new Exp_Arith<Base>(code, t, l, r, pos));
         }

      protected:
         Exp_Arith(IR::Code c, Type const *t, Exp const *l, Exp const *r,
            Core::Origin pos_) : Super{t, l, r, pos_}, code{c} {}

         // v_genStmnt
         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
            {GenStmnt_Arith(this, code, ctx, dst);}
      };

      //
      // Exp_ArithEq
      //
      // Implements statement generation as a call to GenStmnt_ArithEq. Uses a
      // template base class to implement other expression properties.
      //
      template<typename Base>
      class Exp_ArithEq : public Base
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_ArithEq<Base>, Base);

      public:
         Type::CRef const evalT;
         IR::Code   const code;
         bool       const post : 1;


         // Create
         static CRef Create(Type const *evalT, IR::Code code, bool post,
            Type const *t, Exp const *l, Exp const *r, Core::Origin pos)
            {return CRef(new This(evalT, code, post, t, l, r, pos));}

      protected:
         Exp_ArithEq(Type const *evalT_, IR::Code c, bool post_, Type const *t,
            Exp const *l, Exp const *r, Core::Origin pos_) :
            Super{t, l, r, pos_}, evalT{evalT_}, code{c}, post{post_} {}

         // v_genStmnt
         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
            {GenStmnt_ArithEq(this, code, ctx, dst, evalT, post);}

         // v_getIRExp
         virtual IR::Exp::CRef v_getIRExp() const
            {return post ? Super::expL->getIRExp() : Super::v_getIRExp();}

         // v_isEffect
         virtual bool v_isEffect() const {return true;}

         // v_isIRExp
         virtual bool v_isIRExp() const
            {return post ? Super::expL->isIRExp() : Super::v_isIRExp();}
      };

      //
      // Exp_Add
      //
      class Exp_Add : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_Add);
      };

      //
      // Exp_Div
      //
      class Exp_Div : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_Div);
      };

      //
      // Exp_Mod
      //
      class Exp_Mod : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_Mod);
      };

      //
      // Exp_Mul
      //
      class Exp_Mul : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_Mul);
      };

      //
      // Exp_Sub
      //
      class Exp_Sub : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_Sub);
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
      // Does generic codegen centered around a 3-arg arithmetic instruction.
      void GenStmnt_Arith(Exp_Binary const *exp, IR::Code code,
         GenStmntCtx const &ctx, Arg const &dst);

      // As in GenStmnt_Arith, but also assigns the result to the left operand.
      void GenStmnt_ArithEq(Exp_Binary const *exp, IR::Code code,
         GenStmntCtx const &ctx, Arg const &dst, Type const *evalT,
         bool post = false);
   }
}

#endif//GDCC__AST__Exp__Arith_H__

