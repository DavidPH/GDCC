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
         case Core::STR_clientside:
         case Core::STR_net:

         // Types.
         case Core::STR_bluereturn:
         case Core::STR_death:
         case Core::STR_disconnect:
         case Core::STR_enter:
         case Core::STR_event:
         case Core::STR_kill:
         case Core::STR_lightning:
         case Core::STR_open:
         case Core::STR_pickup:
         case Core::STR_redreturn:
         case Core::STR_reopen:
         case Core::STR_respawn:
         case Core::STR_return:
         case Core::STR_unloading:
         case Core::STR_whitereturn:
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

