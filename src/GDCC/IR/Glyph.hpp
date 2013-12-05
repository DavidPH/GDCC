//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation glyph/symbol handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Glyph_H__
#define GDCC__IR__Glyph_H__

#include "Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Glyph;
      class GlyphData;
      class OArchive;
      class Program;

      class Glyph
      {
      public:
         Glyph() = default;
         Glyph(Program *prog_, String str_) : prog{prog_}, str{str_} {}

         explicit operator bool () const {return prog && str;}
         explicit operator String () const {return str;}

         bool operator == (Glyph const &glyph) const {return glyph.str == str;}
         bool operator != (Glyph const &glyph) const {return glyph.str != str;}

         GlyphData *findData() const;

         GlyphData &getData() const;


         friend OArchive &operator << (OArchive &out, Glyph const &in);
         friend IArchive &operator >> (IArchive &in, Glyph &out);

      private:
         Program *prog;
         String   str;
      };

      class GlyphData
      {
      public:
         explicit GlyphData(String glyph_) : glyph{glyph_} {}

         String    glyph;
         Type      type;
         Exp::CPtr value;
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
      OArchive &operator << (OArchive &out, Glyph const &in);
      OArchive &operator << (OArchive &out, GlyphData const &in);

      IArchive &operator >> (IArchive &in, Glyph &out);
      IArchive &operator >> (IArchive &in, GlyphData &out);
   }
}

#endif//GDCC__IR__Glyph_H__

