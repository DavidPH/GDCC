//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: value primitive.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__ValueRoot_H__
#define GDCC__IR__Exp__ValueRoot_H__

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_ValueRoot
      //
      class Exp_ValueRoot final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_ValueRoot, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_ValueRoot;}

         Type const type;
         Value const value;


         friend Exp::CRef ExpCreate_ValueRoot(Value const &value,
            Core::Origin pos);
         friend Exp::CRef ExpCreate_ValueRoot(Value &&value, Core::Origin pos);

         friend Exp::CRef ExpGetIR_ValueRoot(IArchive &in);

      protected:
         Exp_ValueRoot(Exp_ValueRoot const &) = default;
         Exp_ValueRoot(Value const &value_, Core::Origin pos_) :
            Super{pos_}, type{value_.getType()}, value{value_} {}
         Exp_ValueRoot(Value &&value_, Core::Origin pos_) :
            Super{pos_}, type{value_.getType()}, value{std::move(value_)} {}
         explicit Exp_ValueRoot(IArchive &in);

         virtual bool v_canGetValue() const {return true;}

         virtual Type v_getType() const {return type;}

         virtual Value v_getValue() const {return value;}

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__ValueRoot_H__

