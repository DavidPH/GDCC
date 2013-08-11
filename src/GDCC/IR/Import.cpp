//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation library import handling.
//
//-----------------------------------------------------------------------------

#include "Import.hpp"

#include "IArchive.hpp"
#include "OArchive.hpp"


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::unordered_map<GDCC::String, GDCC::IR::Import> Table;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // IArchive::getTablesImport
      //
      IArchive &IArchive::getTablesImport()
      {
         for(auto count = GetIR<std::size_t>(*this); count--;)
            *this >> Import::Get(GetIR<String>(*this));

         return *this;
      }

      //
      // Import constructor
      //
      Import::Import(String glyph_) :
         glyph{glyph_}
      {
      }

      //
      // Import::Get
      //
      Import &Import::Get(String glyph)
      {
         auto itr = Table.find(glyph);

         if(itr == Table.end())
            itr = Table.emplace(std::piecewise_construct,
               std::forward_as_tuple(glyph), std::forward_as_tuple(glyph)).first;

         return itr->second;
      }

      //
      // OArchive::putTablesImport
      //
      OArchive &OArchive::putTablesImport()
      {
         *this << Table.size();
         for(auto const &itr : Table)
            *this << itr.first << itr.second;

         return *this;
      }

      //
      // operator OArchive << Import
      //
      OArchive &operator << (OArchive &out, Import const &)
      {
         return out;
      }

      //
      // operator IArchive >> Import
      //
      IArchive &operator >> (IArchive &in, Import &)
      {
         return in;
      }

      //
      // ImportRange
      //
      Range<MemItr<std::unordered_map<String, Import>::iterator>> ImportRange()
      {
         return MakeRange(
            MemItr<std::unordered_map<String, Import>::iterator>(Table.begin()),
            MemItr<std::unordered_map<String, Import>::iterator>(Table.end()));
      }
   }
}

// EOF

