//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: value primitive.
//
//-----------------------------------------------------------------------------

#include "IR/Exp/Value.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // Exp_Value constructor
   //
   Exp_Value::Exp_Value(IArchive &in) : Super{in}, type{in}, value{in}
   {
   }

   //
   // Exp_Value::v_putIR
   //
   OArchive &Exp_Value::v_putIR(OArchive &out) const
   {
      return Super::v_putIR(out) << type << value;
   }

   //
   // ExpCreate_Value
   //
   Exp::CRef ExpCreate_Value(Value const &value, Core::Origin pos)
   {
      return static_cast<Exp::CRef>(new Exp_Value(value, pos));
   }

   //
   // ExpCreate_Value
   //
   Exp::CRef ExpCreate_Value(Value &&value, Core::Origin pos)
   {
      return static_cast<Exp::CRef>(new Exp_Value(std::move(value), pos));
   }

   //
   // ExpGetIR_Value
   //
   Exp::CRef ExpGetIR_Value(IArchive &in)
   {
      return static_cast<Exp::CRef>(new Exp_Value(in));
   }
}

// EOF

