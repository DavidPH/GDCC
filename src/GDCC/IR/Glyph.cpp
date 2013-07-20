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

#include "Glyph.hpp"

#include "IArchive.hpp"
#include "OArchive.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::unordered_map<GDCC::String, GDCC::IR::GlyphData> GlyphMap;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Glyph::FindData
      //
      GlyphData *Glyph::FindData(String str)
      {
         auto itr = GlyphMap.find(str);

         return itr == GlyphMap.end() ? nullptr : &itr->second;
      }

      //
      // Glyph::GetData
      //
      GlyphData &Glyph::GetData(String str)
      {
         return GlyphMap[str];
      }

      //
      // IArchive::getTablesGlyphs
      //
      IArchive &IArchive::getTablesGlyphs()
      {
         auto count = getNumber<std::size_t>();

         String name;
         GlyphData newData;

         while(count--)
         {
            *this >> name >> newData;
            auto &oldData = Glyph::GetData(name);

            if(newData.type.t != TypeBase::Empty)
            {
               if(oldData.type.t == TypeBase::Empty)
                  oldData.type = newData.type;
            }

            if(newData.value)
            {
               if(!oldData.value)
                  oldData.value = newData.value;
            }
         }

         return *this;
      }

      //
      // OArchive::writeTablesGlyphs
      //
      OArchive &OArchive::writeTablesGlyphs()
      {
         *this << GlyphMap.size();

         for(auto const &itr : GlyphMap)
            *this << itr.first << itr.second;

         return *this;
      }

      //
      // operator OArchive << GlyphData
      //
      OArchive &operator << (OArchive &out, GlyphData const &in)
      {
         return out << in.type << in.value;
      }

      //
      // operator IArchive >> GlyphData
      //
      IArchive &operator >> (IArchive &in, GlyphData &out)
      {
         return in >> out.type >> out.value;
      }
   }
}

// EOF

