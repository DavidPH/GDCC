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
         using TypeCRef = CounterRef<Type const>;

      public:
         void genStmnt(IR::Block &block, Function *fn) const;

         TypeCRef getType() const;

         bool isEffect() const {return v_isEffect();}

         Origin const pos;

      protected:
         explicit Exp(Origin pos_) : pos{pos_} {}

         virtual void v_genStmnt(IR::Block &block, Function *fn, Arg const &dst) const = 0;

         virtual TypeCRef v_getType() const = 0;

         virtual bool v_isEffect() const = 0;
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
      Exp::Ref ExpCreate_BinaryMulSize(Exp *l, Exp *r);

      Exp::Ref ExpCreate_ValueSize(FastU value);
   }
}

#endif//GDCC__AST__Exp_H__

