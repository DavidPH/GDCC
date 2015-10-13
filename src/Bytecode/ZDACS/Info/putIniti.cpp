//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
// Extern Functions                                                           |
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
            bool isHubArr = isInitiHubArr();

            if(!isGblArr && !isHubArr)
               return;

            if(isHubArr)
            {
               Core::FastU arr = getInitHubArray();
               Core::FastU idx = getInitHubIndex();

               // Check if already initialized.
               putCode(Code::Push_Lit);
               putWord(idx);
               putCode(Code::Push_HubArr);
               putWord(arr);
               putCode(Code::Jcnd_Tru);
               putWord(codeInitEnd);

               // Set initialized flag.
               putCode(Code::Push_Lit);
               putWord(idx);
               putCode(Code::Push_Lit);
               putWord(1);
               putCode(Code::Drop_HubArr);
               putWord(arr);

               // Write instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceHubArs())
                  putInitiSpace(itr, Code::Drop_HubArr);
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

