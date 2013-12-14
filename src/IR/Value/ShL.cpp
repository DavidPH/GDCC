//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator <<.
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
      GDCC_IR_ValueBinOpImplEq(<<, Fixed);

      //
      // Type::PromoteShL
      //
      Type Type::PromoteShL(Type const &l, Type const &)
      {
         return l;
      }

      //
      // operator Value << Value
      //
      Value operator << (Value const &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed << r.vFixed;

         throw TypeError();
      }

      //
      // operator Value <<= Value
      //
      Value &operator <<= (Value &l, Value const &r)
      {
         if(l.v == ValueBase::Fixed && r.v == ValueBase::Fixed) return l.vFixed <<= r.vFixed, l;

         throw TypeError();
      }

      //
      // operator Value_Fixed <<= Value_Fixed
      //
      Value_Fixed &operator <<= (Value_Fixed &l, Value_Fixed const &r)
      {
         l.value <<= number_cast<Core::FastU>(r.value >> r.vtype.bitsF);

         return l.clamp();
      }
   }
}

// EOF

