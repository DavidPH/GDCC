//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/Program.hpp"


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
         // Into::putIniti
         //
         void Info::putIniti()
         {
            bool isGblArr = isInitiGblArr();
            bool isWldArr = isInitiWldArr();

            if(!isGblArr && !isWldArr)
               return;

            if(isWldArr)
            {
               Core::FastU arr = getInitWldArray();
               Core::FastU idx = getInitWldIndex();

               // Check if already initialized.
               putWord(3); // pushnumber
               putWord(idx);
               putWord(226); // pushworldarray
               putWord(arr);
               putWord(53); // ifgoto
               putWord(codeInitEnd);

               // Set initialized flag.
               putWord(3); // pushnumber
               putWord(idx);
               putWord(3); // pushnumber
               putWord(1);
               putWord(227); // assignworldarray
               putWord(arr);

               // Write instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceWldArs()) putInitiSpace(itr, 227); // assignworldarray
            }

            if(isGblArr)
            {
               Core::FastU arr = getInitGblArray();
               Core::FastU idx = getInitGblIndex();

               // Check if already initialized.
               putWord(3); // pushnumber
               putWord(idx);
               putWord(235); // pushglobalarray
               putWord(arr);
               putWord(53); // ifgoto
               putWord(codeInitEnd);

               // Set initialized flag.
               putWord(3); // pushnumber
               putWord(idx);
               putWord(3); // pushnumber
               putWord(1);
               putWord(236); // assignglobalarray
               putWord(arr);

               // Write instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceGblArs()) putInitiSpace(itr, 236); // assignglobalarray
               putInitiSpace(prog->getSpaceLocArs(), 236); // assignglobalarray
            }

            putWord(1); // terminate
         }

         //
         // Info::putInitiSpace
         //
         void Info::putInitiSpace(IR::Space &space_, Core::FastU code)
         {
            auto const &ini = init[&space_];

            // Write instructions needed for initializers.
            for(auto const &val : ini.vals) switch(val.second.tag)
            {
            case InitTag::Empty: break;

            case InitTag::Fixed:
               putWord(3); // pushnumber
               putWord(val.first);
               putWord(3); // pushnumber
               putWord(val.second.val);
               putWord(code);
               putWord(space_.value);
               break;

            case InitTag::Funct:
               putWord(3); // pushnumber
               putWord(val.first);
               putWord(359); // pushfunction
               putWord(val.second.val);
               putWord(code);
               putWord(space_.value);
               break;

            case InitTag::StrEn:
               putWord(3); // pushnumber
               putWord(val.first);
               putWord(3); // pushnumber
               putWord(val.second.val);
               putWord(225); //tagstring
               putWord(code);
               putWord(space_.value);
               break;
            }
         }
      }
   }
}

// EOF

