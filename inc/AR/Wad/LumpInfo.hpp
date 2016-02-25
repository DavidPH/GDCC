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

#ifndef GDCC__AR__Wad__LumpInfo_H__
#define GDCC__AR__Wad__LumpInfo_H__

#include "../../AR/Wad/Lump.hpp"

#include "../../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         //
         // LumpType
         //
         enum class LumpType
         {
            Data,
            File,
            Wad,
         };

         //
         // LumpInfo
         //
         class LumpInfo
         {
         public:
            char const  *data;
            Core::String name;
            LumpType     type;
         };
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
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
         LumpInfo GetLumpInfo(char const *data);

         std::pair<Core::String, Core::String> GetNameEnd(Core::String name);

         bool IsNameBinMap(Core::String name);
         bool IsNameStart(Core::String name);
      }
   }
}

#endif//GDCC__AR__Wad__LumpInfo_H__

