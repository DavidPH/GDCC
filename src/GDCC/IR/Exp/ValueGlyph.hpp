//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: glyph.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__ValueGlyph_H__
#define GDCC__IR__Exp__ValueGlyph_H__

#include "../Exp.hpp"

#include "../Glyph.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_ValueGlyph
      //
      class Exp_ValueGlyph final : public Exp
      {
         GDCC_CounterPreamble(GDCC::IR::Exp_ValueGlyph, GDCC::IR::Exp);

      public:
         Glyph const glyph;


         friend Exp::Ref ExpCreate_ValueGlyph(Glyph glyph, Origin pos);

      protected:
         Exp_ValueGlyph(Exp_ValueGlyph const &) = default;
         Exp_ValueGlyph(Glyph glyph_, Origin pos_) : Super{pos_}, glyph{glyph_} {}

         virtual Type v_getType() const {return glyph.getData().type;}

         virtual Value v_getValue() const;
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
      // ExpCreate_ValueGlyph
      //
      inline Exp::Ref ExpCreate_ValueGlyph(Glyph glyph, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueGlyph(glyph, pos));
      }
   }
}

#endif//GDCC__IR__Exp__ValueGlyph_H__

