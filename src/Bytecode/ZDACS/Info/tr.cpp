//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code translation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
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
            case IR::CallType::Script:
               func->ctype = IR::CallType::ScriptI;
               break;

            default: break;
            }
         }
      }
   }
}

// EOF

