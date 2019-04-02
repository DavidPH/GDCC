//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression statement.
//
//-----------------------------------------------------------------------------

#include "SR/Statement/Exp.hpp"

#include "SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Statement_Exp destructor
   //
   Statement_Exp::~Statement_Exp()
   {
   }

   //
   // Statement_Exp::v_genStmnt
   //
   void Statement_Exp::v_genStmnt(GenStmntCtx const &ctx) const
   {
      exp->genStmnt(ctx);
   }

   //
   // Statement_Exp::v_isEffect
   //
   bool Statement_Exp::v_isEffect() const
   {
      return exp->isEffect();
   }

   //
   // Statement_Exp::v_isNoAuto
   //
   bool Statement_Exp::v_isNoAuto() const
   {
      return exp->isNoAuto();
   }
}

// EOF

