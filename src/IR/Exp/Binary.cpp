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

#include "IR/Exp/Binary.hpp"

#include "IR/IArchive.hpp"


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

      GDCC_IR_Exp_BinaryImplCreate(AddPtrRaw)

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

      //
      // Exp_BinaryAddPtrRaw::v_getType
      //
      Type Exp_BinaryAddPtrRaw::v_getType() const
      {
         return Type::PromoteAdd(expL->getType(), expR->getType());
      }

      //
      // Exp_BinaryAddPtrRaw::v_getValue
      //
      Value Exp_BinaryAddPtrRaw::v_getValue() const
      {
         auto valL = expL->getValue();
         auto valR = expR->getValue();

         if(valL.v == ValueBase::Point && valR.v == ValueBase::Fixed)
         {
            valL.vPoint.value += number_cast<Core::FastU>(valR.vFixed.value);
            return valL;
         }

         if(valL.v == ValueBase::Fixed && valR.v == ValueBase::Point)
         {
            valR.vPoint.value += number_cast<Core::FastU>(valL.vFixed.value);
            return valR;
         }

         throw TypeError();
      }
   }
}

// EOF

