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

#ifndef GDCC__IR__Glyph_H__
#define GDCC__IR__Glyph_H__

#include "../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   class Glyph
   {
   public:
      Glyph() = default;
      Glyph(Program *prog_, Core::String str_) : prog{prog_}, str{str_} {}
      Glyph(Program &prog_, Core::String str_) : prog{&prog_}, str{str_} {}

      explicit operator bool () const {return prog && str;}
      operator Core::String () const {return str;}

      bool operator == (Glyph const &glyph) const {return glyph.str == str;}
      bool operator != (Glyph const &glyph) const {return glyph.str != str;}

      GlyphData *findData() const;

      GlyphData &getData() const;


      friend OArchive &operator << (OArchive &out, Glyph const &in);
      friend IArchive &operator >> (IArchive &in, Glyph &out);

   private:
      Program     *prog;
      Core::String str;
   };

   class GlyphData
   {
   public:
      explicit GlyphData(Core::String glyph_) : glyph{glyph_} {}

      Core::String glyph;
      Type         type;
      Exp::CPtr    value;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, Glyph const &in);
   OArchive &operator << (OArchive &out, GlyphData const &in);

   IArchive &operator >> (IArchive &in, Glyph &out);
   IArchive &operator >> (IArchive &in, GlyphData &out);
}

#endif//GDCC__IR__Glyph_H__

