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

#include "Core/Dir.hpp"
#include "Core/File.hpp"
#include "Core/Path.hpp"

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
         // AddWadDirMap
         //
         static void AddWadDirMap(Lump_Wad *wadLump, Core::String nameHead,
            Core::DirStream *dir)
         {
            Wad *wad = &wadLump->wad;

            // UDMF core lumps.
            std::unique_ptr<Lump> lumpTEXTMAP;
            std::unique_ptr<Lump> lumpENDMAP;

            // Binary map lumps.
            std::unique_ptr<Lump> lumpTHINGS;
            std::unique_ptr<Lump> lumpLINEDEFS;
            std::unique_ptr<Lump> lumpSIDEDEFS;
            std::unique_ptr<Lump> lumpVERTEXES;
            std::unique_ptr<Lump> lumpSEGS;
            std::unique_ptr<Lump> lumpSSECTORS;
            std::unique_ptr<Lump> lumpNODES;
            std::unique_ptr<Lump> lumpSECTORS;
            std::unique_ptr<Lump> lumpREJECT;
            std::unique_ptr<Lump> lumpBLOCKMAP;
            std::unique_ptr<Lump> lumpBEHAVIOR;

            // Unsorted lumps.
            std::vector<std::unique_ptr<Lump>> lumps;

            while(dir->next())
            {
               Core::String name = GetNameFromFile(dir->getPart());
               char const  *path = dir->getFull();

               if(Core::IsDir(path))
               {
                  // TODO
               }
               else
               {
                  std::unique_ptr<Lump> lump{new Lump_File{name, Core::StrDup(path)}};

                  if(name == nameHead)
                     wadLump->head = std::move(lump);

                  else if(name == NameTEXTMAP)  lumpTEXTMAP  = std::move(lump);
                  else if(name == NameENDMAP)   lumpENDMAP   = std::move(lump);
                  else if(name == NameTHINGS)   lumpTHINGS   = std::move(lump);
                  else if(name == NameLINEDEFS) lumpLINEDEFS = std::move(lump);
                  else if(name == NameSIDEDEFS) lumpSIDEDEFS = std::move(lump);
                  else if(name == NameVERTEXES) lumpVERTEXES = std::move(lump);
                  else if(name == NameSEGS)     lumpSEGS     = std::move(lump);
                  else if(name == NameSSECTORS) lumpSSECTORS = std::move(lump);
                  else if(name == NameNODES)    lumpNODES    = std::move(lump);
                  else if(name == NameSECTORS)  lumpSECTORS  = std::move(lump);
                  else if(name == NameREJECT)   lumpREJECT   = std::move(lump);
                  else if(name == NameBLOCKMAP) lumpBLOCKMAP = std::move(lump);
                  else if(name == NameBEHAVIOR) lumpBEHAVIOR = std::move(lump);

                  else
                     lumps.push_back(std::move(lump));
               }
            }

            if(!wadLump->head)
               wadLump->head.reset(new Lump_Empty{nameHead});

            if(lumpTEXTMAP)
            {
               wad->addLump(lumpTEXTMAP.release());

               // Add binary map lumps.
               if(lumpTHINGS)   wad->addLump(lumpTHINGS.release());
               if(lumpLINEDEFS) wad->addLump(lumpLINEDEFS.release());
               if(lumpSIDEDEFS) wad->addLump(lumpSIDEDEFS.release());
               if(lumpVERTEXES) wad->addLump(lumpVERTEXES.release());
               if(lumpSEGS)     wad->addLump(lumpSEGS.release());
               if(lumpSSECTORS) wad->addLump(lumpSSECTORS.release());
               if(lumpNODES)    wad->addLump(lumpNODES.release());
               if(lumpSECTORS)  wad->addLump(lumpSECTORS.release());
               if(lumpREJECT)   wad->addLump(lumpREJECT.release());
               if(lumpBLOCKMAP) wad->addLump(lumpBLOCKMAP.release());
               if(lumpBEHAVIOR) wad->addLump(lumpBEHAVIOR.release());

               // Add unsorted lumps.
               for(auto &lump : lumps)
                  wad->addLump(lump.release());

               if(lumpENDMAP)
                  wadLump->tail = std::move(lumpENDMAP);
               else
                  wadLump->tail.reset(new Lump_Empty{NameENDMAP});
            }
            else
            {
               #define DoLump(name) \
                  if(lump##name) wad->addLump(lump##name.release()); \
                  else           wad->addLump(new Lump_Empty{Name##name})

               DoLump(THINGS);
               DoLump(LINEDEFS);
               DoLump(SIDEDEFS);
               DoLump(VERTEXES);
               DoLump(SEGS);
               DoLump(SSECTORS);
               DoLump(NODES);
               DoLump(SECTORS);
               DoLump(REJECT);
               DoLump(BLOCKMAP);

               #undef DoLump

               if(lumpBEHAVIOR) wad->addLump(lumpBEHAVIOR.release());
            }
         }

         //
         // AddWadDirStart
         //
         static void AddWadDirStart(Lump_Wad *wadLump, Core::String nameHead,
            std::pair<Core::String, Core::String> nameTail, Core::DirStream *dir)
         {
            while(dir->next())
            {
               Core::String name = GetNameFromFile(dir->getPart());
               char const  *path = dir->getFull();

               if(Core::IsDir(path))
               {
                  // TODO
               }
               else
               {
                  Lump *lump = new Lump_File{name, Core::StrDup(path)};

                  if(name == nameTail.first || name == nameTail.second)
                     wadLump->tail.reset(lump);
                  else if(name == nameHead)
                     wadLump->head.reset(lump);
                  else
                     wadLump->wad.addLump(lump);
               }
            }

            if(!wadLump->head) wadLump->head.reset(new Lump_Empty{nameHead});
            if(!wadLump->tail) wadLump->tail.reset(new Lump_Empty{nameTail.first});
         }

         //
         // AddWadDir
         //
         static void AddWadDir(Wad *wad, Core::String name, Core::DirStream *dir)
         {
            std::unique_ptr<Lump_Wad> wadLump{new Lump_Wad{name}};
            wadLump->embed = true;

            if(IsNameStart(name))
               AddWadDirStart(wadLump.get(), name, GetNameEnd(name), dir);
            else
               AddWadDirMap (wadLump.get(), name, dir);

            wad->addLump(wadLump.release());
         }

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

         //
         // CreateLump_File
         //
         static Lump *CreateLump_File(LumpInfo const &info)
         {
            auto name = info.name;

            if(!name)
               name = GetNameFromFile(Core::PathFilename(info.data));

            return new Lump_File{name, Core::StrDup(info.data)};
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
               addLump(CreateLump_File(info));
               break;

            case LumpType::Wad:
               if(Core::IsDir(info.data))
                  addLump_WadDir(info);
               else
                  addLump_WadFile(info);
               break;
            }
         }

         //
         // Wad::addLump_WadDir
         //
         void Wad::addLump_WadDir(LumpInfo const &info)
         {
            // Determine what Wad object to insert lumps into.
            Wad                      *wad;
            std::unique_ptr<Lump_Wad> wadLump;
            if(info.name)
            {
               wadLump.reset(new Lump_Wad{info.name});
               wad = &wadLump->wad;
            }
            else
               wad = this;

            // Find and insert lumps.
            for(auto dir = Core::DirOpenStream(info.data); dir->next();)
            {
               Core::String name = GetNameFromFile(dir->getPart());
               char const  *path = dir->getFull();

               if(Core::IsDir(path))
                  AddWadDir(wad, name, Core::DirOpenStream(path).get());
               else
                  wad->addLump(new Lump_File{name, Core::StrDup(path)});
            }

            // Insert wad lump, if any.
            if(wadLump)
               addLump(wadLump.release());
         }

         //
         // Wad::addLump_WadFile
         //
         void Wad::addLump_WadFile(LumpInfo const &info)
         {
            // Load file data.
            std::shared_ptr<Core::FileBlock> data
               {Core::FileOpenBlock(info.data).release()};

            // Parse wad header.
            WadHeader header{data->data(), data->size()};

            // Determine what Wad object to insert lumps into.
            Wad                      *wad;
            std::unique_ptr<Lump_Wad> wadLump;
            if(info.name)
            {
               wadLump.reset(new Lump_Wad{info.name});
               wadLump->wad.iwad = header.ident[0] == 'I';
               wad = &wadLump->wad;
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

            // Insert wad lump, if any.
            if(wadLump)
               addLump(wadLump.release());
         }
      }
   }
}

// EOF

