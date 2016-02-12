//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Lump loading information.
//
//-----------------------------------------------------------------------------

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
         // CheckLumpType
         //
         template<std::size_t N>
         static bool CheckLumpType(char const *data, char const (&type)[N])
         {
            return std::equal(type, type + (N-1), data);
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
         // GetLumpInfo
         //
         LumpInfo GetLumpInfo(char const *data)
         {
            LumpInfo info;

            // Get type.
            if(data[0] == ':')
               info.type = LumpType::File, data += 1;

            else if(CheckLumpType(data, "data:"))
               info.type = LumpType::Data, data += 5;

            else if(CheckLumpType(data, "file:"))
               info.type = LumpType::File, data += 5;

            else
               info.type = LumpType::File;

            // Get name.
            if(data[0] == '=')
            {
               info.name = nullptr;
               data     += 1;
            }
            else if(auto eq = std::strchr(data, '='))
            {
               // TODO: Process path.

               info.name = {data, eq};
               data      = eq + 1;
            }
            else
               info.name = nullptr;

            // Data is the remainder of the string.
            info.data = data;

            return info;
         }
      }
   }
}

// EOF

