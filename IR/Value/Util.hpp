//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Value__Util_H__
#define GDCC__IR__Value__Util_H__

#include "IR/Value.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_IR_ValueBinOpImplEq
//
// Implements a binary operator using op=.
//
#define GDCC_IR_ValueBinOpImplEq(op, v) \
   Value_##v operator op (Value_##v const &l, Value_##v const &r) \
      {auto tl = l; return tl op##= r;} \
   \
   Value_##v &&operator op (Value_##v &&l, Value_##v const &r) \
      {return std::move(l op##= r);}

//
// GDCC_IR_ValueBinOpBits
//
#define GDCC_IR_ValueBinOpBits() \
   if(l.vtype.bitsF < r.vtype.bitsF) \
   { \
      l.value << (r.vtype.bitsF - l.vtype.bitsF); \
      l.vtype.bitsF = r.vtype.bitsF; \
   }

//
// GDCC_IR_ValueBinOpBitsOp
//
#define GDCC_IR_ValueBinOpBitsOp(op) \
   if(l.vtype.bitsF < r.vtype.bitsF) \
   { \
      l.value << (r.vtype.bitsF - l.vtype.bitsF); \
      l.vtype.bitsF = r.vtype.bitsF; \
      \
      l.value op##= r.value; \
   } \
   else if(l.vtype.bitsF > r.vtype.bitsF) \
      l.value op##= r.value << (l.vtype.bitsF - r.vtype.bitsF); \
   else \
      l.value op##= r.value

//
// GDCC_IR_ValueBinOpFltsOp
//
#define GDCC_IR_ValueBinOpFltsOp(op) \
   if(l.vtype.bitsI < r.vtype.bitsI) l.vtype.bitsI = r.vtype.bitsI; \
   if(l.vtype.bitsF < r.vtype.bitsF) l.vtype.bitsF = r.vtype.bitsF; \
   l.value op##= r.value

#endif//GDCC__IR__Value__Util_H__

