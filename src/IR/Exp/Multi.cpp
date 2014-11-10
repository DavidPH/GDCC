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
      // Exp_Array constructor
      //
      Exp_Array::Exp_Array(IArchive &in) : Super{in},
         elemT{in}, elemV{GetIR(in, elemV)}
      {
      }

      //
      // Exp_Array::v_getType
      //
      Type Exp_Array::v_getType() const
      {
         return Type_Array(elemT, elemV.size());
      }

      //
      // Exp_Array::v_getValue
      //
      Value Exp_Array::v_getValue() const
      {
         std::vector<Value> values; values.reserve(elemV.size());

         for(auto const &elem : elemV)
            values.emplace_back(elem->getValue());

         return Value_Array({Core::Move, values.begin(), values.end()},
            {elemT, elemV.size()});
      }

      //
      // Exp_Array::v_isValue
      //
      bool Exp_Array::v_isValue() const
      {
         for(auto const &elem : elemV)
            if(!elem->isValue()) return false;

         return true;
      }

      //
      // Exp_Array::v_putIR
      //
      OArchive &Exp_Array::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << elemT << elemV;
      }

      //
      // Exp_Assoc constructor
      //
      Exp_Assoc::Exp_Assoc(IArchive &in) : Super{in},
         elemT{in}, elemV{GetIR(in, elemV)}
      {
      }

      //
      // Exp_Assoc::v_getType
      //
      Type Exp_Assoc::v_getType() const
      {
         return elemT;
      }

      //
      // Exp_Assoc::v_getValue
      //
      Value Exp_Assoc::v_getValue() const
      {
         std::vector<Value> values; values.reserve(elemV.size());

         for(auto const &elem : elemV)
            values.emplace_back(elem->getValue());

         return Value_Assoc({Core::Move, values.begin(), values.end()}, elemT);
      }

      //
      // Exp_Assoc::v_isValue
      //
      bool Exp_Assoc::v_isValue() const
      {
         for(auto const &elem : elemV)
            if(!elem->isValue()) return false;

         return true;
      }

      //
      // Exp_Assoc::v_putIR
      //
      OArchive &Exp_Assoc::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << elemT << elemV;
      }

      //
      // Exp_Tuple constructor
      //
      Exp_Tuple::Exp_Tuple(IArchive &in) : Super{in},
         elemV{GetIR(in, elemV)}
      {
      }

      //
      // Exp_Tuple::v_getType
      //
      Type Exp_Tuple::v_getType() const
      {
         std::vector<Type> typev; typev.reserve(elemV.size());

         for(auto const &exp : elemV)
            typev.emplace_back(exp->getType());

         return Type_Tuple({Core::Move, typev.begin(), typev.end()});
      }

      //
      // Exp_Tuple::v_getValue
      //
      Value Exp_Tuple::v_getValue() const
      {
         std::vector<Type> typev; typev.reserve(elemV.size());
         std::vector<Value> valuev; valuev.reserve(elemV.size());

         for(auto const &exp : elemV)
         {
            valuev.emplace_back(exp->getValue());
            typev.emplace_back(valuev.back().getType());
         }

         return Value_Tuple({Core::Move, valuev.begin(), valuev.end()},
            {{Core::Move, typev.begin(), typev.end()}});
      }

      //
      // Exp_Tuple::v_isValue
      //
      bool Exp_Tuple::v_isValue() const
      {
         for(auto const &exp : elemV)
            if(!exp->isValue()) return false;

         return true;
      }

      //
      // Exp_Tuple::v_putIR
      //
      OArchive &Exp_Tuple::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << elemV;
      }

      //
      // Exp_Union constructor
      //
      Exp_Union::Exp_Union(IArchive &in) : Super{in},
         elemT{in}, elemV{GetIR(in, elemV)}
      {
      }

      //
      // Exp_Union::v_getType
      //
      Type Exp_Union::v_getType() const
      {
         return elemT;
      }

      //
      // Exp_Union::v_getValue
      //
      Value Exp_Union::v_getValue() const
      {
         return Value_Union(elemV->getValue(), elemT);
      }

      //
      // Exp_Union::v_isValue
      //
      bool Exp_Union::v_isValue() const
      {
         return elemV->isValue();
      }

      //
      // Exp_Union::v_putIR
      //
      OArchive &Exp_Union::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << elemT << elemV;
      }

      //
      // ExpCreate_Array
      //
      Exp::CRef ExpCreate_Array(Type const &elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Array(elemT, elemV, pos));
      }

      //
      // ExpCreate_Array
      //
      Exp::CRef ExpCreate_Array(Type const &elemT,
         Core::Array<Exp::CRef> &&elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Array(elemT, std::move(elemV), pos));
      }

      //
      // ExpCreate_Array
      //
      Exp::CRef ExpCreate_Array(Type &&elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Array(std::move(elemT), elemV, pos));
      }

      //
      // ExpCreate_Array
      //
      Exp::CRef ExpCreate_Array(Type &&elemT,
         Core::Array<Exp::CRef> &&elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Array(std::move(elemT), std::move(elemV), pos));
      }

      //
      // ExpCreate_Assoc
      //
      Exp::CRef ExpCreate_Assoc(Type_Assoc const &elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Assoc(elemT, elemV, pos));
      }

      //
      // ExpCreate_Assoc
      //
      Exp::CRef ExpCreate_Assoc(Type_Assoc const &elemT,
         Core::Array<Exp::CRef> &&elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Assoc(elemT, std::move(elemV), pos));
      }

      //
      // ExpCreate_Assoc
      //
      Exp::CRef ExpCreate_Assoc(Type_Assoc &&elemT,
         Core::Array<Exp::CRef> const &elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Assoc(std::move(elemT), elemV, pos));
      }

      //
      // ExpCreate_Assoc
      //
      Exp::CRef ExpCreate_Assoc(Type_Assoc &&elemT,
         Core::Array<Exp::CRef> &&elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Assoc(std::move(elemT), std::move(elemV), pos));
      }

      //
      // ExpCreate_Tuple
      //
      Exp::CRef ExpCreate_Tuple(Core::Array<Exp::CRef> const &elemV,
         Core::Origin pos)
      {
         return Exp::CRef(new Exp_Tuple(elemV, pos));
      }

      //
      // ExpCreate_Tuple
      //
      Exp::CRef ExpCreate_Tuple(Core::Array<Exp::CRef> &&elemV, Core::Origin pos)
      {
         return Exp::CRef(new Exp_Tuple(std::move(elemV), pos));
      }

      //
      // ExpCreate_Union
      //
      Exp::CRef ExpCreate_Union(Type_Union const &elemT, Exp const *elemV,
         Core::Origin pos)
      {
         return Exp::CRef(new Exp_Union(elemT, elemV, pos));
      }

      //
      // ExpCreate_Union
      //
      Exp::CRef ExpCreate_Union(Type_Union &&elemT, Exp const *elemV,
         Core::Origin pos)
      {
         return Exp::CRef(new Exp_Union(std::move(elemT), elemV, pos));
      }

      //
      // ExpGetIR_Array
      //
      Exp::CRef ExpGetIR_Array(IArchive &in)
      {
         return Exp::CRef(new Exp_Array(in));
      }

      //
      // ExpGetIR_Assoc
      //
      Exp::CRef ExpGetIR_Assoc(IArchive &in)
      {
         return Exp::CRef(new Exp_Assoc(in));
      }

      //
      // ExpGetIR_Tuple
      //
      Exp::CRef ExpGetIR_Tuple(IArchive &in)
      {
         return Exp::CRef(new Exp_Tuple(in));
      }

      //
      // ExpGetIR_Union
      //
      Exp::CRef ExpGetIR_Union(IArchive &in)
      {
         return Exp::CRef(new Exp_Union(in));
      }
   }
}

// EOF

