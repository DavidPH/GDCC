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
         FunctionRef const fn;
         IR::Program      &prog;


         // Create
         static CRef Create(IR::Program &prog, AST::Function *fn, Core::Origin pos)
            {return CRef(new This(prog, fn, pos));}

      protected:
         Exp_Func(IR::Program &prog, AST::Function *fn, Core::Origin pos);
         virtual ~Exp_Func();

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual AST::Arg v_getArg() const;

         virtual FunctionRef v_getFunction() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isFunction() const {return true;}

         virtual bool v_isIRExp() const;

         virtual bool v_isNoAuto() const {return true;}
      };
   }
}

#endif//GDCC__CC__Exp__Func_H__

