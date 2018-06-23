//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code translation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/CallType.hpp"
#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::trFunc
   //
   void Info::trFunc()
   {
      trBlock(func->block);

      switch(func->ctype)
      {
      case IR::CallType::SScriptS:
      case IR::CallType::ScriptS:
         if(!func->valueStr)
            func->valueStr = func->glyph;

         // Check script types.
         for(auto const &st : func->stype) switch(st)
         {
         // Flags.
         case Core::STR_Clientside:
         case Core::STR_Net:

         // Types.
         case Core::STR_BlueReturn:
         case Core::STR_Death:
         case Core::STR_Disconnect:
         case Core::STR_Enter:
         case Core::STR_Event:
         case Core::STR_Kill:
         case Core::STR_Lightning:
         case Core::STR_Open:
         case Core::STR_Pickup:
         case Core::STR_RedReturn:
         case Core::STR_Reopen:
         case Core::STR_Respawn:
         case Core::STR_Return:
         case Core::STR_Unloading:
         case Core::STR_WhiteReturn:
            break;

         default:
            throw Core::ParseExceptExpect(func->getOrigin(),
               "ZDACS script type", st, false);
         }

         break;

      default: break;
      }
   }
}

// EOF

