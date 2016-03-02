//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "IR/Program.hpp"


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
               // Check if already initialized.
               putCode(Code::Push_Lit);
               putWord(getInitHubIndex());
               putCode(Code::Push_HubArr);
               putWord(getInitHubArray());
               putCode(Code::Jcnd_Tru);
               putWord(codeInitEnd);

               // Write instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceHubArs())
                  putInitiSpace(itr, Code::Drop_HubArr);
            }

            if(isGblArr)
            {
               // Check if already initialized.
               putCode(Code::Push_Lit);
               putWord(getInitGblIndex());
               putCode(Code::Push_GblArr);
               putWord(getInitGblArray());
               putCode(Code::Jcnd_Tru);
               putWord(codeInitEnd);

               // Write instructions needed for initializers.
               for(auto &itr : prog->rangeSpaceGblArs())
                  putInitiSpace(itr, Code::Drop_GblArr);
               putInitiSpace(prog->getSpaceSta(), Code::Drop_GblArr);
            }

            // Set initialized flag(s) after a delay to allow other
            // initialization scripts to run on first tic.
            if(InitDelay)
            {
               putCode(Code::Wait_Lit, 1);
            }

            if(isHubArr)
            {
               // Set initialized flag.
               putCode(Code::Push_Lit);
               putWord(getInitHubIndex());
               putCode(Code::Push_Lit);
               putWord(1);
               putCode(Code::Drop_HubArr);
               putWord(getInitHubArray());
            }

            if(isGblArr)
            {
               // Set initialized flag.
               putCode(Code::Push_Lit);
               putWord(getInitGblIndex());
               putCode(Code::Push_Lit);
               putWord(1);
               putCode(Code::Drop_GblArr);
               putWord(getInitGblArray());
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

