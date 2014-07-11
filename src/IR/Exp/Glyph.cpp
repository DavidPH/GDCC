//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: glyph.
//
//-----------------------------------------------------------------------------

#include "IR/Exp/Glyph.hpp"

#include "IR/IArchive.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Glyph constructor
      //
      Exp_Glyph::Exp_Glyph(IArchive &in) : Super{in},
         glyph{GetIR(in, glyph)}
      {
      }

      //
      // Exp_Glyph::v_canGetValue
      //
      bool Exp_Glyph::v_canGetValue() const
      {
         return glyph.getData().value;
      }

      //
      // Exp_Glyph::v_getValue
      //
      Value Exp_Glyph::v_getValue() const
      {
         if(auto value = glyph.getData().value)
            return value->getValue();
         else
         {
            std::cerr << "ERROR: " << pos << ": undefined glyph: '"
               << static_cast<Core::String>(glyph) << "'\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Exp_Glyph::v_putIR
      //
      OArchive &Exp_Glyph::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << glyph;
      }

      //
      // ExpCreate_Glyph
      //
      Exp::CRef ExpCreate_Glyph(Glyph glyph, Core::Origin pos)
      {
         return static_cast<Exp::CRef>(new Exp_Glyph(glyph, pos));
      }

      //
      // ExpGetIR_Glyph
      //
      Exp::CRef ExpGetIR_Glyph(IArchive &in)
      {
         return static_cast<Exp::CRef>(new Exp_Glyph(in));
      }
   }
}

// EOF

