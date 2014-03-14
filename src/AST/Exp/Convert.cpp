//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree conversion expression bases.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Convert.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Convert::v_getIRExp
      //
      IR::Exp::CRef Exp_Convert::v_getIRExp() const
      {
         // The conversion logic is handled in Exp::getIRExp.
         return exp->getIRExp();
      }
   }
}

// EOF

