//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator =" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Assign_H__
#define GDCC__CC__Exp__Assign_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Assign
      //
      class Exp_Assign : public AST::Exp_Binary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Assign, GDCC::AST::Exp_Binary);

      public:
         friend AST::Exp::CRef ExpCreate_Assign(AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos);

      protected:
         Exp_Assign(AST::Exp const *l, AST::Exp const *r, Core::Origin pos);

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Assign_H__

