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

#ifndef GDCC__AR__Wad__LumpInfo_H__
#define GDCC__AR__Wad__LumpInfo_H__

#include "../../AR/Wad/Lump.hpp"

#include "../../Core/Array.hpp"
#include "../../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::AR::Wad
{
   //
   // LumpType
   //
   enum class LumpType
   {
      Data,
      File,
      Special,
      Wad,
   };

   //
   // LumpInfo
   //
   class LumpInfo
   {
   public:
      bool dataBool() const;

      LumpInfo dataInfo() const;

      LumpPath     path;
      Core::String name;
      char const  *data;
      LumpType     type;
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::AR::Wad
{
   extern Core::String const Name_embed;
   extern Core::String const Name_head;
   extern Core::String const Name_tail;

   extern Core::String const NameBEHAVIOR;
   extern Core::String const NameBLOCKMAP;
   extern Core::String const NameENDMAP;
   extern Core::String const NameLINEDEFS;
   extern Core::String const NameNODES;
   extern Core::String const NameREJECT;
   extern Core::String const NameSECTORS;
   extern Core::String const NameSEGS;
   extern Core::String const NameSIDEDEFS;
   extern Core::String const NameSSECTORS;
   extern Core::String const NameTEXTMAP;
   extern Core::String const NameTHINGS;
   extern Core::String const NameVERTEXES;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AR::Wad
{
   Core::String GetFileFromName(Core::String name);

   LumpInfo GetLumpInfo(char const *data);

   std::pair<Core::String, Core::String> GetNameEnd(Core::String name);

   Core::String GetNameFromFile(char const *filename);

   bool IsNameBinMap(Core::String name);
   bool IsNameStart(Core::String name);
}

#endif//GDCC__AR__Wad__LumpInfo_H__

