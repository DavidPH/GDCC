//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk length calculation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Program.hpp"

#include "Platform/Platform.hpp"


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
         // Info::lenChunk
         //
         Core::FastU Info::lenChunk()
         {
            Core::FastU len = 0;

            len += lenChunkCODE();

            len += lenChunkAIMP();
            len += lenChunkAINI();
            len += lenChunkARAY();
            len += lenChunkASTR();
            len += lenChunkATAG();
            len += lenChunkFARY();
            len += lenChunkFNAM();
            len += lenChunkFUNC();
            len += lenChunkJUMP();
            len += lenChunkLOAD();
            len += lenChunkMEXP();
            len += lenChunkMIMP();
            len += lenChunkMINI();
            len += lenChunkMSTR();
            len += lenChunkSARY();
            len += lenChunkSFLG();
            len += lenChunkSNAM();
            len += lenChunkSPTR();
            len += lenChunkSTRL();
            len += lenChunkSVCT();

            return len;
         }

         //
         // Info::lenChunk
         //
         Core::FastU Info::lenChunk(char const *,
            Core::Array<Core::String> const &strs, bool junk)
         {
            Core::FastU len = 8;

            // Table header.
            len += strs.size() * 4 + 4;
            if(junk) len += 8;

            // Calculate size of chunk.
            for(auto const &s : strs)
               len += lenString(s);

            return len;
         }

         //
         // Info::lenChunkAIMP
         //
         Core::FastU Info::lenChunkAIMP()
         {
            if(!numChunkAIMP) return 0;

            Core::FastU len = numChunkAIMP * 8 + 12;

            for(auto const &itr : prog->rangeSpaceModArs())
               if(!itr.defin) len += lenString(itr.glyph);

            return len;
         }

         //
         // Info::lenChunkAINI
         //
         Core::FastU Info::lenChunkAINI()
         {
            if(!numChunkAINI) return 0;

            Core::FastU len = 0;

            for(auto const &itr : init)
               if(itr.first->space.base == IR::AddrBase::ModArr)
                  len += itr.second.max * 4 + 12;

            return len;
         }

         //
         // Info::lenChunkARAY
         //
         Core::FastU Info::lenChunkARAY()
         {
            if(!numChunkARAY) return 0;

            return numChunkARAY * 8 + 8;
         }

         //
         // Info::lenChunkASTR
         //
         Core::FastU Info::lenChunkASTR()
         {
            if(!numChunkASTR) return 0;

            return numChunkASTR * 4 + 8;
         }

         //
         // Info::lenChunkATAG
         //
         Core::FastU Info::lenChunkATAG()
         {
            if(!numChunkATAG) return 0;

            Core::FastU len = 0;

            for(auto const &itr : init)
               if(itr.first->space.base == IR::AddrBase::ModArr)
            {
               if(itr.second.needTag && !itr.second.onlyStr)
                  len += itr.second.max + 13;
            }

            return len;
         }

         //
         // Info::lenChunkCODE
         //
         Core::FastU Info::lenChunkCODE()
         {
            return numChunkCODE + 8;
         }

         //
         // Info::lenChunkFARY
         //
         Core::FastU Info::lenChunkFARY()
         {
            Core::FastU numChunk = 0;
            Core::FastU numArray = 0;

            for(auto const &itr : prog->rangeFunction())
            {
               if(itr.ctype != IR::CallType::StdCall &&
                  itr.ctype != IR::CallType::StkCall)
                  continue;

               if(itr.localArr.empty()) continue;

               ++numChunk;
               numArray += itr.localArr.size();
            }

            return numChunk * 10 + numArray * 4;
         }

         //
         // Info::lenChunkFNAM
         //
         Core::FastU Info::lenChunkFNAM()
         {
            if(!numChunkFNAM) return 0;

            Core::Array<Core::String> strs{numChunkFNAM};

            for(auto &s : strs) s = Core::STR_;

            for(auto const &itr : prog->rangeFunction())
            {
               if(itr.ctype != IR::CallType::StdCall &&
                  itr.ctype != IR::CallType::StkCall)
                  continue;

               strs[itr.valueInt] = itr.glyph;
            }

            return lenChunk("FNAM", strs, false);
         }

         //
         // Info::lenChunkFUNC
         //
         Core::FastU Info::lenChunkFUNC()
         {
            if(!numChunkFUNC) return 0;

            return numChunkFUNC * 8 + 8;
         }

         //
         // Info::lenChunkJUMP
         //
         Core::FastU Info::lenChunkJUMP()
         {
            if(!numChunkJUMP) return 0;

            return numChunkJUMP * 4 + 8;
         }

         //
         // Info::lenChunkLOAD
         //
         Core::FastU Info::lenChunkLOAD()
         {
            numChunkLOAD = prog->sizeImport();

            if(!numChunkLOAD) return 0;

            Core::FastU len = 8;

            for(auto const &itr : prog->rangeImport())
               len += lenString(itr.glyph);

            return len;
         }

         //
         // Info::lenChunkMEXP
         //
         Core::FastU Info::lenChunkMEXP()
         {
            if(!numChunkMEXP) return 0;

            Core::Array<Core::String> strs{numChunkMEXP};
            for(auto &s : strs) s = Core::STR_;

            for(auto const &itr : prog->rangeObjectBySpace({IR::AddrBase::ModReg, Core::STR_}))
            {
               if(itr->defin)
                  strs[itr->value] = itr->glyph;
            }

            for(auto const &itr : prog->rangeSpaceModArs())
               if(itr.defin) strs[itr.value] = itr.glyph;

            return lenChunk("MEXP", strs, false);
         }

         //
         // Info::lenChunkMIMP
         //
         Core::FastU Info::lenChunkMIMP()
         {
            if(!numChunkMIMP) return 0;

            Core::FastU len = numChunkMIMP * 4 + 8;

            for(auto const &itr : prog->rangeObjectBySpace({IR::AddrBase::ModReg, Core::STR_}))
            {
               if(!itr->defin)
                  len += lenString(itr->glyph);
            }

            return len;
         }

         //
         // Info::lenChunkMINI
         //
         Core::FastU Info::lenChunkMINI()
         {
            if(!numChunkMINI) return 0;

            return numChunkMINI * 16;
         }

         //
         // Info::lenChunkMSTR
         //
         Core::FastU Info::lenChunkMSTR()
         {
            if(!numChunkMSTR) return 0;

            return numChunkMSTR * 4 + 8;
         }

         //
         // Info::lenChunkSARY
         //
         Core::FastU Info::lenChunkSARY()
         {
            Core::FastU numChunk = 0;
            Core::FastU numArray = 0;

            for(auto const &itr : prog->rangeFunction())
            {
               if(!IsScriptS(itr.ctype))
                  continue;

               if(itr.localArr.empty()) continue;

               ++numChunk;
               numArray += itr.localArr.size();
            }

            return numChunk * 10 + numArray * 4;
         }

         //
         // Info::lenChunkSFLG
         //
         Core::FastU Info::lenChunkSFLG()
         {
            if(!numChunkSFLG) return 0;

            return numChunkSFLG * 4 + 8;
         }

         //
         // Info::lenChunkSNAM
         //
         Core::FastU Info::lenChunkSNAM()
         {
            if(!numChunkSNAM) return 0;

            Core::Array<Core::String> strs{numChunkSNAM};

            for(auto &s : strs) s = Core::STR_;

            for(auto const &itr : prog->rangeFunction())
            {
               if(!IsScriptS(itr.ctype))
                  continue;

               strs[itr.valueInt] = itr.valueStr;
            }

            if(codeInit && InitScriptNamed)
            {
               *(strs.end() - 1) = InitScriptName;

               if(Platform::TargetCur == Platform::Target::Zandronum)
                  *(strs.end() - 2) = InitScriptName + "_ClS";
            }

            return lenChunk("SNAM", strs, false);
         }

         //
         // Info::lenChunkSPTR
         //
         Core::FastU Info::lenChunkSPTR()
         {
            if(!numChunkSPTR) return 0;

            return numChunkSPTR * (UseFakeACS0 ? 8 : 12) + 8;
         }

         //
         // Info::lenChunkSTRL
         //
         Core::FastU Info::lenChunkSTRL()
         {
            if(!numChunkSTRL) return 0;

            Core::Array<Core::String> strs{numChunkSTRL};

            for(auto &s : strs) s = Core::STR_;

            for(auto const &itr : prog->rangeStrEnt()) if(itr.defin)
               strs[itr.valueInt] = itr.valueStr;

            return lenChunk("STRL", strs, true);
         }

         //
         // Info::lenChunkSVCT
         //
         Core::FastU Info::lenChunkSVCT()
         {
            if(!numChunkSVCT) return 0;

            return numChunkSVCT * 4 + 8;
         }
      }
   }
}

// EOF

