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
            return static_cast<Exp::CRef>(
               new Exp_Arith<Base>(code, t, l, r, pos));
         }

      protected:
         Exp_Arith(IR::Code c, Type const *t, Exp const *l, Exp const *r,
            Core::Origin pos_) : Super{t, l, r, pos_}, code{c} {}

         //
         // v_genStmnt
         //
         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
         {
            GenStmnt_Arith(this, code, ctx, dst);
         }
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
   }
}

#endif//GDCC__AST__Exp__Arith_H__

