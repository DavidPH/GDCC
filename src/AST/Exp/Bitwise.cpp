//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree bitwise and shift expression bases.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Bitwise.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_AST_Exp_BitwiseImpl
//
#define GDCC_AST_Exp_BitwiseImpl(class, ir) \
   IR::Exp::CRef class::v_getIRExp() const \
   { \
      return IR::ExpCreate_##ir( \
         expL->getIRExp(), expR->getIRExp(), pos); \
   }


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      GDCC_AST_Exp_BitwiseImpl(Exp_BitAnd, BinaryAnd)
      GDCC_AST_Exp_BitwiseImpl(Exp_BitOrI, BinaryOrI)
      GDCC_AST_Exp_BitwiseImpl(Exp_BitOrX, BinaryOrX)
      GDCC_AST_Exp_BitwiseImpl(Exp_ShL,    BinaryShL)
      GDCC_AST_Exp_BitwiseImpl(Exp_ShR,    BinaryShR)
   }
}

// EOF

