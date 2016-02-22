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

         //
         // Lump_Wad constructor
         //
         Lump_Wad::Lump_Wad(Core::String name_) :
            Lump{name_},
            embed{false}
         {
         }

         //
         // Lump_Wad::sizeData
         //
         std::size_t Lump_Wad::sizeData() const
         {
            if(embed)
            {
               std::size_t n = 0;

               if(head)
                  n += head->sizeData();

               for(auto const &lump : wad)
                  n += lump.sizeData();

               if(tail)
                  n += tail->sizeData();

               return n;
            }
            else
               return wad.sizeData();
         }

         //
         // Lump_Wad::sizeHead
         //
         std::size_t Lump_Wad::sizeHead() const
         {
            if(embed)
               return wad.size() + !!head + !!tail;
            else
               return 1;
         }

         //
         // Lump_Wad::writeData
         //
         void Lump_Wad::writeData(std::ostream &out) const
         {
            if(embed)
            {
               if(head)
                  head->writeData(out);

               for(Lump const &lump : wad)
                  lump.writeData(out);

               if(tail)
                  tail->writeData(out);
            }
            else
               wad.writeData(out);
         }

         //
         // Lump_Wad::writeHead
         //
         void Lump_Wad::writeHead(std::ostream &out, std::size_t offset) const
         {
            if(embed)
            {
               if(head)
                  head->writeHead(out, offset), offset += head->sizeData();

               for(Lump const &lump : wad)
                  lump.writeHead(out, offset), offset += lump.sizeData();

               if(tail)
                  tail->writeHead(out, offset);
            }
            else
               Lump::writeHead(out, offset);
         }
      }
   }
}

// EOF

