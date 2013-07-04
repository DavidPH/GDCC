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

#include "Exp.hpp"


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
         GDCC_CounterPreamble(GDCC::IR::Exp_ValueRoot, GDCC::IR::Exp);

      public:
         Type const type;
         Value const value;


         friend Exp::Ref ExpCreate_ValueRoot(Value const &value, Origin pos);
         friend Exp::Ref ExpCreate_ValueRoot(Value &&value, Origin pos);

      protected:
         Exp_ValueRoot(Exp_ValueRoot const &) = default;
         Exp_ValueRoot(Value const &value_, Origin pos_) : Super{pos_},
            type{value_.getType()}, value{value_} {}
         Exp_ValueRoot(Value &&value_, Origin pos_) : Super{pos_},
            type{value_.getType()}, value{std::move(value_)} {}

         virtual Type v_getType() const {return type;}

         virtual Value v_getValue() const {return value;}
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // ExpCreate_ValueRoot
      //
      inline Exp::Ref ExpCreate_ValueRoot(Value const &value, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueRoot(value, pos));
      }

      //
      // ExpCreate_ValueRoot
      //
      inline Exp::Ref ExpCreate_ValueRoot(Value &&value, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueRoot(std::move(value), pos));
      }
   }
}

#endif//GDCC__IR__Exp__ValueRoot_H__

