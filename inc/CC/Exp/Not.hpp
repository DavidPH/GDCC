//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator !" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Not_H__
#define GDCC__CC__Exp__Not_H__

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
      // Exp_Not
      //
      class Exp_Not : public AST::Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Not, GDCC::AST::Exp_Unary);

      public:
         static CRef Create(AST::Type const *t, AST::Exp const *e, Core::Origin pos)
            {return CRef(new This(t, e, pos));}

      protected:
         Exp_Not(AST::Type const *t, AST::Exp const *e, Core::Origin pos_) :
            Super{t, e, pos_} {}

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Not_H__

