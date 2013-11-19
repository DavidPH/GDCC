//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk output.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Program.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
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
         putChunkFNAM();
         putChunkFUNC();
         putChunkLOAD();
         putChunkMEXP();
         putChunkMIMP();
         putChunkMINI();
         putChunkMSTR();
         putChunkSFLG();
         putChunkSNAM();
         putChunkSPTR();
         putChunkSTRL();
         putChunkSVCT();
      }

      //
      // Info::putChunk
      //
      void Info::putChunk(char const *name, GDCC::Array<GDCC::String> const &strs, bool junk)
      {
         std::size_t base;
         std::size_t off;

         // Calculate base offset.
         base = strs.size() * 4 + 4;
         if(junk) base += 8;

         // Calculate size of chunk.
         off = base;
         for(auto const &str : strs)
            off += lenString(str);

         // Write chunk header.
         putData(name, 4);
         putWord(off);

         // Write string count.
         if(junk) putWord(0);
         putWord(strs.size());
         if(junk) putWord(0);

         // Write string offsets.
         off = base;
         for(auto const &str : strs)
         {
            putWord(off);
            off += lenString(str);
         }

         // Write strings.
         if(junk && UseChunkSTRE)
         {
            off = base;
            for(auto const &str : strs)
            {
               putString(str, off * 157135);
               off += lenString(str);
            }
         }
         else for(auto const &str : strs)
            putString(str);
      }

      //
      // Info::putChunkAIMP
      //
      void Info::putChunkAIMP()
      {
         if(!numChunkAIMP) return;

         GDCC::Array<GDCC::IR::Space const *> imps{numChunkAIMP};

         auto itr = imps.begin();
         for(auto const &sp : prog->rangeSpaceMapArs())
            if(!sp.defin) *itr++ = &sp;

         GDCC::FastU size = numChunkAIMP * 8;
         for(auto const &imp : imps)
            size += lenString(imp->glyph);

         putData("AIMP", 4);
         putWord(size);

         for(auto const &imp : imps)
         {
            putWord(imp->value);
            putWord(imp->words);
            putString(imp->glyph);
         }
      }

      //
      // Info::putChunkAINI
      //
      void Info::putChunkAINI()
      {
         if(!numChunkAINI) return;

         for(auto const &itr : init)
            if(itr.first->space.base == GDCC::IR::AddrBase::MapArr)
         {
            putData("AINI", 4);
            putWord(itr.second.vals.size() * 4 + 4);
            putWord(itr.first->value);

            for(auto const &i : itr.second.vals)
               putWord(i.val);
         }
      }

      //
      // Info::putChunkARAY
      //
      void Info::putChunkARAY()
      {
         if(!numChunkARAY) return;

         putData("ARAY", 4);
         putWord(numChunkARAY * 8);

         for(auto const &itr : prog->rangeSpaceMapArs())
         {
            if(!itr.defin) continue;

            putWord(itr.value);
            putWord(itr.words);
         }
      }

      //
      // Info::putChunkASTR
      //
      void Info::putChunkASTR()
      {
         if(!numChunkASTR) return;

         putData("ASTR", 4);
         putWord(numChunkASTR * 4);

         for(auto const &itr : init)
            if(itr.first->space.base == GDCC::IR::AddrBase::MapArr)
         {
            if(itr.second.needTag && itr.second.onlyStr)
               putWord(itr.first->value);
         }
      }

      //
      // Info::putChunkATAG
      //
      void Info::putChunkATAG()
      {
         if(!numChunkATAG) return;

         for(auto const &itr : init)
            if(itr.first->space.base == GDCC::IR::AddrBase::MapArr)
         {
            auto const &ini = itr.second;

            if(!ini.needTag || ini.onlyStr) continue;

            putData("ATAG", 4);
            putWord(ini.vals.size() + 5);

            putByte(0); // version
            putWord(itr.first->value);

            for(auto const &i : itr.second.vals) switch(i.tag)
            {
            case InitTag::Empty: putByte(0); break;
            case InitTag::Fixed: putByte(0); break;
            case InitTag::Funct: putByte(2); break;
            case InitTag::StrEn: putByte(1); break;
            }
         }
      }

      //
      // Info::putChunkCODE
      //
      void Info::putChunkCODE()
      {
         putData("\0\0\0\0", 4);
         putWord(numChunkCODE);

         // Put statements.
         for(auto &itr : prog->rangeFunction()) try
         {
            func = &itr;
            putBlock(func->block);
            func = nullptr;
         }
         catch(...) {func = nullptr; throw;}
      }

      //
      // Info::putChunkFNAM
      //
      void Info::putChunkFNAM()
      {
         if(!numChunkFNAM) return;

         GDCC::Array<GDCC::String> strs{numChunkFNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : prog->rangeFunction())
         {
            if(itr.ctype != GDCC::IR::CallType::LangACS)
               continue;

            strs[itr.valueInt] = itr.glyph;
         }

         putChunk("FNAM", strs, false);
      }

      //
      // Info::putChunkFUNC
      //
      void Info::putChunkFUNC()
      {
         if(!numChunkFUNC) return;

         GDCC::Array<GDCC::IR::Function const *> funcs{numChunkFUNC};

         for(auto &func : funcs) func = nullptr;

         for(auto const &itr : prog->rangeFunction())
         {
            if(itr.ctype != GDCC::IR::CallType::LangACS)
               continue;

            funcs[itr.valueInt] = &itr;
         }

         putData("FUNC", 4);
         putWord(numChunkFUNC * 8);

         for(auto func : funcs)
         {
            if(func)
            {
               putByte(func->param);
               putByte(std::max(func->localReg, func->param));
               putByte(!!func->retrn);
               putByte(0);

               if(func->defin)
                  putExpWord(resolveGlyph(func->label));
               else
                  putWord(0);
            }
            else
               putData("\0\0\0\0\0\0\0\0", 8);
         }
      }

      //
      // Info::putChunkLOAD
      //
      void Info::putChunkLOAD()
      {
         numChunkLOAD = prog->sizeImport();

         if(!numChunkLOAD) return;

         GDCC::FastU size = 0;

         for(auto const &itr : prog->rangeImport())
            size += lenString(itr.glyph);

         putData("LOAD", 4);
         putWord(size);

         for(auto const &itr : prog->rangeImport())
            putString(itr.glyph);
      }

      //
      // Info::putChunkMEXP
      //
      void Info::putChunkMEXP()
      {
         if(!numChunkMEXP) return;

         GDCC::Array<GDCC::String> strs{numChunkMEXP};
         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : prog->rangeObject())
         {
            if(!itr.defin && itr.space.base == GDCC::IR::AddrBase::MapReg)
               strs[itr.value] = itr.glyph;
         }

         for(auto const &itr : prog->rangeSpaceMapArs())
            if(itr.defin) strs[itr.value] = itr.glyph;

         putChunk("MEXP", strs, false);
      }

      //
      // Info::putChunkMIMP
      //
      void Info::putChunkMIMP()
      {
         if(!numChunkMIMP) return;

         GDCC::Array<GDCC::IR::Object const *> imps{numChunkMIMP};

         auto itr = imps.begin();
         for(auto const &obj : prog->rangeObject())
         {
            if(!obj.defin && obj.space.base == GDCC::IR::AddrBase::MapReg)
               *itr++ = &obj;
         }

         GDCC::FastU size = numChunkMIMP * 4;
         for(auto const &imp : imps)
            size += lenString(imp->glyph);

         putData("MIMP", 4);
         putWord(size);

         for(auto const &imp : imps)
         {
            putWord(imp->value);
            putString(imp->glyph);
         }
      }

      //
      // Info::putChunkMINI
      //
      void Info::putChunkMINI()
      {
         if(!numChunkMINI) return;

         auto const &ini = init[&prog->getSpaceMapReg()];

         for(std::size_t i = 0, e = ini.vals.size(); i != e; ++i) if(ini.vals[i].val)
         {
            putData("MINI", 4);
            putWord(8);
            putWord(i);
            putWord(ini.vals[i].val);
         }
      }

      //
      // Info::putChunkMSTR
      //
      void Info::putChunkMSTR()
      {
         if(!numChunkMSTR) return;

         putData("MSTR", 4);
         putWord(numChunkMSTR * 4);

         auto const &ini = init[&prog->getSpaceMapReg()];

         for(std::size_t i = 0, e = ini.vals.size(); i != e; ++i)
            if(ini.vals[i].tag == InitTag::StrEn) putWord(i);
      }

      //
      // Info::putChunkSFLG
      //
      void Info::putChunkSFLG()
      {
         if(!numChunkSFLG) return;

         putData("SFLG", 4);
         putWord(numChunkSFLG * 4);

         for(auto const &itr : prog->rangeFunction())
         {
            if(itr.ctype != GDCC::IR::CallType::Script  &&
               itr.ctype != GDCC::IR::CallType::ScriptI &&
               itr.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!itr.defin) continue;

            GDCC::FastU flags = 0;

            if(itr.sflagClS) flags |= 0x0002;
            if(itr.sflagNet) flags |= 0x0001;

            if(!flags) continue;

            putHWord(itr.valueInt);
            putHWord(flags);
         }
      }

      //
      // Info::putChunkSNAM
      //
      void Info::putChunkSNAM()
      {
         if(!numChunkSNAM) return;

         GDCC::Array<GDCC::String> strs{numChunkSNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : prog->rangeFunction())
         {
            if(itr.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            strs[itr.valueInt] = itr.valueStr;
         }

         putChunk("SNAM", strs, false);
      }

      //
      // Info::putChunkSPTR
      //
      void Info::putChunkSPTR()
      {
         if(!numChunkSPTR) return;

         putData("SPTR", 4);
         putWord(numChunkSPTR * (UseFakeACS0 ? 8 : 12));

         for(auto const &itr : prog->rangeFunction())
         {
            if(itr.ctype != GDCC::IR::CallType::Script  &&
               itr.ctype != GDCC::IR::CallType::ScriptI &&
               itr.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!itr.defin) continue;

            GDCC::FastU stype, value;

            // Convert script type.
            switch(itr.stype)
            {
            case GDCC::IR::ScriptType::None:       stype =  0; break;
            case GDCC::IR::ScriptType::Death:      stype =  3; break;
            case GDCC::IR::ScriptType::Disconnect: stype = 14; break;
            case GDCC::IR::ScriptType::Enter:      stype =  4; break;
            case GDCC::IR::ScriptType::Lightning:  stype = 12; break;
            case GDCC::IR::ScriptType::Open:       stype =  1; break;
            case GDCC::IR::ScriptType::Respawn:    stype =  2; break;
            case GDCC::IR::ScriptType::Return:     stype = 15; break;
            case GDCC::IR::ScriptType::Unloading:  stype = 13; break;
            }

            // Convert script index.
            if(itr.ctype == GDCC::IR::CallType::ScriptS)
               value = -static_cast<GDCC::FastI>(itr.valueInt) - 1;
            else
               value = itr.valueInt;

            // Write entry.
            if(UseFakeACS0)
            {
               putHWord(value);
               putByte(stype);
               putByte(itr.param);
               putExpWord(resolveGlyph(itr.label));
            }
            else
            {
               putHWord(value);
               putHWord(stype);
               putExpWord(resolveGlyph(itr.label));
               putWord(itr.param);
            }
         }
      }

      //
      // Info::putChunkSTRL
      //
      void Info::putChunkSTRL()
      {
         if(!numChunkSTRL) return;

         GDCC::Array<GDCC::String> strs{numChunkSTRL};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : prog->rangeStrEnt()) if(itr.defin)
            strs[itr.valueInt] = itr.valueStr;

         putChunk(UseChunkSTRE ? "STRE" : "STRL", strs, true);
      }

      //
      // Info::putChunkSVCT
      //
      void Info::putChunkSVCT()
      {
         if(!numChunkSVCT) return;

         putData("SVCT", 4);
         putWord(numChunkSFLG * 4);

         for(auto const &itr : prog->rangeFunction())
         {
            if(itr.ctype != GDCC::IR::CallType::Script  &&
               itr.ctype != GDCC::IR::CallType::ScriptI &&
               itr.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!itr.defin) continue;

            if(itr.localReg <= 20) continue;

            putHWord(itr.valueInt);
            putHWord(itr.localReg);
         }
      }
   }
}

// EOF

