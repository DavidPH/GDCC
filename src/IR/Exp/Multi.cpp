//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: multiple expressions.
//
//-----------------------------------------------------------------------------

#include "IR/Exp/Multi.hpp"

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
      // Exp_Multi constructor
      //
      Exp_Multi::Exp_Multi(IArchive &in) : Super{in},
         expv{GetIR(in, expv)}
      {
      }

      //
      // Exp_Multi::v_canGetValue
      //
      bool Exp_Multi::v_canGetValue() const
      {
         for(auto const &exp : expv)
            if(!exp->canGetValue()) return false;

         return true;
      }

      //
      // Exp_Multi::v_getType
      //
      Type Exp_Multi::v_getType() const
      {
         std::vector<Type> typev; typev.reserve(expv.size());

         for(auto const &exp : expv)
            typev.emplace_back(exp->getType());

         return Type_Multi(Core::Array<Type>(Core::Move, typev.begin(), typev.end()));
      }

      //
      // Exp_Multi::v_getValue
      //
      Value Exp_Multi::v_getValue() const
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
      // Exp_Multi::v_putIR
      //
      OArchive &Exp_Multi::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << expv;
      }

      //
      // ExpCreate_Multi
      //
      Exp::CRef ExpCreate_Multi(Core::Array<Exp::CRef> const &expv,
         Core::Origin pos)
      {
         return static_cast<Exp::CRef>(new Exp_Multi(expv, pos));
      }

      //
      // ExpCreate_Multi
      //
      Exp::CRef ExpCreate_Multi(Core::Array<Exp::CRef> &&expv, Core::Origin pos)
      {
         return static_cast<Exp::CRef>(new Exp_Multi(std::move(expv), pos));
      }

      //
      // ExpGetIR_Multi
      //
      Exp::CRef ExpGetIR_Multi(IArchive &in)
      {
         return static_cast<Exp::CRef>(new Exp_Multi(in));
      }
   }
}

// EOF

