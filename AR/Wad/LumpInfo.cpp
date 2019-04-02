//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Lump loading information.
//
//-----------------------------------------------------------------------------

#include "AR/Wad/LumpInfo.hpp"

#include "Core/Exception.hpp"

#include <algorithm>
#include <cstring>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::AR::Wad
{
   extern Core::String const Name_embed{"embed"};
   extern Core::String const Name_head {"head"};
   extern Core::String const Name_tail {"tail"};

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


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::AR::Wad
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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AR::Wad
{
   //
   // LumpInfo::dataBool
   //
   bool LumpInfo::dataBool() const
   {
      if(!std::strcmp(data, "false") || !std::strcmp(data, "0"))
         return false;

      if(!std::strcmp(data, "true") || !std::strcmp(data, "1"))
         return true;

      Core::ErrorExpect({}, "boolean", data);
   }

   //
   // LumpInfo::dataInfo
   //
   LumpInfo LumpInfo::dataInfo() const
   {
      return GetLumpInfo(data);
   }

   //
   // GetFileFromName
   //
   Core::String GetFileFromName(Core::String name)
   {
      // Translate \ to ^.
      if(std::find(name.begin(), name.end(), '\\'))
      {
         std::unique_ptr<char[]> tmp{new char[name.size()]};

         auto itr = name.begin(), end = name.end();
         for(char *out = tmp.get(); itr != end; ++itr, ++out)
            *out = *itr == '\\' ? '^' : *itr;

         return {&tmp[0], &tmp[name.size()]};
      }
      else
         return name;
   }

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

      else if(CheckLumpType(data, "special:"))
         info.type = LumpType::Special, data += 8;

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
      else if(char const *eq = std::strchr(data, '='))
      {
         if(std::size_t pathN = std::count(data, eq, '/'))
         {
            for(auto &path : info.path = LumpPath{pathN})
            {
               char const *sep = strchr(data, '/');
               path = {data, sep};
               data = sep + 1;
            }
         }

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
   // GetNameFromFile
   //
   Core::String GetNameFromFile(char const *filename)
   {
      char const *end = std::strchr(filename, '.');
      if(!end) end = filename + std::strlen(filename);
      std::size_t len = end - filename;

      // Translate ^ to \.
      if(std::find(filename, end, '^'))
      {
         std::unique_ptr<char[]> tmp{new char[len]};

         char const *itr = filename;
         for(char *out = tmp.get(); itr != end; ++itr, ++out)
            *out = *itr == '^' ? '\\' : *itr;

         return {&tmp[0], &tmp[len]};
      }
      else
         return {filename, end};
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

// EOF

