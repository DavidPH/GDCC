//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: multiple expressions.
//
//-----------------------------------------------------------------------------

#include "IR/Exp/ValueMulti.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_ValueMulti constructor
      //
      Exp_ValueMulti::Exp_ValueMulti(IArchive &in) : Super{in},
         expv{GetIR<Core::Array<Exp::CRef>::size_type>(in), GetIRCaller<Exp::CRef>(in)}
      {
      }

      //
      // Exp_ValueMulti::v_canGetValue
      //
      bool Exp_ValueMulti::v_canGetValue() const
      {
         for(auto const &exp : expv)
            if(!exp->canGetValue()) return false;

         return true;
      }

      //
      // Exp_ValueMulti::v_getType
      //
      Type Exp_ValueMulti::v_getType() const
      {
         std::vector<Type> typev; typev.reserve(expv.size());

         for(auto const &exp : expv)
            typev.emplace_back(exp->getType());

         return Type_Multi(Core::Array<Type>(Core::Move, typev.begin(), typev.end()));
      }

      //
      // Exp_ValueMulti::v_getValue
      //
      Value Exp_ValueMulti::v_getValue() const
      {
         std::vector<Type> typev; typev.reserve(expv.size());
         std::vector<Value> valuev; valuev.reserve(expv.size());

         for(auto const &exp : expv)
         {
            valuev.emplace_back(exp->getValue());
            typev.emplace_back(valuev.back().getType());
         }

         return Value_Multi(
            Core::Array<Value>(Core::Move, valuev.begin(), valuev.end()),
            Type_Multi(Core::Array<Type>(Core::Move, typev.begin(), typev.end())));
      }

      //
      // Exp_ValueMulti::v_putIR
      //
      OArchive &Exp_ValueMulti::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << expv;
      }

      //
      // ExpCreate_ValueMulti
      //
      Exp::CRef ExpCreate_ValueMulti(Exp::CRef const *expv, std::size_t expc,
         Core::Origin pos)
      {
         return static_cast<Exp::CRef>(new Exp_ValueMulti(expv, expc, pos));
      }

      //
      // ExpGetIR_ValueMulti
      //
      Exp::CRef ExpGetIR_ValueMulti(IArchive &in)
      {
         return static_cast<Exp::CRef>(new Exp_ValueMulti(in));
      }
   }
}

// EOF

