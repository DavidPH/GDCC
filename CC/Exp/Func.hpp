//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_Func
   //
   class Exp_Func : public SR::Exp
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_Func, GDCC::SR::Exp);

   public:
      FunctionRef const fn;
      IR::Program      &prog;


      // Create
      static CRef Create(IR::Program &prog, SR::Function *fn, Core::Origin pos)
         {return CRef(new This(prog, fn, pos));}

   protected:
      Exp_Func(IR::Program &prog, SR::Function *fn, Core::Origin pos);
      virtual ~Exp_Func();

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual SR::Arg v_getArg() const;

      virtual FunctionRef v_getFunction() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isFunction() const {return true;}

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const {return true;}
   };
}

#endif//GDCC__CC__Exp__Func_H__

