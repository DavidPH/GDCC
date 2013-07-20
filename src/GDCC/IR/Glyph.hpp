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

      class Glyph
      {
      public:
         Glyph() = default;
         explicit constexpr Glyph(String str_) : str{str_} {}

         explicit constexpr operator bool () const {return str != STRNULL;}
         explicit constexpr operator String () const {return str;}

         constexpr bool operator == (Glyph const &glyph) const {return glyph.str == str;}
         constexpr bool operator != (Glyph const &glyph) const {return glyph.str != str;}

         GlyphData *findData() const {return FindData(str);}

         GlyphData &getData() const {return GetData(str);}


         friend OArchive &operator << (OArchive &out, Glyph const &in);
         friend IArchive &operator >> (IArchive &in, Glyph &out);

         static GlyphData *FindData(String str);

         static GlyphData &GetData(String str);

      private:
         String str;
      };

      class GlyphData
      {
      public:
         Type     type;
         Exp::Ptr value;
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

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << Glyph
      //
      inline OArchive &operator << (OArchive &out, Glyph const &in)
      {
         return out << in.str;
      }

      //
      // operator OArchive >> Glyph
      //
      inline IArchive &operator >> (IArchive &in, Glyph &out)
      {
         return in >> out.str;
      }
   }
}

#endif//GDCC__IR__Glyph_H__

