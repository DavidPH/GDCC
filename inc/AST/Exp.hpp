//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp_H__
#define GDCC__AST__Exp_H__

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
      class Block;
      class Exp;
   }

   namespace AST
   {
      class Arg;
      class Function;
      class Type;

      //
      // Exp
      //
      class Exp : public Core::Counter
      {
         GDCC_Core_CounterPreambleAbstract(GDCC::AST::Exp, GDCC::Core::Counter);

      protected:
         using IRExpCPtr = Core::CounterPtr<IR::Exp const>;
         using IRExpCRef = Core::CounterRef<IR::Exp const>;
         using TypeCRef  = Core::CounterRef<Type const>;

      public:
         void genStmnt(IR::Block &block, Function *fn) const;

         void genStmnt(IR::Block &block, Function *fn, Arg const &dst) const;

         void genStmntStk(IR::Block &block, Function *fn) const;

         Arg getArg() const;
         Arg getArgDst() const;
         Arg getArgSrc() const;

         IRExpCRef getIRExp() const;

         TypeCRef getType() const;

         // Does this expression have side effects?
         bool isEffect() const;

         // Can this expression be made into an IR expression?
         bool isIRExp() const;

         // Marks the expression as needing its address taken.
         void setRefer() const;

         Core::Origin const pos;

      protected:
         Exp(Exp const &exp);
         explicit Exp(Core::Origin pos);
         virtual ~Exp();

         void genStmntMove(IR::Block &block, Function *fn, Arg const &dst,
            Arg const &src) const;

         void genStmntMovePart(IR::Block &block, Function *fn, Arg const &arg,
            bool get, bool set) const;

         virtual void v_genStmnt(IR::Block &block, Function *fn,
            Arg const &dst) const = 0;

         virtual Arg v_getArg() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const = 0;

         virtual bool v_isEffect() const = 0;

         virtual bool v_isIRExp() const = 0;

         virtual void v_setRefer() const;

      private:
         mutable IRExpCPtr cacheIRExp;
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
      Exp::CRef ExpCreate_Arg(Arg const &arg, Core::Origin pos);

      Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type);
      Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type,
         Core::Origin pos);

      Exp::CRef ExpCreate_MulSize(Exp const *l, Exp const *r);

      Exp::CRef ExpCreate_Size(Core::FastU value);
   }
}

#endif//GDCC__AST__Exp_H__

