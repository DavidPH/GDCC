//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator &" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Refer_H__
#define GDCC__CC__Exp__Refer_H__

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
      // Exp_Refer
      //
      class Exp_Refer : public AST::Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Refer, GDCC::AST::Exp_Unary);

      public:
         // Create
         static CRef Create(AST::Exp const *e, Core::Origin pos)
            {return CRef(new This(e, pos));}

      protected:
         Exp_Refer(AST::Exp const *e, Core::Origin pos);

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual FunctionRef v_getFunction() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual bool v_isEffect() const;

         virtual bool v_isFunction() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Refer_H__

