//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: Aggregate expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__Multi_H__
#define GDCC__IR__Exp__Multi_H__

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Array
      //
      class Exp_Array final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Array, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_Array;}

         Type                   const elemT;
         Core::Array<Exp::CRef> const elemV;


         friend Exp::CRef ExpCreate_Array(Type const &elemT,
            Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
         friend Exp::CRef ExpCreate_Array(Type const &elemT,
            Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);
         friend Exp::CRef ExpCreate_Array(Type      &&elemT,
            Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
         friend Exp::CRef ExpCreate_Array(Type      &&elemT,
            Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);

         friend Exp::CRef ExpGetIR_Array(IArchive &in);

      protected:
         Exp_Array(Exp_Array const &) = default;

         Exp_Array(Type const &t, Core::Array<Exp::CRef> const &v,
            Core::Origin pos_) : Super{pos_}, elemT{t}, elemV{v} {}
         Exp_Array(Type const &t, Core::Array<Exp::CRef> &&v,
            Core::Origin pos_) : Super{pos_}, elemT{t}, elemV{std::move(v)} {}
         Exp_Array(Type &&t, Core::Array<Exp::CRef> const &v,
            Core::Origin pos_) : Super{pos_}, elemT{std::move(t)}, elemV{v} {}
         Exp_Array(Type &&t, Core::Array<Exp::CRef> &&v, Core::Origin pos_) :
            Super{pos_}, elemT{std::move(t)}, elemV{std::move(v)} {}

         explicit Exp_Array(IArchive &in);

         virtual Type v_getType() const;

         virtual Value v_getValue() const;

         virtual bool v_isValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      //
      // Exp_Assoc
      //
      class Exp_Assoc final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Assoc, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_Assoc;}

         Type_Assoc             const elemT;
         Core::Array<Exp::CRef> const elemV;


         friend Exp::CRef ExpCreate_Assoc(Type_Assoc const &elemT,
            Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
         friend Exp::CRef ExpCreate_Assoc(Type_Assoc const &elemT,
            Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);
         friend Exp::CRef ExpCreate_Assoc(Type_Assoc      &&elemT,
            Core::Array<Exp::CRef> const &elemV, Core::Origin pos);
         friend Exp::CRef ExpCreate_Assoc(Type_Assoc      &&elemT,
            Core::Array<Exp::CRef>      &&elemV, Core::Origin pos);

         friend Exp::CRef ExpGetIR_Assoc(IArchive &in);

      protected:
         Exp_Assoc(Exp_Assoc const &) = default;

         Exp_Assoc(Type_Assoc const &t, Core::Array<Exp::CRef> const &v,
            Core::Origin pos_) : Super{pos_}, elemT{t}, elemV{v} {}
         Exp_Assoc(Type_Assoc const &t, Core::Array<Exp::CRef> &&v,
            Core::Origin pos_) : Super{pos_}, elemT{t}, elemV{std::move(v)} {}
         Exp_Assoc(Type_Assoc &&t, Core::Array<Exp::CRef> const &v,
            Core::Origin pos_) : Super{pos_}, elemT{std::move(t)}, elemV{v} {}
         Exp_Assoc(Type_Assoc &&t, Core::Array<Exp::CRef> &&v, Core::Origin pos_) :
            Super{pos_}, elemT{std::move(t)}, elemV{std::move(v)} {}

         explicit Exp_Assoc(IArchive &in);

         virtual Type v_getType() const;

         virtual Value v_getValue() const;

         virtual bool v_isValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      //
      // Exp_Tuple
      //
      class Exp_Tuple final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Tuple, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_Tuple;}

         Core::Array<Exp::CRef> const elemV;


         friend Exp::CRef ExpCreate_Tuple(Core::Array<Exp::CRef> const &elemV,
            Core::Origin pos);
         friend Exp::CRef ExpCreate_Tuple(Core::Array<Exp::CRef> &&elemV,
            Core::Origin pos);

         friend Exp::CRef ExpGetIR_Tuple(IArchive &in);

      protected:
         Exp_Tuple(Exp_Tuple const &) = default;
         Exp_Tuple(Core::Array<Exp::CRef> const &elemV_, Core::Origin pos_) :
            Super{pos_}, elemV{elemV_} {}
         Exp_Tuple(Core::Array<Exp::CRef> &&elemV_, Core::Origin pos_) :
            Super{pos_}, elemV{std::move(elemV_)} {}
         explicit Exp_Tuple(IArchive &in);

         virtual Type v_getType() const;

         virtual Value v_getValue() const;

         virtual bool v_isValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };

      //
      // Exp_Union
      //
      class Exp_Union final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Union, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_Union;}

         Type_Union const elemT;
         Exp::CRef  const elemV;


         friend Exp::CRef ExpCreate_Union(Type_Union const &elemT, Exp const *elemV,
            Core::Origin pos);
         friend Exp::CRef ExpCreate_Union(Type_Union &&elemT, Exp const *elemV,
            Core::Origin pos);

         friend Exp::CRef ExpGetIR_Union(IArchive &in);

      protected:
         Exp_Union(Exp_Union const &) = default;

         Exp_Union(Type_Union const &t, Exp const *v, Core::Origin pos_) :
            Super{pos_}, elemT{t}, elemV{v} {}
         Exp_Union(Type_Union &&t, Exp const *v, Core::Origin pos_) :
            Super{pos_}, elemT{std::move(t)}, elemV{v} {}

         explicit Exp_Union(IArchive &in);

         virtual Type v_getType() const;

         virtual Value v_getValue() const;

         virtual bool v_isValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__ValueMulti_H__

