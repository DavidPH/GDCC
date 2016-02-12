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
            }
         }
      }
   }
}

// EOF

