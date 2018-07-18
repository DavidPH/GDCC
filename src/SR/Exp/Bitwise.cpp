//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation bitwise and shift expression bases.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Bitwise.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_SR_Exp_BitwiseImpl
//
#define GDCC_SR_Exp_BitwiseImpl(class, ir) \
   IR::Exp::CRef class::v_getIRExp() const \
   { \
      return IR::ExpCreate_##ir( \
         expL->getIRExp(), expR->getIRExp(), pos); \
   }


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   GDCC_SR_Exp_BitwiseImpl(Exp_BitAnd, BitAnd)
   GDCC_SR_Exp_BitwiseImpl(Exp_BitOrI, BitOrI)
   GDCC_SR_Exp_BitwiseImpl(Exp_BitOrX, BitOrX)
   GDCC_SR_Exp_BitwiseImpl(Exp_ShL,    ShL)
   GDCC_SR_Exp_BitwiseImpl(Exp_ShR,    ShR)
}

// EOF

