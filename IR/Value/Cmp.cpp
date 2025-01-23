//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value comparison operators.
//
//-----------------------------------------------------------------------------

#include "Util.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

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

//
// GDCC_IR_ValueCmpOpFltsImpl
//
#define GDCC_IR_ValueCmpOpFltsImpl(op) \
   bool operator op (Value_Float const &l, Value_Float const &r) \
   { \
      return l.value op r.value; \
   }

//
// GDCC_IR_ValueCmpOpValsImpl
//
#define GDCC_IR_ValueCmpOpValsImpl(op) \
   bool operator op (Value const &l, Value const &r) \
   { \
      if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed op r.vFixed; \
      if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat op r.vFloat; \
      \
      throw TypeError(); \
   }

//
// GDCC_IR_ValueCmpOpValsImplEq
//
#define GDCC_IR_ValueCmpOpValsImplEq(op) \
   bool operator op (Value const &l, Value const &r) \
   { \
      if(l.v != r.v) return l.v op r.v; \
      \
      if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed op r.vFixed; \
      if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat op r.vFloat; \
      if(l.v == ValueBase::Point && r.v == ValueBase::Point) return l.vPoint op r.vPoint; \
      \
      throw TypeError(); \
   }


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   GDCC_IR_ValueCmpOpBitsImpl(<)
   GDCC_IR_ValueCmpOpFltsImpl(<)
   GDCC_IR_ValueCmpOpValsImpl(<)
   GDCC_IR_ValueCmpOpBitsImpl(>)
   GDCC_IR_ValueCmpOpFltsImpl(>)
   GDCC_IR_ValueCmpOpValsImpl(>)
   GDCC_IR_ValueCmpOpBitsImpl(<=)
   GDCC_IR_ValueCmpOpFltsImpl(<=)
   GDCC_IR_ValueCmpOpValsImpl(<=)
   GDCC_IR_ValueCmpOpBitsImpl(>=)
   GDCC_IR_ValueCmpOpFltsImpl(>=)
   GDCC_IR_ValueCmpOpValsImpl(>=)

   GDCC_IR_ValueCmpOpBitsImpl(==)
   GDCC_IR_ValueCmpOpFltsImpl(==)
   GDCC_IR_ValueCmpOpValsImplEq(==)
   GDCC_IR_ValueCmpOpBitsImpl(!=)
   GDCC_IR_ValueCmpOpFltsImpl(!=)
   GDCC_IR_ValueCmpOpValsImplEq(!=)

   //
   // Value_Point == Value_Point
   //
   bool operator == (Value_Point const &l, Value_Point const &r)
   {
      return
         l.vtype == r.vtype &&
         l.value == r.value &&
         l.addrB == r.addrB &&
         l.addrN == r.addrN;
   }

   //
   // Value_Point != Value_Point
   //
   bool operator != (Value_Point const &l, Value_Point const &r)
   {
      return !(l == r);
   }
}

// EOF

