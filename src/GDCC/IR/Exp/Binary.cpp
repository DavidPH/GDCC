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
      GDCC_IR_Exp_BinaryImplCreate(Add)
      GDCC_IR_Exp_BinaryImplCreate(And)
      GDCC_IR_Exp_BinaryImplCreate(Div)
      GDCC_IR_Exp_BinaryImplCreate(Mod)
      GDCC_IR_Exp_BinaryImplCreate(Mul)
      GDCC_IR_Exp_BinaryImplCreate(OrI)
      GDCC_IR_Exp_BinaryImplCreate(OrX)
      GDCC_IR_Exp_BinaryImplCreate(ShL)
      GDCC_IR_Exp_BinaryImplCreate(ShR)
      GDCC_IR_Exp_BinaryImplCreate(Sub)

      GDCC_IR_Exp_BinaryImplValueFloat(Add, +)
      GDCC_IR_Exp_BinaryImplValueFixed(And, &)
      GDCC_IR_Exp_BinaryImplValueFloat(Div, /)
      GDCC_IR_Exp_BinaryImplValueFloat(Mod, %)
      GDCC_IR_Exp_BinaryImplValueFloat(Mul, *)
      GDCC_IR_Exp_BinaryImplValueFixed(OrI, |)
      GDCC_IR_Exp_BinaryImplValueFixed(OrX, ^)
      GDCC_IR_Exp_BinaryImplValueFixed(ShL, <<)
      GDCC_IR_Exp_BinaryImplValueFixed(ShR, >>)
      GDCC_IR_Exp_BinaryImplValueFloat(Sub, -)

      //
      // Exp_Binary constructor
      //
      Exp_Binary::Exp_Binary(IArchive &in) : Super{in}, expL{GetIR(in, expL)},
         expR{GetIR(in, expR)}
      {
      }

      //
      // Exp_Binary::v_getType
      //
      Type Exp_Binary::v_getType() const
      {
         auto l = expL->getType(), r = expR->getType();

         if(l.t == r.t) return l;

         return v_getTypeInequal(l, r);
      }

      //
      // Exp_Binary::v_getTypeInequal
      //
      Type Exp_Binary::v_getTypeInequal(Type const &, Type const &) const
      {
         return Type_Empty();
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

