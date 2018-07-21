//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Extra macro handling for ACS.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Macro_H__
#define GDCC__ACC__Macro_H__

#include "../ACC/Types.hpp"

#include "../CPP/Macro.hpp"

#include <unordered_set>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // MacroMap
   //
   class MacroMap : public CPP::MacroMap
   {
   public:
      using CPP::MacroMap::MacroMap;

      void tempAdd(Core::String name);

      void tempDrop();

      void tempPush();

      void tempRem(Core::String name);

   protected:
      std::vector<std::unordered_set<Core::String>> tempSets;
   };
}

#endif//GDCC__ACC__Macro_H__

