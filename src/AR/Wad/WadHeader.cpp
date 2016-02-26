//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Wad on-disk format handling.
//
//-----------------------------------------------------------------------------

#include "AR/Wad/WadHeader.hpp"

#include "AR/Wad/LumpInfo.hpp"

#include "AR/Exception.hpp"

#include "Core/BinaryIO.hpp"


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
         // WadHeader constructor
         //
         WadHeader::WadHeader(char const *data, std::size_t size)
         {
            // A valid wad archive must be at least 12 bytes.
            if(size < 12)
               throw ReadError("wad header out of bounds");

            // Read wad header.
            ident = {{data[0], data[1], data[2], data[3]}};
            std::size_t count = Core::ReadLE4(data + 4);
            std::size_t iter  = Core::ReadLE4(data + 8);

            // Must both contain the index and have enough space there.
            if(size < iter || size - iter < count * 16)
              throw ReadError("wad table out of bounds");

            // Read lump headers.
            for(Lump &lump : lumps = Core::Array<Lump>{count})
            {
               std::size_t idx = Core::ReadLE4 (data + iter);    iter += 4;
               lump.size       = Core::ReadLE4 (data + iter);    iter += 4;
               lump.name       = Core::ReadStrN(data + iter, 8); iter += 8;

               // Must both contain the index and have enough space there.
               if(size < idx || size - idx < lump.size)
                  throw ReadError("wad lump out of bounds");

               lump.data = data + idx;
            }
         }

         //
         // WadHeader::findDirs
         //
         Core::Array<WadHeader::Dir> WadHeader::findDirs() const
         {
            std::vector<Dir> found;

            auto first = lumps.begin(), iter = first, last = lumps.end();

            for(; iter != last;)
            {
               // Detect X_START.
               if(IsNameStart(iter->name))
               {
                  if(first != iter)
                     found.push_back({{first, iter}, nullptr, nullptr, nullptr});

                  auto head = iter;
                  first = ++iter;

                  for(auto ends = GetNameEnd(head->name);; ++iter)
                  {
                     if(iter == last)
                        throw ReadError("unterminated X_START");

                     if(iter->name == ends.first || iter->name == ends.second)
                        break;
                  }

                  found.push_back({{first, iter}, head, iter, head->name});

                  first = ++iter;
               }
               // Detect binary maps.
               else if(iter + 1 != last && IsNameBinMap((iter + 1)->name))
               {
                  if(first != iter)
                     found.push_back({{first, iter}, nullptr, nullptr, nullptr});

                  auto head = iter;
                  first = ++iter;

                  for(; iter != last && IsNameBinMap(iter->name); ++iter) {}

                  found.push_back({{first, iter}, head, nullptr, head->name});

                  first = iter;
               }
               // Detect UDMF maps.
               else if(iter + 1 != last && (iter + 1)->name == NameTEXTMAP)
               {
                  if(first != iter)
                     found.push_back({{first, iter}, nullptr, nullptr, nullptr});

                  auto head = iter;
                  first = ++iter;

                  while(iter->name != NameENDMAP)
                  {
                     if(++iter == last)
                        throw ReadError("unterminated TEXTMAP");
                  }

                  found.push_back({{first, iter}, head, iter, head->name});

                  first = ++iter;
               }
               else
                  ++iter;
            }

            if(first != last)
               found.push_back({{first, last}, nullptr, nullptr, nullptr});

            return {Core::Move, found.begin(), found.end()};
         }
      }
   }
}

// EOF

