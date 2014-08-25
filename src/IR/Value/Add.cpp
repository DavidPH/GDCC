//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator +.
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
      GDCC_IR_ValueBinOpImplEq(+, Fixed)
      GDCC_IR_ValueBinOpImplEq(+, Float)

      Value_Point operator + (Value_Fixed const &l, Value_Point const &r)
         {auto rt = r; return rt += l;}

      Value_Point &&operator + (Value_Fixed const &l, Value_Point &&r)
         {return std::move(r += l);}

      Value_Point operator + (Value_Point const &l, Value_Fixed const &r)
         {auto lt = l; return lt += r;}

      Value_Point &&operator + (Value_Point &&l, Value_Fixed const &r)
         {return std::move(l += r);}

      //
      // Type::PromoteAdd
      //
      Type Type::PromoteAdd(Type const &l, Type const &r)
      {
         if(l.t == TypeBase::Fixed && r.t == TypeBase::Point) return r.tPoint;
         if(l.t == TypeBase::Point && r.t == TypeBase::Fixed) return l.tPoint;

         if(l.t == TypeBase::Fixed && r.t == TypeBase::Fixed)
            return Type_Fixed::Promote(l.tFixed, r.tFixed);

         if(l.t == TypeBase::Float && r.t == TypeBase::Float)
            return Type_Float::Promote(l.tFloat, r.tFloat);

         return Type_Empty();
      }

      //
      // operator Value + Value
      //
      Value operator + (Value const &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed + r.vFixed;
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Point) return l.vFixed + r.vPoint;
         if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat + r.vFloat;
         if(l.v == ValueBase::Point && r.v == ValueBase::Fixed) return l.vPoint + r.vFixed;

         throw TypeError();
      }

      //
      // operator Value += Value
      //
      Value &operator += (Value &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed += r.vFixed, l;
         if(l.v == ValueBase::Float && r.v == ValueBase::Float) return l.vFloat += r.vFloat, l;
         if(l.v == ValueBase::Point && r.v == ValueBase::Fixed) return l.vPoint += r.vFixed, l;

         if(l.v == ValueBase::Fixed && r.v == ValueBase::Point)
            {auto lv = std::move(l.vFixed) + r.vPoint; return l = std::move(lv);}

         throw TypeError();
      }

      //
      // operator Value_Fixed += Value_Fixed
      //
      Value_Fixed &operator += (Value_Fixed &l, Value_Fixed const &r)
      {
         GDCC_IR_ValueBinOpBitsOp(+);

         return l.clamp();
      }

      //
      // operator Value_Float += Value_Float
      //
      Value_Float &operator += (Value_Float &l, Value_Float const &r)
      {
         GDCC_IR_ValueBinOpFltsOp(+);

         return l.clamp();
      }

      //
      // operator Value_Point += Value_Fixed
      //
      Value_Point &operator += (Value_Point &l, Value_Fixed const &r)
      {
         l.value += Core::NumberCast<Core::FastU>(r.value >> r.vtype.bitsF) /
            (l.vtype.reprS * l.vtype.reprW);

         return l;
      }
   }
}

// EOF

