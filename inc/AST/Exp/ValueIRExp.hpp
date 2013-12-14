//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree IR expression values.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__ValueIRExp_H__
#define GDCC__IR__Exp__ValueIRExp_H__

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_ValueIRExp
      //
      class Exp_ValueIRExp : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_ValueIRExp, GDCC::AST::Exp);

      public:
         IRExpCRef const exp;
         TypeCRef  const type;


         friend Exp::CRef ExpCreate_ValueIRExp(IR::Exp const *exp,
            Type const *type);
         friend Exp::CRef ExpCreate_ValueIRExp(IR::Exp const *exp,
            Type const *type, Core::Origin pos);

      protected:
         Exp_ValueIRExp(IR::Exp const *exp, Type const *type, Core::Origin pos);
         virtual ~Exp_ValueIRExp();

         virtual void v_genStmnt(IR::Block &block, Function *fn, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const {return false;}

         virtual bool v_isIRExp() const {return true;}
      };
   }
}

#endif//GDCC__IR__Exp__ValueIRExp_H__

