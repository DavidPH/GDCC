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

         GDCC_AST_Exp_UnaryCreator(Exp_Not);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Not_H__

