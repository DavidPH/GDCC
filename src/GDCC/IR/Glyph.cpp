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
      // OArchive::writeTablesGlyphs
      //
      OArchive &OArchive::writeTablesGlyphs()
      {
         auto begin = GlyphMap.begin();
         auto end   = GlyphMap.end();

         *this << GlyphMap.size();

         for(auto itr = begin; itr != end; ++itr)
         {
            auto const &data = itr->second;

            *this << itr->first << data.type << data.value;
         }

         return *this;
      }
   }
}

// EOF

