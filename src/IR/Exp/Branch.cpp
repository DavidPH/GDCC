//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: branch base and utilities.
//
//-----------------------------------------------------------------------------

#include "IR/Exp/Branch.hpp"

#include "IR/Exp/Binary.hpp"

#include "IR/IArchive.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IR
   {
      Type_Fixed const Exp_Branch::BoolRaw{1, 0, false, false};
      Type       const Exp_Branch::BoolType{BoolRaw};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      GDCC_IR_Exp_BraBinImplCreate(And)
      GDCC_IR_Exp_BraBinImplCreate(CmpEQ)
      GDCC_IR_Exp_BraBinImplCreate(CmpGE)
      GDCC_IR_Exp_BraBinImplCreate(CmpGT)
      GDCC_IR_Exp_BraBinImplCreate(CmpLE)
      GDCC_IR_Exp_BraBinImplCreate(CmpLT)
      GDCC_IR_Exp_BraBinImplCreate(CmpNE)
      GDCC_IR_Exp_BraTerImplCreate(Cnd)
      GDCC_IR_Exp_BraUnaImplCreate(Not)
      GDCC_IR_Exp_BraBinImplCreate(OrI)
      GDCC_IR_Exp_BraBinImplCreate(OrX)

      GDCC_IR_Exp_BranchImplCmpValue(CmpEQ, ==)
      GDCC_IR_Exp_BranchImplCmpValue(CmpGE, >=)
      GDCC_IR_Exp_BranchImplCmpValue(CmpGT, >)
      GDCC_IR_Exp_BranchImplCmpValue(CmpLE, <=)
      GDCC_IR_Exp_BranchImplCmpValue(CmpLT, <)
      GDCC_IR_Exp_BranchImplCmpValue(CmpNE, !=)

      //
      // Exp_BraBin constructor
      //
      Exp_BraBin::Exp_BraBin(IArchive &in) : Super{in},
         expL{GetIR(in, expL)}, expR{GetIR(in, expR)}
      {
      }

      //
      // Exp_BraTer constructor
      //
      Exp_BraTer::Exp_BraTer(IArchive &in) : Super{in},
         expC{GetIR(in, expC)}
      {
      }

      //
      // Exp_BraUna constructor
      //
      Exp_BraUna::Exp_BraUna(IArchive &in) : Super{in},
         exp{GetIR(in, exp)}
      {
      }

      //
      // Exp_BraBin::v_putIR
      //
      OArchive &Exp_BraBin::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << expL << expR;
      }

      //
      // Exp_BraTer::v_putIR
      //
      OArchive &Exp_BraTer::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << expC;
      }

      //
      // Exp_BraUna::v_putIR
      //
      OArchive &Exp_BraUna::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << exp;
      }

      //
      // Exp_BranchAnd::v_getValue
      //
      Value Exp_BranchAnd::v_getValue() const
      {
         return Value_Fixed(expL->getValue() && expR->getValue(), BoolRaw);
      }

      //
      // Exp_BranchCnd::v_getType
      //
      Type Exp_BranchCnd::v_getType() const
      {
         return expC->getValue() ? expL->getType() : expR->getType();
      }

      //
      // Exp_BranchCnd::v_getValue
      //
      Value Exp_BranchCnd::v_getValue() const
      {
         return expC->getValue() ? expL->getValue() : expR->getValue();
      }

      //
      // Exp_BranchNot::v_getValue
      //
      Value Exp_BranchNot::v_getValue() const
      {
         return Value_Fixed(!exp->getValue(), BoolRaw);
      }

      //
      // Exp_BranchOrI::v_getValue
      //
      Value Exp_BranchOrI::v_getValue() const
      {
         return Value_Fixed(expL->getValue() || expR->getValue(), BoolRaw);
      }

      //
      // Exp_BranchOrX::v_getValue
      //
      Value Exp_BranchOrX::v_getValue() const
      {
         return Value_Fixed(!expL->getValue() ^ !expR->getValue(), BoolRaw);
      }
   }
}

// EOF

