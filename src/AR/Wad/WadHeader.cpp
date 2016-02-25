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

#include "AR/BinaryIO.hpp"
#include "AR/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         extern Core::String const NameBEHAVIOR{"BEHAVIOR"};
         extern Core::String const NameBLOCKMAP{"BLOCKMAP"};
         extern Core::String const NameENDMAP  {"ENDMAP"};
         extern Core::String const NameLINEDEFS{"LINEDEFS"};
         extern Core::String const NameNODES   {"NODES"};
         extern Core::String const NameREJECT  {"REJECT"};
         extern Core::String const NameSECTORS {"SECTORS"};
         extern Core::String const NameSEGS    {"SEGS"};
         extern Core::String const NameSIDEDEFS{"SIDEDEFS"};
         extern Core::String const NameSSECTORS{"SSECTORS"};
         extern Core::String const NameTEXTMAP {"TEXTMAP"};
         extern Core::String const NameTHINGS  {"THINGS"};
         extern Core::String const NameVERTEXES{"VERTEXES"};
      }
   }
}


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
            std::size_t count = ReadLE4(data + 4);
            std::size_t iter  = ReadLE4(data + 8);

            // Must both contain the index and have enough space there.
            if(size < iter || size - iter < count * 16)
              throw ReadError("wad table out of bounds");

            // Read lump headers.
            for(Lump &lump : lumps = Core::Array<Lump>{count})
            {
               std::size_t idx = ReadLE4 (data + iter);    iter += 4;
               lump.size       = ReadLE4 (data + iter);    iter += 4;
               lump.name       = ReadStrN(data + iter, 8); iter += 8;

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

         //
         // GetNameEnd
         //
         std::pair<Core::String, Core::String> GetNameEnd(Core::String name)
         {
            char s[7]{name[0], name[1], '_', 'E', 'N', 'D'};

            if(s[1] == '_')
               s[1] = s[0];

            // FIXME: First should always be the canonical name.
            if(s[1] == s[0])
               return {s, s + 1};
            else
               return {s, nullptr};
         }

         //
         // IsNameBinMap
         //
         bool IsNameBinMap(Core::String name)
         {
            return
               name == NameTHINGS   ||
               name == NameLINEDEFS ||
               name == NameSIDEDEFS ||
               name == NameVERTEXES ||
               name == NameSEGS     ||
               name == NameSSECTORS ||
               name == NameNODES    ||
               name == NameSECTORS  ||
               name == NameREJECT   ||
               name == NameBLOCKMAP ||
               name == NameBEHAVIOR;
         }

         //
         // IsNameStart
         //
         bool IsNameStart(Core::String name)
         {
            switch(name.size())
            {
            case 7: return std::equal(name.begin() + 1, name.end(), "_START");
            case 8: return std::equal(name.begin() + 2, name.end(), "_START");
            default: return false;
            }
         }
      }
   }
}

// EOF

