//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: binary base.
//
//-----------------------------------------------------------------------------

#include "Binary.hpp"

#include "../IArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      GDCC_IR_Exp_BinaryImpl(Add, +)
      GDCC_IR_Exp_BinaryImpl(And, &)
      GDCC_IR_Exp_BinaryImpl(Div, /)
      GDCC_IR_Exp_BinaryImpl(Mod, %)
      GDCC_IR_Exp_BinaryImpl(Mul, *)
      GDCC_IR_Exp_BinaryImpl(OrI, |)
      GDCC_IR_Exp_BinaryImpl(OrX, ^)
      GDCC_IR_Exp_BinaryImpl(ShL, <<)
      GDCC_IR_Exp_BinaryImpl(ShR, >>)
      GDCC_IR_Exp_BinaryImpl(Sub, -)

      //
      // Exp_Binary constructor
      //
      Exp_Binary::Exp_Binary(IArchive &in) : Super{in}, expL{GetIR(in, expL)},
         expR{GetIR(in, expR)}
      {
      }

      //
      // Exp_Binary::v_putIR
      //
      OArchive &Exp_Binary::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << expL << expR;
      }
   }
}

// EOF

