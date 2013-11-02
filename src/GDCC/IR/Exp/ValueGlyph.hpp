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
         virtual String getName() const {return STR_ValueGlyph;}

         Glyph const glyph;


         friend Exp::Ref ExpCreate_ValueGlyph(Glyph glyph, Origin pos);

         friend Exp::Ref ExpGetIR_ValueGlyph(IArchive &in);

      protected:
         Exp_ValueGlyph(Exp_ValueGlyph const &) = default;
         Exp_ValueGlyph(Glyph glyph_, Origin pos_) : Super{pos_}, glyph{glyph_} {}
         explicit Exp_ValueGlyph(IArchive &in);

         virtual bool v_canGetValue() const;

         virtual Type v_getType() const {return glyph.getData().type;}

         virtual Value v_getValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__ValueGlyph_H__

