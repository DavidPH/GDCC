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

            else if(CheckLumpType(data, "wad:"))
               info.type = LumpType::Wad, data += 4;

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

