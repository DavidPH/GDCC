//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Wad lump adding.
//
//-----------------------------------------------------------------------------

#include "AR/Wad/Wad.hpp"

#include "AR/Wad/LumpInfo.hpp"
#include "AR/Wad/WadHeader.hpp"

#include "Core/File.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         //
         // CreateLump_Data
         //
         static Lump *CreateLump_Data(LumpInfo const &info)
         {
            if(info.name)
            {
               std::size_t size = std::strlen(info.data);
               if(size)
                  return new Lump_Data(info.name, Core::StrDup(info.data, size), size);
               else
                  return new Lump_Empty(info.name);
            }
            else
               return new Lump_Empty(info.data);
         }

         //
         // CreateLump_Data
         //
         static Lump *CreateLump_Data(WadHeader::Lump const &lump,
            std::shared_ptr<Core::FileBlock> const &data)
         {
            if(lump.size)
               return new Lump_FilePart{lump.name, lump.data, lump.size, data};
            else
               return new Lump_Empty{lump.name};
         }
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
         // Wad::addLump
         //
         void Wad::addLump(LumpInfo const &info)
         {
            switch(info.type)
            {
            case LumpType::Data:
               addLump(CreateLump_Data(info));
               break;

            case LumpType::File:
               addLump(new Lump_File(info.name, Core::StrDup(info.data)));
               break;

            case LumpType::Wad:
               addLump_Wad(info);
               break;
            }
         }

         //
         // Wad::addLump_Wad
         //
         void Wad::addLump_Wad(LumpInfo const &info)
         {
            // Load file data.
            std::shared_ptr<Core::FileBlock> data
               {Core::FileOpenBlock(info.data).release()};

            // Parse wad header.
            WadHeader header{data->data(), data->size()};

            // Determine what Wad object to insert lumps into.
            // FIXME: This inserts the Lump_Wad object even if errors occur.
            Wad *wad;
            if(info.name)
            {
               Lump_Wad *lump = new Lump_Wad{info.name};

               lump->wad.iwad = header.ident[0] == 'I';

               addLump(lump);
               wad = &lump->wad;
            }
            else
               wad = this;

            // Insert lumps.
            for(WadHeader::Dir const &dir : header.findDirs())
            {
               if(dir.name)
               {
                  std::unique_ptr<Lump_Wad> sub{new Lump_Wad{dir.name}};

                  sub->embed = true;

                  if(dir.head)
                     sub->head.reset(CreateLump_Data(*dir.head, data));

                  for(WadHeader::Lump const &lump : dir.lumps)
                     sub->wad.addLump(CreateLump_Data(lump, data));

                  if(dir.tail)
                     sub->tail.reset(CreateLump_Data(*dir.tail, data));

                  wad->addLump(sub.release());
               }
               else
               {
                  for(WadHeader::Lump const &lump : dir.lumps)
                     wad->addLump(CreateLump_Data(lump, data));
               }
            }
         }
      }
   }
}

// EOF

