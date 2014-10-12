//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C __va_arg expression.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__VaArg__H__
#define GDCC__CC__Exp__VaArg__H__

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
      // Exp_VaArg
      //
      class Exp_VaArg : public AST::Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_VaArg, GDCC::AST::Exp_Unary);

         GDCC_AST_Exp_UnaryCreator(Exp_VaArg);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual AST::Arg v_getArg() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__VaArg__H__

