//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: value primitive.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__Value_H__
#define GDCC__IR__Exp__Value_H__

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Value
      //
      class Exp_Value final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Value, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_Value;}

         Type const type;
         Value const value;


         friend Exp::CRef ExpCreate_Value(Value const &value,
            Core::Origin pos);
         friend Exp::CRef ExpCreate_Value(Value &&value, Core::Origin pos);

         friend Exp::CRef ExpGetIR_Value(IArchive &in);

      protected:
         Exp_Value(Exp_Value const &) = default;
         Exp_Value(Value const &value_, Core::Origin pos_) :
            Super{pos_}, type{value_.getType()}, value{value_} {}
         Exp_Value(Value &&value_, Core::Origin pos_) :
            Super{pos_}, type{value_.getType()}, value{std::move(value_)} {}
         explicit Exp_Value(IArchive &in);

         virtual Type v_getType() const {return type;}

         virtual Value v_getValue() const {return value;}

         virtual bool v_isValue() const {return true;}

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__Value_H__

