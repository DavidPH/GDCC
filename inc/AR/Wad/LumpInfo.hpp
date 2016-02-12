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
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         LumpInfo GetLumpInfo(char const *data);
      }
   }
}

#endif//GDCC__AR__Wad__LumpInfo_H__

