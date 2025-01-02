//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk length calculation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Module.hpp"

#include "IR/Program.hpp"

#include "Target/CallType.hpp"
#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
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
      auto elemC = module->chunkAIMP.size();
      if(!elemC) return 0;

      Core::FastU len = elemC * 8 + 4;
      for(auto const &elem : module->chunkAIMP)
         len += lenString(elem.glyph);

      return len;
   }

   //
   // Info::lenChunkAINI
   //
   Core::FastU Info::lenChunkAINI()
   {
      Core::FastU len = 0;
      for(auto const &elem : module->chunkAINI)
         len += 12 + elem.inits.size() * 4;

      return len;
   }

   //
   // Info::lenChunkARAY
   //
   Core::FastU Info::lenChunkARAY()
   {
      auto elemC = module->chunkARAY.size();
      if(!elemC) return 0;

      return 8 + elemC * 8;
   }

   //
   // Info::lenChunkASTR
   //
   Core::FastU Info::lenChunkASTR()
   {
      auto elemC = module->chunkASTR.size();
      if(!elemC) return 0;

      return 8 + elemC * 4;
   }

   //
   // Info::lenChunkATAG
   //
   Core::FastU Info::lenChunkATAG()
   {
      Core::FastU len = 0;
      for(auto const &elem : module->chunkATAG)
         len += 13 + elem.tags.size();

      return len;
   }

   //
   // Info::lenChunkCODE
   //
   Core::FastU Info::lenChunkCODE()
   {
      return 8 + module->chunkCODE.getPos();
   }

   //
   // Info::lenChunkFARY
   //
   Core::FastU Info::lenChunkFARY()
   {
      Core::FastU len = 0;
      for(auto const &elem : module->chunkFARY)
         len += 10 + elem.sizes.size() * 4;

      return len;
   }

   //
   // Info::lenChunkFNAM
   //
   Core::FastU Info::lenChunkFNAM()
   {
      if(module->chunkFNAM.empty()) return 0;

      return lenChunk("FNAM", {module->chunkFNAM.begin(), module->chunkFNAM.end(),
         [](ElemFNAM const &elem) {return elem.glyph;}}, false);
   }

   //
   // Info::lenChunkFUNC
   //
   Core::FastU Info::lenChunkFUNC()
   {
      auto elemC = module->chunkFUNC.size();
      if(!elemC) return 0;

      return 8 + elemC * 8;
   }

   //
   // Info::lenChunkJUMP
   //
   Core::FastU Info::lenChunkJUMP()
   {
      auto elemC = module->chunkJUMP.size();
      if(!elemC) return 0;

      return 8 + elemC * 4;
   }

   //
   // Info::lenChunkLOAD
   //
   Core::FastU Info::lenChunkLOAD()
   {
      auto elemC = module->chunkLOAD.size();
      if(!elemC) return 0;

      Core::FastU len = 8;
      for(auto const &elem : module->chunkLOAD)
         len += lenString(elem.glyph);

      return len;
   }

   //
   // Info::lenChunkMEXP
   //
   Core::FastU Info::lenChunkMEXP()
   {
      if(module->chunkMEXP.empty()) return 0;

      return lenChunk("MEXP", {module->chunkMEXP.begin(), module->chunkMEXP.end(),
         [](ElemMEXP const &elem) {return elem.glyph;}}, false);
   }

   //
   // Info::lenChunkMIMP
   //
   Core::FastU Info::lenChunkMIMP()
   {
      auto elemC = module->chunkMIMP.size();
      if(!elemC) return 0;

      Core::FastU len = 8 + elemC * 4;
      for(auto const &elem : module->chunkMIMP)
         len += lenString(elem.glyph);

      return len;
   }

   //
   // Info::lenChunkMINI
   //
   Core::FastU Info::lenChunkMINI()
   {
      Core::FastU len = 0;
      for(auto const &elem : module->chunkMINI)
         len += 12 + elem.inits.size() * 4;

      return len;
   }

   //
   // Info::lenChunkMSTR
   //
   Core::FastU Info::lenChunkMSTR()
   {
      auto elemC = module->chunkMSTR.size();
      if(!elemC) return 0;

      return 8 + elemC * 4;
   }

   //
   // Info::lenChunkSARY
   //
   Core::FastU Info::lenChunkSARY()
   {
      Core::FastU len = 0;
      for(auto const &elem : module->chunkSARY)
         len += 10 + elem.sizes.size() * 4;

      return len;
   }

   //
   // Info::lenChunkSFLG
   //
   Core::FastU Info::lenChunkSFLG()
   {
      auto elemC = module->chunkSFLG.size();
      if(!elemC) return 0;

      return 8 + elemC * 4;
   }

   //
   // Info::lenChunkSNAM
   //
   Core::FastU Info::lenChunkSNAM()
   {
      if(module->chunkSNAM.empty()) return 0;

      return lenChunk("SNAM", {module->chunkSNAM.begin(), module->chunkSNAM.end(),
         [](ElemSNAM const &elem) {return elem.glyph;}}, false);
   }

   //
   // Info::lenChunkSPTR
   //
   Core::FastU Info::lenChunkSPTR()
   {
      auto elemC = module->chunkSPTR.size();
      if(!elemC) return 0;

      return 8 + elemC * (UseFakeACS0 ? 8 : 12);
   }

   //
   // Info::lenChunkSTRL
   //
   Core::FastU Info::lenChunkSTRL()
   {
      if(module->chunkSTRL.empty()) return 0;

      return lenChunk(UseChunkSTRE ? "STRE" : "STRL", 
         {module->chunkSTRL.begin(), module->chunkSTRL.end(),
            [](ElemSTRL const &elem) {return elem.value;}}, true);
   }

   //
   // Info::lenChunkSVCT
   //
   Core::FastU Info::lenChunkSVCT()
   {
      auto elemC = module->chunkSVCT.size();
      if(!elemC) return 0;

      return 8 + elemC * 4;
   }
}

// EOF

