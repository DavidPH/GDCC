//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation unary expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__Unary_H__
#define GDCC__SR__Exp__Unary_H__

#include "../../SR/Exp.hpp"

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_SR_Exp_UnaryCreator
//
// Defines a basic constructor and static Create function for unary expression
// classes.
//
#define GDCC_SR_Exp_UnaryCreator(class) \
public: \
   static ::GDCC::SR::Exp::CRef Create(::GDCC::SR::Type const *t, \
      ::GDCC::SR::Exp const *e, ::GDCC::Core::Origin pos) \
   { \
      return static_cast<SR::Exp::CRef>(new class(t, e, pos)); \
   } \
   \
protected: \
   class(::GDCC::SR::Type const *t, ::GDCC::SR::Exp const *e, \
      ::GDCC::Core::Origin pos_) : Super{t, e, pos_} {}


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Exp_Unary
   //
   class Exp_Unary : public Exp
   {
      GDCC_Core_CounterPreambleAbstract(
         GDCC::SR::Exp_Unary, GDCC::SR::Exp);

   public:
      Exp::CRef const exp;
      TypeCRef  const type;

   protected:
      Exp_Unary(Type const *t, Exp const *e, Core::Origin pos);
      virtual ~Exp_Unary();

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;
   };

   //
   // Exp_UnaryCode
   //
   template<typename Base>
   class Exp_UnaryCode : public Base
   {
      GDCC_Core_CounterPreamble(GDCC::SR::Exp_UnaryCode<Base>, Base);

   public:
      IR::Code const code;


      // Create
      static Exp::CRef Create(IR::Code code, Type const *t, Exp const *e,
         Core::Origin pos)
         {return Exp::CRef(new This(code, t, e, pos));}

   protected:
      // constructor
      Exp_UnaryCode(IR::Code c, Type const *t, Exp const *e, Core::Origin pos_) :
         Super{t, e, pos_}, code{c} {}

      // v_genStmnt
      virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
         {GenStmnt_UnaryCode(this, code, ctx, dst);}
   };

   //
   // Exp_Inv
   //
   class Exp_Inv : public Exp_Unary
   {
      GDCC_Core_CounterPreambleAbstract(
         GDCC::SR::Exp_Inv, GDCC::SR::Exp_Unary);

   protected:
      Exp_Inv(Type const *t, Exp const *e, Core::Origin pos_) :
         Super{t, e, pos_} {}

      virtual IRExpCRef v_getIRExp() const;
   };

   //
   // Exp_Neg
   //
   class Exp_Neg : public Exp_Unary
   {
      GDCC_Core_CounterPreambleAbstract(
         GDCC::SR::Exp_Neg, GDCC::SR::Exp_Unary);

   protected:
      Exp_Neg(Type const *t, Exp const *e, Core::Origin pos_) :
         Super{t, e, pos_} {}

      virtual IRExpCRef v_getIRExp() const;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   // Does generic codegen centered around a 2-arg unary instruction.
   void GenStmnt_UnaryCode(Exp_Unary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst);

   // Returns true if only evaluating for side effects.
   bool GenStmntNul(Exp_Unary const *exp, GenStmntCtx const &ctx,
      Arg const &dst);
}

#endif//GDCC__SR__Exp__Binary_H__

