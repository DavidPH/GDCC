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

#include "Bytecode/ZDACS/Code.hpp"

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
               putCode(Code::Push_Lit);
               putWord(idx);
               putCode(Code::Push_WldArr);
               putWord(arr);
               putCode(Code::Jcnd_Tru);
               putWord(codeInitEnd);

               // Set initialized flag.
               putCode(Code::Push_Lit);
               putWord(idx);
               putCode(Code::Push_Lit);
               putWord(1);
               putCode(Code::Drop_WldArr);
               putWord(arr);

               // Write instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceWldArs())
                  putInitiSpace(itr, Code::Drop_WldArr);
            }

            if(isGblArr)
            {
               Core::FastU arr = getInitGblArray();
               Core::FastU idx = getInitGblIndex();

               // Check if already initialized.
               putCode(Code::Push_Lit);
               putWord(idx);
               putCode(Code::Push_GblArr);
               putWord(arr);
               putCode(Code::Jcnd_Tru);
               putWord(codeInitEnd);

               // Set initialized flag.
               putCode(Code::Push_Lit);
               putWord(idx);
               putCode(Code::Push_Lit);
               putWord(1);
               putCode(Code::Drop_GblArr);
               putWord(arr);

               // Write instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceGblArs())
                  putInitiSpace(itr, Code::Drop_GblArr);
               putInitiSpace(prog->getSpaceSta(), Code::Drop_GblArr);
            }

            putCode(Code::Rscr);
         }

         //
         // Info::putInitiSpace
         //
         void Info::putInitiSpace(IR::Space &space_, Code code)
         {
            auto const &ini = init[&space_];

            // Write instructions needed for initializers.
            for(auto const &val : ini.vals) switch(val.second.tag)
            {
            case InitTag::Empty: break;

            case InitTag::Fixed:
               // Skip zeroes.
               if(!val.second.val) break;

               putCode(Code::Push_Lit);
               putWord(val.first);
               putCode(Code::Push_Lit);
               putWord(val.second.val);
               putCode(code);
               putWord(space_.value);
               break;

            case InitTag::Funct:
               putCode(Code::Push_Lit);
               putWord(val.first);
               putStmntPushFunct(val.second.val);
               putCode(code);
               putWord(space_.value);
               break;

            case InitTag::StrEn:
               putCode(Code::Push_Lit);
               putWord(val.first);
               putStmntPushStrEn(val.second.val);
               putCode(code);
               putWord(space_.value);
               break;
            }
         }
      }
   }
}

// EOF

