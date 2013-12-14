//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator %.
//
//-----------------------------------------------------------------------------

#include "Util.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      GDCC_IR_ValueBinOpImplEq(%, Fixed);
      GDCC_IR_ValueBinOpImplEq(%, Float);

      //
      // Type::PromoteMod
      //
      Type Type::PromoteMod(Type const &l, Type const &r)
      {
         if(l.t == TypeBase::Fixed && r.t == TypeBase::Fixed)
            return Type_Fixed::Promote(l.tFixed, r.tFixed);

         if(l.t == TypeBase::Float && r.t == TypeBase::Float)
            return Type_Float::Promote(l.tFloat, r.tFloat);

         return Type_Empty();
      }

      //
      // operator Value % Value
      //
      Value operator % (Value const &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed % r.vFixed;
         if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat % r.vFloat;

         throw TypeError();
      }

      //
      // operator Value %= Value
      //
      Value &operator %= (Value &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed %= r.vFixed, l;
         if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat %= r.vFloat, l;

         throw TypeError();
      }

      //
      // operator Value_Fixed %= Value_Fixed
      //
      Value_Fixed &operator %= (Value_Fixed &l, Value_Fixed const &r)
      {
         GDCC_IR_ValueBinOpBitsOp(%);

         return l.clamp();
      }

      //
      // operator Value_Float %= Value_Float
      //
      Value_Float &operator %= (Value_Float &l, Value_Float const &r)
      {
         if(l.vtype.bitsI < r.vtype.bitsI) l.vtype.bitsI = r.vtype.bitsI;
         if(l.vtype.bitsF < r.vtype.bitsF) l.vtype.bitsF = r.vtype.bitsF;
         l.value = std::fmod(l.value.get_d(), r.value.get_d()); // FIXME

         return l.clamp();
      }
   }
}

// EOF

