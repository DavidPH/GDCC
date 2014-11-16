//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C conversion to void expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Convert__Void_H__
#define GDCC__CC__Exp__Convert__Void_H__

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
      // Exp_ConvertVoid
      //
      class Exp_ConvertVoid : public AST::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertVoid, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertVoid);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
      };
   }
}

#endif//GDCC__CC__Exp__Convert__Void_H__

