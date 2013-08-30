//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "../Value.hpp"


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
#define GDCC_IR_ValueBinOpBits(op) \
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
// GDCC_IR_ValueCmpOpBitsImpl
//
#define GDCC_IR_ValueCmpOpBitsImpl(op) \
   bool operator op (Value_Fixed const &l, Value_Fixed const &r) \
   { \
      GDCC_IR_ValueCmpOpBitsOp(op); \
   }

//
// GDCC_IR_ValueCmpOpBitsOp
//
#define GDCC_IR_ValueCmpOpBitsOp(op) \
   if(l.vtype.bitsF < r.vtype.bitsF) \
      return (l.value << (r.vtype.bitsF - l.vtype.bitsF)) op r.value; \
   else if(l.vtype.bitsF > r.vtype.bitsF) \
      return l.value op (r.value << (l.vtype.bitsF - r.vtype.bitsF)); \
   else \
      return l.value op r.value

#endif//GDCC__IR__Value__Util_H__

