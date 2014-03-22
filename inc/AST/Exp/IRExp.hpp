//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree IR expression values.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__IRExp_H__
#define GDCC__IR__Exp__IRExp_H__

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_IRExp
      //
      class Exp_IRExp : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_IRExp, GDCC::AST::Exp);

      public:
         IRExpCRef const exp;
         TypeCRef  const type;


         friend Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type);
         friend Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type,
            Core::Origin pos);

      protected:
         Exp_IRExp(IR::Exp const *exp, Type const *type, Core::Origin pos);
         virtual ~Exp_IRExp();

         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const {return false;}

         virtual bool v_isIRExp() const {return true;}
      };
   }
}

#endif//GDCC__IR__Exp__IRExp_H__

