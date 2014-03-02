//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree unary expression base.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Unary constructor
      //
      Exp_Unary::Exp_Unary(Exp const *e, Core::Origin pos_) :
         Super{pos_}, exp{e}
      {
      }

      //
      // Exp_Unary::v_isEffect
      //
      bool Exp_Unary::v_isEffect() const
      {
         return exp->isEffect();
      }

      //
      // Exp_Unary::v_isIRExp
      //
      bool Exp_Unary::v_isIRExp() const
      {
         return exp->isIRExp();
      }
   }
}

// EOF

