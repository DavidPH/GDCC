//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree Arg values.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Arg_H__
#define GDCC__AST__Exp__Arg_H__

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
      // Exp_Arg
      //
      class Exp_Arg : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_Arg, GDCC::AST::Exp);

      public:
         Arg const arg;


         friend Exp::CRef ExpCreate_Arg(Arg const &arg, Core::Origin pos);

      protected:
         Exp_Arg(Arg const &arg, Core::Origin pos);

         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual Arg v_getArg() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__AST__Exp__Arg_H__

