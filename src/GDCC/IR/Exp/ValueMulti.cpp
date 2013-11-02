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

#include "ValueMulti.hpp"

#include "../IArchive.hpp"
#include "../OArchive.hpp"


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
         expv{GetIR<Array<Exp::Ref>::size_type>(in), GetIRCaller<Exp::Ref>(in)}
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

         return Type_Multi(Array<Type>(Move, typev.begin(), typev.end()));
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

         return Value_Multi(Array<Value>(Move, valuev.begin(), valuev.end()),
            Type_Multi(Array<Type>(Move, typev.begin(), typev.end())));
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
      Exp::Ref ExpCreate_ValueMulti(Exp::Ref const *expv, std::size_t expc, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueMulti(expv, expc, pos));
      }

      //
      // ExpGetIR_ValueMulti
      //
      Exp::Ref ExpGetIR_ValueMulti(IArchive &in)
      {
         return static_cast<Exp::Ref>(new Exp_ValueMulti(in));
      }
   }
}

// EOF

