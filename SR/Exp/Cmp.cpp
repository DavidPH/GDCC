//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation comparator expression bases.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Cmp.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_SR_Exp_CmpImpl
//
#define GDCC_SR_Exp_CmpImpl(class, ir) \
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

namespace GDCC::SR
{
   GDCC_SR_Exp_CmpImpl(Exp_CmpEQ, CmpEQ)
   GDCC_SR_Exp_CmpImpl(Exp_CmpGE, CmpGE)
   GDCC_SR_Exp_CmpImpl(Exp_CmpGT, CmpGT)
   GDCC_SR_Exp_CmpImpl(Exp_CmpLE, CmpLE)
   GDCC_SR_Exp_CmpImpl(Exp_CmpLT, CmpLT)
   GDCC_SR_Exp_CmpImpl(Exp_CmpNE, CmpNE)
}

// EOF

