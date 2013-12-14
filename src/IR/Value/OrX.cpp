//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator ^.
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
      GDCC_IR_ValueBinOpImplEq(^, Fixed);

      //
      // Type::PromoteOrX
      //
      Type Type::PromoteOrX(Type const &l, Type const &r)
      {
         if(l.t == TypeBase::Fixed && r.t == TypeBase::Fixed)
            return Type_Fixed::Promote(l.tFixed, r.tFixed);

         return Type_Empty();
      }

      //
      // operator Value ^ Value
      //
      Value operator ^ (Value const &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed ^ r.vFixed;

         throw TypeError();
      }

      //
      // operator Value ^= Value
      //
      Value &operator ^= (Value &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed ^= r.vFixed, l;

         throw TypeError();
      }

      //
      // operator Value_Fixed ^= Value_Fixed
      //
      Value_Fixed &operator ^= (Value_Fixed &l, Value_Fixed const &r)
      {
         GDCC_IR_ValueBinOpBitsOp(^);

         return l;
      }
   }
}

// EOF

