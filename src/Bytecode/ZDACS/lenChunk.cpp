//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk length calculation.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Import.hpp"
#include "GDCC/IR/Object.hpp"
#include "GDCC/IR/StrEnt.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::lenChunk
      //
      GDCC::FastU Info::lenChunk()
      {
         GDCC::FastU len = 0;

         len += lenChunkCODE();

         len += lenChunkAIMP();
         len += lenChunkAINI();
         len += lenChunkARAY();
         len += lenChunkASTR();
         len += lenChunkATAG();
         len += lenChunkFNAM();
         len += lenChunkFUNC();
         len += lenChunkLOAD();
         len += lenChunkMEXP();
         len += lenChunkMIMP();
         len += lenChunkMINI();
         len += lenChunkMSTR();
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
      GDCC::FastU Info::lenChunk(char const *, GDCC::Array<GDCC::String> const &strs, bool junk)
      {
         GDCC::FastU len = 8;

         // Table header.
         len += strs.size() * 4 + 4;
         if(junk) len += 8;

         // Calculate size of chunk.
         for(auto const &str : strs)
            len += lenString(str);

         return len;
      }

      //
      // Info::lenChunkAIMP
      //
      GDCC::FastU Info::lenChunkAIMP()
      {
         if(!numChunkAIMP) return 0;

         GDCC::FastU len = numChunkAIMP * 8 + 8;

         for(auto const &sp : GDCC::IR::Space::MapArs)
            if(!sp.second.defin) len += lenString(sp.second.glyph);

         return len;
      }

      //
      // Info::lenChunkAINI
      //
      GDCC::FastU Info::lenChunkAINI()
      {
         if(!numChunkAINI) return 0;

         GDCC::FastU len = 0;

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
            len += itr.second.vals.size() * 4 + 12;

         return len;
      }

      //
      // Info::lenChunkARAY
      //
      GDCC::FastU Info::lenChunkARAY()
      {
         if(!numChunkARAY) return 0;

         return numChunkARAY * 8 + 8;
      }

      //
      // Info::lenChunkASTR
      //
      GDCC::FastU Info::lenChunkASTR()
      {
         if(!numChunkASTR) return 0;

         return numChunkASTR * 4 + 8;
      }

      //
      // Info::lenChunkATAG
      //
      GDCC::FastU Info::lenChunkATAG()
      {
         if(!numChunkATAG) return 0;

         GDCC::FastU len = 0;

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
         {
            if(itr.second.needTag && !itr.second.onlyStr)
               len += itr.second.vals.size() + 13;
         }

         return len;
      }

      //
      // Info::lenChunkCODE
      //
      GDCC::FastU Info::lenChunkCODE()
      {
         return numChunkCODE + 8;
      }

      //
      // Info::lenChunkFNAM
      //
      GDCC::FastU Info::lenChunkFNAM()
      {
         if(!numChunkFNAM) return 0;

         GDCC::Array<GDCC::String> strs{numChunkFNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::LangACS)
               continue;

            strs[func.valueInt] = func.glyph;
         }

         return lenChunk("FNAM", strs, false);
      }

      //
      // Info::lenChunkFUNC
      //
      GDCC::FastU Info::lenChunkFUNC()
      {
         if(!numChunkFUNC) return 0;

         return numChunkFUNC * 8 + 8;
      }

      //
      // Info::lenChunkLOAD
      //
      GDCC::FastU Info::lenChunkLOAD()
      {
         numChunkLOAD = GDCC::IR::ImportRange().size();

         if(!numChunkLOAD) return 0;

         GDCC::FastU len = 8;

         for(auto const &imp : GDCC::IR::ImportRange())
            len += lenString(imp.glyph);

         return len;
      }

      //
      // Info::lenChunkMEXP
      //
      GDCC::FastU Info::lenChunkMEXP()
      {
         if(!numChunkMEXP) return 0;

         GDCC::Array<GDCC::String> strs{numChunkMEXP};
         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &obj : GDCC::IR::Space::MapReg.obset)
            if(obj->defin) strs[obj->value] = obj->glyph;

         for(auto const &itr : GDCC::IR::Space::MapArs)
            if(itr.second.defin) strs[itr.second.value] = itr.second.glyph;

         return lenChunk("MEXP", strs, false);
      }

      //
      // Info::lenChunkMIMP
      //
      GDCC::FastU Info::lenChunkMIMP()
      {
         if(!numChunkMIMP) return 0;

         GDCC::FastU len = numChunkMIMP * 4 + 8;

         for(auto const &obj : GDCC::IR::Space::MapReg.obset)
            if(!obj->defin) len += lenString(obj->glyph);

         return len;
      }

      //
      // Info::lenChunkMINI
      //
      GDCC::FastU Info::lenChunkMINI()
      {
         if(!numChunkMINI) return 0;

         return numChunkMINI * 16;
      }

      //
      // Info::lenChunkMSTR
      //
      GDCC::FastU Info::lenChunkMSTR()
      {
         if(!numChunkMSTR) return 0;

         return numChunkMSTR * 4 + 8;
      }

      //
      // Info::lenChunkSFLG
      //
      GDCC::FastU Info::lenChunkSFLG()
      {
         if(!numChunkSFLG) return 0;

         return numChunkSFLG * 4 + 8;
      }

      //
      // Info::lenChunkSNAM
      //
      GDCC::FastU Info::lenChunkSNAM()
      {
         if(!numChunkSNAM) return 0;

         GDCC::Array<GDCC::String> strs{numChunkSNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            strs[func.valueInt] = func.valueStr;
         }

         return lenChunk("SNAM", strs, false);
      }

      //
      // Info::lenChunkSPTR
      //
      GDCC::FastU Info::lenChunkSPTR()
      {
         if(!numChunkSPTR) return 0;

         return numChunkSPTR * (UseFakeACS0 ? 8 : 12) + 8;
      }

      //
      // Info::lenChunkSTRL
      //
      GDCC::FastU Info::lenChunkSTRL()
      {
         if(!numChunkSTRL) return 0;

         GDCC::Array<GDCC::String> strs{numChunkSTRL};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::StrEntRange()) if(itr.second.defin)
            strs[itr.second.valueInt] = itr.second.valueStr;

         return lenChunk("STRL", strs, true);
      }

      //
      // Info::lenChunkSVCT
      //
      GDCC::FastU Info::lenChunkSVCT()
      {
         if(!numChunkSVCT) return 0;

         return numChunkSFLG * 4 + 8;
      }
   }
}

// EOF

