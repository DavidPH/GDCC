//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
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

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // Exp_Glyph constructor
   //
   Exp_Glyph::Exp_Glyph(IArchive &in) : Super{in},
      glyph{GetIR(in, glyph)}
   {
   }

   //
   // Exp_Glyph::v_getValue
   //
   Value Exp_Glyph::v_getValue() const
   {
      if(auto value = glyph.getData().value)
         return value->getValue();

      Core::ErrorUndef(pos, "glyph", static_cast<Core::String>(glyph));
   }

   //
   // Exp_Glyph::v_isValue
   //
   bool Exp_Glyph::v_isValue() const
   {
      auto &glyphData = glyph.getData();
      return glyphData.value && glyphData.value->isValue();
   }

   //
   // Exp_Glyph::v_putIR
   //
   OArchive &Exp_Glyph::v_putIR(OArchive &out) const
   {
      return Super::v_putIR(out) << glyph;
   }

   //
   // Exp_Glyph::v_isEqual
   //
   bool Exp_Glyph::v_isEqual(Exp const *e) const
   {
      switch(e->getName())
      {
      case Core::STR_Glyph:
         return glyph == static_cast<Exp_Glyph const *>(e)->glyph;

      default: return false;
      }
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

// EOF

