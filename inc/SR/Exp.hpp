//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp_H__
#define GDCC__SR__Exp_H__

#include "../SR/GenStmnt.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"
#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Exp;
   }

   namespace SR
   {
      class Arg;
      class Function;
      class Object;
      class Type;

      //
      // Exp
      //
      class Exp : public Core::Counter
      {
         GDCC_Core_CounterPreambleAbstract(GDCC::SR::Exp, GDCC::Core::Counter);

      protected:
         using FunctionRef = Core::CounterRef<Function>;
         using IRExpCPtr   = Core::CounterPtr<IR::Exp const>;
         using IRExpCRef   = Core::CounterRef<IR::Exp const>;
         using ObjectRef   = Core::CounterRef<Object>;
         using TypeCRef    = Core::CounterRef<Type const>;

      public:
         void genStmnt(GenStmntCtx const &ctx) const;

         void genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         void genStmntStk(GenStmntCtx const &ctx) const;

         Arg getArg() const;
         Arg getArgDst() const;
         Arg getArgDup() const;
         Arg getArgSrc() const;

         FunctionRef getFunction() const;

         IRExpCRef getIRExp() const;

         ObjectRef getObject() const;

         TypeCRef getType() const;

         // Does the result of this excpression have boolean representation?
         bool isBoolean() const;

         // Does this expression have side effects?
         bool isEffect() const;

         // Does this expression designate a function?
         bool isFunction() const;

         // Can this expression be made into an IR expression?
         bool isIRExp() const;

         // Is this expression known to not use automatic storage?
         bool isNoAuto() const;

         // Is this expression known to be nonzero?
         bool isNonzero() const;

         // Does this expression designate an AST::Object?
         bool isObject() const;

         // Is this expression known to be zero?
         bool isZero() const;

         // Marks the expression as needing its address taken.
         void setRefer() const;

         Core::Origin const pos;

      protected:
         Exp(Exp const &exp);
         explicit Exp(Core::Origin pos);
         virtual ~Exp();

         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const = 0;

         virtual Arg v_getArg() const;

         virtual FunctionRef v_getFunction() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual ObjectRef v_getObject() const;

         virtual TypeCRef v_getType() const = 0;

         virtual bool v_isBoolean() const;

         virtual bool v_isEffect() const = 0;

         virtual bool v_isFunction() const;

         // If this returns false, genStmnt will try to avoid calling
         // v_genStmnt if there are no side effects.
         virtual bool v_isGenStmnt() const;

         virtual bool v_isIRExp() const = 0;

         virtual bool v_isNoAuto() const = 0;

         virtual bool v_isObject() const;

         virtual void v_setRefer() const;

      private:
         mutable IRExpCPtr cacheIRExp;

         mutable int cacheIsIRExp;
      };
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      Exp::CRef ExpCreate_Arg(Arg const &arg, Core::Origin pos);

      Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type);
      Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type,
         Core::Origin pos);

      Exp::CRef ExpCreate_MulSize(Exp const *l, Exp const *r);

      Exp::CRef ExpCreate_Size(Core::FastU value);

      // Moves data from one Arg to another.
      void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &dst, Arg const &src);

      // Moves data from Arg to two others. (dst = dup = src)
      void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &dst, Arg const &dup, Arg const &src);

      // Performs one part of an Arg movement.
      void GenStmnt_MovePart(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &arg, bool get, bool set);
   }
}

#endif//GDCC__AST__Exp_H__

