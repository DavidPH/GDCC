//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Deref_H__
#define GDCC__CC__Exp__Deref_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Deref
      //
      class Exp_Deref : public AST::Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Deref, GDCC::AST::Exp_Unary);

         GDCC_AST_Exp_UnaryCreator(Exp_Deref);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual AST::Arg v_getArg() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;

         virtual bool v_isNoAuto() const;
      };
   }
}

#endif//GDCC__CC__Exp__Deref_H__

