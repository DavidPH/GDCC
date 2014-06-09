//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C arithmetic conversion expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Convert__Arith_H__
#define GDCC__CC__Exp__Convert__Arith_H__

#include "../../../CC/Exp.hpp"

#include "../../../AST/Exp/Convert.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_ConvertFixed
      //
      // Converts between fixed-point types.
      //
      class Exp_ConvertFixed : public AST::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertFixed, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertFixed);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
      };
   }
}

#endif//GDCC__CC__Exp__Convert__Arith_H__

