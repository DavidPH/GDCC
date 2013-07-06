//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp_H__
#define GDCC__IR__Exp_H__

#include "Value.hpp"

#include "../Counter.hpp"
#include "../Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Glyph;

      //
      // Exp
      //
      class Exp : public Counter
      {
         GDCC_CounterPreambleAbstract(GDCC::IR::Exp, GDCC::Counter);

      public:
         Type getType() const {return v_getType();}

         Value getValue() const {return v_getValue();}

         Origin const pos;

      protected:
         Exp(Exp const &) = default;
         explicit Exp(Origin pos_) : pos{pos_} {}

         virtual Type v_getType() const = 0;

         virtual Value v_getValue() const = 0;
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
      Exp::Ref ExpCreate_ValueGlyph(Glyph glyph, Origin pos);

      Exp::Ref ExpCreate_ValueRoot(Value const &value, Origin pos);
      Exp::Ref ExpCreate_ValueRoot(Value &&value, Origin pos);
   }
}

#endif//GDCC__IR__Exp_H__

