//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Wad handling.
//
//-----------------------------------------------------------------------------

#include "AR/Wad/Wad.hpp"

#include "AR/BinaryIO.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         //
         // Wad constructor
         //
         Wad::Wad() :
            iwad{false}
         {
         }

         //
         // Wad destructor
         //
         Wad::~Wad()
         {
            while(head.wadNext != &head)
               delete head.wadNext;
         }

         //
         // Wad::addLump
         //
         void Wad::addLump(Lump *lump)
         {
            ListUtil::Insert(lump, &head);
         }

         //
         // Wad::size
         //
         std::size_t Wad::size() const
         {
            return std::distance(begin(), end());
         }

         //
         // Wad::sizeData
         //
         std::size_t Wad::sizeData() const
         {
            std::size_t n = 16;
            for(auto const &lump : *this)
               n += lump.sizeHead() * 16 + lump.sizeData();
            return n;
         }

         //
         // Wad::writeData
         //
         void Wad::writeData(std::ostream &out) const
         {
            // Count number of lumps.
            std::size_t numLumps = 0;
            for(auto const &lump : *this)
               numLumps += lump.sizeHead();

            // Write archive header.
            std::size_t offset = 16;
            out.write(iwad ? "IWAD" : "PWAD", 4);
            WriteLE4(out, numLumps);
            WriteLE4(out, offset);
            out.write("GDCC", 4);

            // Write lump headers.
            offset += numLumps * 16;
            for(auto const &lump : *this)
            {
               lump.writeHead(out, offset);
               offset += lump.sizeData();
            }

            // Write lump data.
            for(auto const &lump : *this)
               lump.writeData(out);
         }
      }
   }
}

// EOF

