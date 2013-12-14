//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree Arg values.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__ValueArg_H__
#define GDCC__AST__Exp__ValueArg_H__

#include "../../AST/Exp.hpp"

#include "../../AST/Arg.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_ValueArg
      //
      class Exp_ValueArg : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_ValueArg, GDCC::AST::Exp);

      public:
         Arg const arg;


         friend Exp::CRef ExpCreate_ValueArg(Arg const &arg, Core::Origin pos);

      protected:
         Exp_ValueArg(Arg const &arg, Core::Origin pos);

         virtual void v_genStmnt(IR::Block &block, Function *fn, Arg const &dst) const;

         virtual Arg v_getArg() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__AST__Exp__ValueArg_H__

