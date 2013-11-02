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

#include "ValueGlyph.hpp"

#include "../IArchive.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_ValueGlyph constructor
      //
      Exp_ValueGlyph::Exp_ValueGlyph(IArchive &in) : Super{in},
         glyph{GetIR(in, glyph)}
      {
      }

      //
      // Exp_ValueGlyph::v_canGetValue
      //
      bool Exp_ValueGlyph::v_canGetValue() const
      {
         return glyph.getData().value;
      }

      //
      // Exp_ValueGlyph::v_getValue
      //
      Value Exp_ValueGlyph::v_getValue() const
      {
         if(auto value = glyph.getData().value)
            return value->getValue();
         else
         {
            std::cerr << "ERROR: " << pos << ": undefined glyph: '"
               << static_cast<String>(glyph) << "'\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Exp_ValueGlyph::v_putIR
      //
      OArchive &Exp_ValueGlyph::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << glyph;
      }

      //
      // ExpCreate_ValueGlyph
      //
      Exp::Ref ExpCreate_ValueGlyph(Glyph glyph, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueGlyph(glyph, pos));
      }

      //
      // ExpGetIR_ValueGlyph
      //
      Exp::Ref ExpGetIR_ValueGlyph(IArchive &in)
      {
         return static_cast<Exp::Ref>(new Exp_ValueGlyph(in));
      }
   }
}

// EOF

