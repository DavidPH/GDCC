//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation glyph/symbol handling.
//
//-----------------------------------------------------------------------------

#include "IR/Glyph.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // Glyph::findData
   //
   GlyphData *Glyph::findData() const
   {
      return prog->findGlyphData(str);
   }

   //
   // Glyph::getData
   //
   GlyphData &Glyph::getData() const
   {
      return prog->getGlyphData(str);
   }
   //
   // operator OArchive << Glyph
   //
   OArchive &operator << (OArchive &out, Glyph const &in)
   {
      return out << in.str;
   }

   //
   // operator OArchive << GlyphData
   //
   OArchive &operator << (OArchive &out, GlyphData const &in)
   {
      return out << in.type << in.value;
   }

   //
   // operator IArchive >> Glyph
   //
   IArchive &operator >> (IArchive &in, Glyph &out)
   {
      out.prog = in.prog;
      return in >> out.str;
   }

   //
   // operator IArchive >> GlyphData
   //
   IArchive &operator >> (IArchive &in, GlyphData &out)
   {
      return in >> out.type >> out.value;
   }
}

// EOF

