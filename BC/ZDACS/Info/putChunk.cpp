//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk output.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Module.hpp"

#include "Core/Exception.hpp"

#include "IR/Program.hpp"

#include "Target/CallType.hpp"
#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::putChunk
   //
   void Info::putChunk()
   {
      // Not a real ACSE chunk, but it should be.
      putChunkCODE();

      putChunkAIMP();
      putChunkAINI();
      putChunkARAY();
      putChunkASTR();
      putChunkATAG();
      putChunkFARY();
      putChunkFNAM();
      putChunkFUNC();
      putChunkJUMP();
      putChunkLOAD();
      putChunkMEXP();
      putChunkMIMP();
      putChunkMINI();
      putChunkMSTR();
      putChunkSARY();
      putChunkSFLG();
      putChunkSNAM();
      putChunkSPTR();
      putChunkSTRL();
      putChunkSVCT();
   }

   //
   // Info::putChunk
   //
   void Info::putChunk(char const *name, Core::Array<Core::String> const &strs, bool junk)
   {
      std::size_t base;
      std::size_t off;

      // Calculate base offset.
      base = strs.size() * 4 + 4;
      if(junk) base += 8;

      // Calculate size of chunk.
      off = base;
      for(auto const &s : strs)
         off += lenString(s);

      // Write chunk header.
      putData(name, 4);
      putWord(off);

      // Write string count.
      if(junk) putWord(0);
      putWord(strs.size());
      if(junk) putWord(0);

      // Write string offsets.
      off = base;
      for(auto const &s : strs)
      {
         putWord(off);
         off += lenString(s);
      }

      // Write strings.
      if(junk && UseChunkSTRE)
      {
         off = base;
         for(auto const &s : strs)
         {
            putString(s, off * 157135);
            off += lenString(s);
         }
      }
      else for(auto const &s : strs)
         putString(s);
   }

   //
   // Info::putChunkAIMP
   //
   void Info::putChunkAIMP()
   {
      auto elemC = module->chunkAIMP.size();
      if(!elemC) return;

      Core::FastU len = elemC * 8 + 4;
      for(auto const &elem : module->chunkAIMP)
         len += lenString(elem.glyph);

      putData("AIMP", 4);
      putWord(len);
      putWord(elemC);

      for(auto const &elem : module->chunkAIMP)
      {
         putWord(elem.value);
         putWord(elem.words);
         putString(elem.glyph);
      }
   }

   //
   // Info::putChunkAINI
   //
   void Info::putChunkAINI()
   {
      for(auto const &elem : module->chunkAINI)
      {
         putData("AINI", 4);
         putWord(4 + elem.inits.size() * 4);
         putWord(elem.value);

         for(auto const &ini : elem.inits)
            putWord(getWord(ini));
      }
   }

   //
   // Info::putChunkARAY
   //
   void Info::putChunkARAY()
   {
      auto elemC = module->chunkARAY.size();
      if(!elemC) return;

      putData("ARAY", 4);
      putWord(elemC * 8);

      for(auto const &elem : module->chunkARAY)
      {
         putWord(elem.value);
         putWord(elem.words);
      }
   }

   //
   // Info::putChunkASTR
   //
   void Info::putChunkASTR()
   {
      auto elemC = module->chunkASTR.size();
      if(!elemC) return;

      putData("ASTR", 4);
      putWord(elemC * 4);

      for(auto const &elem : module->chunkASTR)
         putWord(elem.value);
   }

   //
   // Info::putChunkATAG
   //
   void Info::putChunkATAG()
   {
      for(auto const &elem : module->chunkATAG)
      {
         putData("ATAG", 4);
         putWord(elem.tags.size() + 5);

         putByte(0); // version
         putWord(elem.value);

         for(auto const &tag : elem.tags)
            putByte(tag);
      }
   }

   //
   // Info::putChunkCODE
   //
   void Info::putChunkCODE()
   {
      putData("\0\0\0\0", 4);
      putWord(module->chunkCODE.getPos());

      putCode();
   }

   //
   // Info::putChunkFARY
   //
   void Info::putChunkFARY()
   {
      for(auto const &elem : module->chunkFARY)
      {
         putData("FARY", 4);
         putWord(elem.sizes.size() * 4 + 2);

         putHWord(elem.value);

         for(auto const &size : elem.sizes)
            putWord(size);
      }
   }

   //
   // Info::putChunkFNAM
   //
   void Info::putChunkFNAM()
   {
      if(module->chunkFNAM.empty()) return;

      putChunk("FNAM", {module->chunkFNAM.begin(), module->chunkFNAM.end(),
         [](ElemFNAM const &elem) {return elem.glyph;}}, false);
   }

   //
   // Info::putChunkFUNC
   //
   void Info::putChunkFUNC()
   {
      auto elemC = module->chunkFUNC.size();
      if(!elemC) return;

      putData("FUNC", 4);
      putWord(elemC * 8);

      for(auto const &elem : module->chunkFUNC)
      {
         putByte(elem.param);
         putByte(elem.local);
         putByte(!!elem.retrn);
         putByte(0);
         putWord(getWord(elem.entry));
      }
   }

   //
   // Info::putChunkJUMP
   //
   void Info::putChunkJUMP()
   {
      auto elemC = module->chunkJUMP.size();
      if(!elemC) return;

      putData("JUMP", 4);
      putWord(elemC * 4);

      for(auto const &elem : module->chunkJUMP)
         putWord(getWord(elem.entry));
   }

   //
   // Info::putChunkLOAD
   //
   void Info::putChunkLOAD()
   {
      auto elemC = module->chunkLOAD.size();
      if(!elemC) return;

      Core::FastU len = 0;
      for(auto const &elem : module->chunkLOAD)
         len += lenString(elem.glyph);

      putData("LOAD", 4);
      putWord(len);

      for(auto const &elem : module->chunkLOAD)
         putString(elem.glyph);
   }

   //
   // Info::putChunkMEXP
   //
   void Info::putChunkMEXP()
   {
      if(module->chunkMEXP.empty()) return;

      putChunk("MEXP", {module->chunkMEXP.begin(), module->chunkMEXP.end(),
         [](ElemMEXP const &elem) {return elem.glyph;}}, false);
   }

   //
   // Info::putChunkMIMP
   //
   void Info::putChunkMIMP()
   {
      auto elemC = module->chunkMIMP.size();
      if(!elemC) return;

      Core::FastU len = elemC * 4;
      for(auto const &elem : module->chunkMIMP)
         len += lenString(elem.glyph);

      putData("MIMP", 4);
      putWord(len);

      for(auto const &elem : module->chunkMIMP)
      {
         putWord(elem.value);
         putString(elem.glyph);
      }
   }

   //
   // Info::putChunkMINI
   //
   void Info::putChunkMINI()
   {
      for(auto const &elem : module->chunkMINI)
      {
         putData("MINI", 4);
         putWord(4 + elem.inits.size() * 4);
         putWord(elem.value);
         for(auto const &ini : elem.inits)
            putWord(getWord(ini));
      }
   }

   //
   // Info::putChunkMSTR
   //
   void Info::putChunkMSTR()
   {
      auto elemC = module->chunkMSTR.size();
      if(!elemC) return;

      putData("MSTR", 4);
      putWord(elemC * 4);

      for(auto const &elem : module->chunkMSTR)
         putWord(elem.value);
   }

   //
   // Info::putChunkSARY
   //
   void Info::putChunkSARY()
   {
      for(auto const &elem : module->chunkSARY)
      {
         putData("FARY", 4);
         putWord(elem.sizes.size() * 4 + 2);

         putHWord(elem.value);

         for(auto const &size : elem.sizes)
            putWord(size);
      }
   }

   //
   // Info::putChunkSFLG
   //
   void Info::putChunkSFLG()
   {
      auto elemC = module->chunkSFLG.size();
      if(!elemC) return;

      putData("SFLG", 4);
      putWord(elemC * 4);

      for(auto const &elem : module->chunkSFLG)
      {
         putHWord(elem.value);
         putHWord(elem.flags);
      }
   }

   //
   // Info::putChunkSNAM
   //
   void Info::putChunkSNAM()
   {
      if(module->chunkSNAM.empty()) return;

      putChunk("SNAM", {module->chunkSNAM.begin(), module->chunkSNAM.end(),
         [](ElemSNAM const &elem) {return elem.glyph;}}, false);
   }

   //
   // Info::putChunkSPTR
   //
   void Info::putChunkSPTR()
   {
      auto elemC = module->chunkSPTR.size();
      if(!elemC) return;

      putData("SPTR", 4);
      putWord(elemC * (UseFakeACS0 ? 8 : 12));

      for(auto const &elem : module->chunkSPTR)
      {
         if(UseFakeACS0)
         {
            putHWord(elem.value);
            putByte(elem.stype);
            putByte(elem.param);
            putWord(getWord(elem.entry));
         }
         else
         {
            putHWord(elem.value);
            putHWord(elem.stype);
            putWord(getWord(elem.entry));
            putWord(elem.param);
         }
      }
   }

   //
   // Info::putChunkSTRL
   //
   void Info::putChunkSTRL()
   {
      if(module->chunkSTRL.empty()) return;

      putChunk(UseChunkSTRE ? "STRE" : "STRL",
         {module->chunkSTRL.begin(), module->chunkSTRL.end(),
            [](ElemSTRL const &elem) {return elem.value;}}, true);
   }

   //
   // Info::putChunkSVCT
   //
   void Info::putChunkSVCT()
   {
      auto elemC = module->chunkSVCT.size();
      if(!elemC) return;

      putData("SVCT", 4);
      putWord(elemC * 4);

      for(auto const &elem : module->chunkSVCT)
      {
         putHWord(elem.value);
         putHWord(elem.local);
      }
   }
}

// EOF

