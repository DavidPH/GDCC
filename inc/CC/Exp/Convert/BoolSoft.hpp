//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C soft boolean conversion expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Convert__SoftBool_H__
#define GDCC__CC__Exp__Convert__SoftBool_H__

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
      // Exp_ConvertBoolSoft_Fixed
      //
      // Converts fixed-point and integer types to a single-word soft boolean.
      //
      class Exp_ConvertBoolSoft_Fixed : public AST::Exp_Convert
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_ConvertBoolSoft_Fixed, GDCC::AST::Exp_Convert);

         GDCC_AST_Exp_UnaryCreator(Exp_ConvertBoolSoft_Fixed);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };
   }
}

#endif//GDCC__CC__Exp__Convert__SoftBool_H__

