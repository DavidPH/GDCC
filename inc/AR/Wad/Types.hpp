//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common typedefs and class forward declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AR__Wad__Types_H__
#define GDCC__AR__Wad__Types_H__

#include "../../AR/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::AR::Wad
{
   using LumpPath = Core::Array<Core::String>;

   enum class LumpType;

   class Lump;
   class Lump_Data;
   class Lump_Empty;
   class Lump_File;
   class Lump_FilePart;
   class Lump_Wad;
   class LumpInfo;
}

#endif//GDCC__AR__Wad__Types_H__

