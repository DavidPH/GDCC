//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS initializer generator.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::genIniti
         //
         void Info::genIniti()
         {
            bool isGblArr = isInitiGblArr();
            bool isHubArr = isInitiHubArr();

            if(!isGblArr && !isHubArr)
               return;

            ++numChunkSPTR;

            // Save index for initializer start.
            codeInit = CodeBase() + numChunkCODE;

            // Initialize world arrays.
            if(isHubArr)
            {
               // Check if already initialized.
               // push_lit push_arr cjmp_tru
               numChunkCODE += 24;

               // Mark as initialized.
               // push_lit push_lit drop_arr
               numChunkCODE += 24;

               // Count instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceHubArs()) genInitiSpace(itr);
            }

            // Initialize global arrays.
            if(isGblArr)
            {
               // Check if already initialized.
               // push_lit push_arr cjmp_tru
               numChunkCODE += 24;

               // Mark as initialized.
               // push_lit push_lit drop_arr
               numChunkCODE += 24;

               // Count instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceGblArs()) genInitiSpace(itr);
               genInitiSpace(prog->getSpaceSta());
            }

            // Save index for initializer end.
            codeInitEnd = CodeBase() + numChunkCODE;

            // Terminate script.
            // term
            numChunkCODE += 4;
         }

         //
         // Info::genInitiSpace
         //
         void Info::genInitiSpace(IR::Space &space_)
         {
            auto const &ini = init[&space_];

            // Count instructions needed for initializers.
            for(auto const &val : ini.vals) switch(val.second.tag)
            {
            case InitTag::Empty: break;

            case InitTag::Fixed:
               if(val.second.val)
                  numChunkCODE += 24;
               break;

            case InitTag::Funct: numChunkCODE += 24; break;
            case InitTag::StrEn: numChunkCODE += 28; break;
            }
         }
      }
   }
}

// EOF

