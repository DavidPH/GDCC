//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../Counter.hpp"
#include "../Number.hpp"
#include "../Origin.hpp"


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
      class Exp : public Counter
      {
         GDCC_CounterPreambleAbstract(GDCC::AST::Exp, GDCC::Counter);

      protected:
         using IRExpCPtr = CounterPtr<IR::Exp const>;
         using IRExpCRef = CounterRef<IR::Exp const>;
         using TypeCRef  = CounterRef<Type const>;

      public:
         void genStmnt(IR::Block &block, Function *fn) const;

         void genStmnt(IR::Block &block, Function *fn, Arg const &dst) const;

         Arg getArg() const;
         Arg getArgDst() const;
         Arg getArgSrc() const;

         IRExpCRef getIRExp() const;

         TypeCRef getType() const;

         // Does this expression have side effects?
         bool isEffect() const;

         // Can this expression be made into an IR expression?
         bool isIRExp() const;

         Origin const pos;

      protected:
         explicit Exp(Origin pos);
         virtual ~Exp();

         void genStmntMove(IR::Block &block, Function *fn, Arg const &dst,
            Arg const &src) const;

         void genStmntMovePart(IR::Block &block, Function *fn, Arg const &arg,
            bool get, bool set) const;

         virtual void v_genStmnt(IR::Block &block, Function *fn, Arg const &dst) const = 0;

         virtual Arg v_getArg() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const = 0;

         virtual bool v_isEffect() const = 0;

         virtual bool v_isIRExp() const = 0;

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
      Exp::CRef ExpCreate_BinaryMulSize(Exp const *l, Exp const *r);

      Exp::CRef ExpCreate_ValueArg(Arg const &arg, Origin pos);

      Exp::CRef ExpCreate_ValueIRExp(IR::Exp const *exp, Type const *type);
      Exp::CRef ExpCreate_ValueIRExp(IR::Exp const *exp, Type const *type, Origin pos);

      Exp::CRef ExpCreate_ValueSize(FastU value);
   }
}

#endif//GDCC__AST__Exp_H__

