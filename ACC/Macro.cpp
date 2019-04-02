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

#include "ACC/Macro.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // MacroMap::tempAdd
   //
   void MacroMap::tempAdd(Core::String name)
   {
      if(tempSets.empty()) return;

      tempSets.back().insert(name);
   }

   //
   // MacroMap::tempDrop
   //
   void MacroMap::tempDrop()
   {
      if(tempSets.empty()) return;

      for(auto name : tempSets.back())
         rem(name);

      tempSets.pop_back();
   }

   //
   // MacroMap::tempPush
   //
   void MacroMap::tempPush()
   {
      tempSets.emplace_back();
   }

   //
   // MacroMap::tempRem
   //
   void MacroMap::tempRem(Core::String name)
   {
      if(tempSets.empty()) return;

      tempSets.back().erase(name);
   }
}

// EOF

