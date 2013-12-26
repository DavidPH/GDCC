//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator -.
//
//-----------------------------------------------------------------------------

#include "Util.hpp"

#include <climits>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      GDCC_IR_ValueBinOpImplEq(-, Fixed)
      GDCC_IR_ValueBinOpImplEq(-, Float)

      Value_Point operator - (Value_Point const &l, Value_Fixed const &r)
         {auto lt = l; return lt -= r;}

      Value_Point &&operator - (Value_Point &&l, Value_Fixed const &r)
         {return std::move(l -= r);}

      //
      // Type::PromoteSub
      //
      Type Type::PromoteSub(Type const &l, Type const &r)
      {
         if(l.t == TypeBase::Fixed && r.t == TypeBase::Fixed)
            return Type_Fixed::Promote(l.tFixed, r.tFixed);

         if(l.t == TypeBase::Fixed && r.t == TypeBase::Point) return r.tPoint;
            return Type_Fixed(sizeof(Core::FastU) * CHAR_BIT, 0, true, false);

         if(l.t == TypeBase::Float && r.t == TypeBase::Float)
            return Type_Float::Promote(l.tFloat, r.tFloat);

         if(l.t == TypeBase::Point && r.t == TypeBase::Fixed) return l.tPoint;
            return Type_Fixed(sizeof(Core::FastU) * CHAR_BIT, 0, true, false);

         return Type_Empty();
      }

      //
      // operator Value - Value
      //
      Value operator - (Value const &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed - r.vFixed;
         if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat - r.vFloat;
         if(l.v == ValueBase::Point && r.v == ValueBase::Fixed) return l.vPoint - r.vFixed;
         if(l.v == ValueBase::Point && r.v == ValueBase::Point) return l.vPoint - r.vPoint;

         throw TypeError();
      }

      //
      // operator Value -= Value
      //
      Value &operator -= (Value &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed -= r.vFixed, l;
         if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat -= r.vFloat, l;
         if(l.v == ValueBase::Point && r.v == ValueBase::Fixed) return l.vPoint -= r.vFixed, l;

         if(l.v == ValueBase::Point && r.v == ValueBase::Point)
            {auto lv = std::move(l.vPoint) - r.vPoint; return l = std::move(lv);}

         throw TypeError();
      }

      //
      // operator Value_Point - Value_Point
      //
      Value_Fixed operator - (Value_Point const &l, Value_Point const &r)
      {
         auto value = l.value; value -= r.value; value /= l.vtype.reprS;
         Type_Fixed type{sizeof(Core::FastU) * CHAR_BIT, 0, true, false};
         return Value_Fixed(std::move(value), std::move(type));
      }

      //
      // operator Value_Fixed -= Value_Fixed
      //
      Value_Fixed &operator -= (Value_Fixed &l, Value_Fixed const &r)
      {
         GDCC_IR_ValueBinOpBitsOp(-);

         return l.clamp();
      }

      //
      // operator Value_Float -= Value_Float
      //
      Value_Float &operator -= (Value_Float &l, Value_Float const &r)
      {
         GDCC_IR_ValueBinOpFltsOp(-);

         return l.clamp();
      }

      //
      // operator Value_Point -= Value_Fixed
      //
      Value_Point &operator -= (Value_Point &l, Value_Fixed const &r)
      {
         l.value -= number_cast<Core::FastU>(r.value >> r.vtype.bitsF) /
            (l.vtype.reprS * l.vtype.reprW);

         return l;
      }
   }
}

// EOF

