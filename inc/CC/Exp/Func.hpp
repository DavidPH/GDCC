//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Func_H__
#define GDCC__CC__Exp__Func_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Func
      //
      class Exp_Func : public AST::Exp
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Func, GDCC::AST::Exp);

      public:
         Core::CounterRef<AST::Function> const fn;
         IR::Program                          &prog;


         friend AST::Exp::CRef ExpCreate_Func(IR::Program &prog,
            AST::Function *fn, Core::Origin pos);

      protected:
         Exp_Func(IR::Program &prog, AST::Function *fn, Core::Origin pos);
         virtual ~Exp_Func();

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual AST::Arg v_getArg() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Func_H__

