//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree comparator expression bases.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Cmp.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_AST_Exp_CmpImpl
//
#define GDCC_AST_Exp_CmpImpl(class, ir) \
   IR::Exp::CRef class::v_getIRExp() const \
   { \
      return IR::ExpCreate_##ir( \
         expL->getIRExp(), expR->getIRExp(), pos); \
   } \
   \
   bool class::v_isBoolean() const \
   { \
      return true; \
   }


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      GDCC_AST_Exp_CmpImpl(Exp_CmpEQ, CmpEQ)
      GDCC_AST_Exp_CmpImpl(Exp_CmpGE, CmpGE)
      GDCC_AST_Exp_CmpImpl(Exp_CmpGT, CmpGT)
      GDCC_AST_Exp_CmpImpl(Exp_CmpLE, CmpLE)
      GDCC_AST_Exp_CmpImpl(Exp_CmpLT, CmpLT)
      GDCC_AST_Exp_CmpImpl(Exp_CmpNE, CmpNE)
   }
}

// EOF

