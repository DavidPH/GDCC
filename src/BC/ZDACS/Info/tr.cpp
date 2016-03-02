//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code translation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         //
         // Info::trFunc
         //
         void Info::trFunc()
         {
            trBlock(func->block);

            if(func->defin) switch(func->ctype)
            {
            case IR::CallType::SScriptS:
            case IR::CallType::ScriptS:
               if(!func->valueStr)
                  func->valueStr = func->glyph;
               break;

            default: break;
            }
         }
      }
   }
}

// EOF

