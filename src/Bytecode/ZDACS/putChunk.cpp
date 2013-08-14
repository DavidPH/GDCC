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
      // Info::putChunk
      //
      void Info::putChunk(std::ostream &out)
      {
         putChunkAIMP(out);
         putChunkAINI(out);
         putChunkARAY(out);
         putChunkASTR(out);
         putChunkATAG(out);
         putChunkFNAM(out);
         putChunkFUNC(out);
         putChunkLOAD(out);
         putChunkMEXP(out);
         putChunkMIMP(out);
         putChunkMINI(out);
         putChunkMSTR(out);
         putChunkSFLG(out);
         putChunkSNAM(out);
         putChunkSPTR(out);
         putChunkSTRL(out);
         putChunkSVCT(out);
      }

      //
      // Info::putChunk
      //
      void Info::putChunk(std::ostream &out, char const *name,
         GDCC::Array<GDCC::String> const &strs, bool junk)
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
         out.write(name, 4);
         putWord(out, off);

         // Write string count.
         if(junk) putWord(out, 0);
         putWord(out, strs.size());
         if(junk) putWord(out, 0);

         // Write string offsets.
         off = base;
         for(auto const &str : strs)
         {
            putWord(out, off);
            off += lenString(str);
         }

         // Write strings.
         for(auto const &str : strs)
            putString(out, str);
      }

      //
      // Info::putChunkAIMP
      //
      void Info::putChunkAIMP(std::ostream &out)
      {
         if(!numChunkAIMP) return;

         GDCC::Array<GDCC::IR::Space const *> imps{numChunkAIMP};

         auto itr = imps.begin();
         for(auto const &sp : GDCC::IR::Space::MapArs)
            if(!sp.second.defin) *itr++ = &sp.second;

         GDCC::FastU size = numChunkAIMP * 8;
         for(auto const &imp : imps)
            size += lenString(imp->glyph);

         out.write("AIMP", 4);
         putWord(out, size);

         for(auto const &imp : imps)
         {
            putWord(out, imp->value);
            putWord(out, imp->words);
            putString(out, imp->glyph);
         }
      }

      //
      // Info::putChunkAINI
      //
      void Info::putChunkAINI(std::ostream &out)
      {
         if(!numChunkAINI) return;

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
         {
            out.write("AINI", 4);
            putWord(out, itr.second.vals.size() * 4 + 4);
            putWord(out, itr.first->value);

            for(auto const &i : itr.second.vals)
               putWord(out, i.val);
         }
      }

      //
      // Info::putChunkARAY
      //
      void Info::putChunkARAY(std::ostream &out)
      {
         if(!numChunkARAY) return;

         out.write("ARAY", 4);
         putWord(out, numChunkARAY * 8);

         for(auto const &itr : GDCC::IR::Space::MapArs)
         {
            auto const &space = itr.second;

            if(!space.defin) continue;

            putWord(out, space.value);
            putWord(out, space.words);
         }
      }

      //
      // Info::putChunkASTR
      //
      void Info::putChunkASTR(std::ostream &out)
      {
         if(!numChunkASTR) return;

         out.write("ASTR", 4);
         putWord(out, numChunkASTR * 4);

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
         {
            if(itr.second.needTag && itr.second.onlyStr)
               putWord(out, itr.first->value);
         }
      }

      //
      // Info::putChunkATAG
      //
      void Info::putChunkATAG(std::ostream &out)
      {
         if(!numChunkATAG) return;

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
         {
            auto const &ini = itr.second;

            if(!ini.needTag || ini.onlyStr) continue;

            out.write("ATAG", 4);
            putWord(out, ini.vals.size() + 5);

            putByte(out, 0); // version
            putWord(out, itr.first->value);

            for(auto const &i : itr.second.vals) switch(i.tag)
            {
            case InitTag::Empty: putByte(out, 0); break;
            case InitTag::Fixed: putByte(out, 0); break;
            case InitTag::Funct: putByte(out, 2); break;
            case InitTag::StrEn: putByte(out, 1); break;
            }
         }
      }

      //
      // Info::putChunkFNAM
      //
      void Info::putChunkFNAM(std::ostream &out)
      {
         if(!numChunkFNAM) return;

         GDCC::Array<GDCC::String> strs{numChunkFNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::LangACS)
               continue;

            strs[func.valueInt] = func.glyph;
         }

         putChunk(out, "FNAM", strs, false);
      }

      //
      // Info::putChunkFUNC
      //
      void Info::putChunkFUNC(std::ostream &out)
      {
         if(!numChunkFUNC) return;

         GDCC::Array<GDCC::IR::Function const *> funcs{numChunkFUNC};

         for(auto &func : funcs) func = nullptr;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::LangACS)
               continue;

            funcs[func.valueInt] = &func;
         }

         out.write("FUNC", 4);
         putWord(out, numChunkFUNC * 8);

         for(auto func : funcs)
         {
            if(func)
            {
               putByte(out, func->param);
               putByte(out, std::max(func->localReg, func->param));
               putByte(out, !!func->retrn);
               putByte(out, 0);

               if(func->defin)
                  putExpWord(out, ResolveGlyph(func->label));
               else
                  putWord(out, 0);
            }
            else
               out.write("\0\0\0\0\0\0\0\0", 8);
         }
      }

      //
      // Info::putChunkLOAD
      //
      void Info::putChunkLOAD(std::ostream &out)
      {
         numChunkLOAD = GDCC::IR::ImportRange().size();

         if(!numChunkLOAD) return;

         GDCC::FastU size = 0;

         for(auto const &imp : GDCC::IR::ImportRange())
            size += lenString(imp.glyph);

         out.write("LOAD", 4);
         putWord(out, size);

         for(auto const &imp : GDCC::IR::ImportRange())
            putString(out, imp.glyph);
      }

      //
      // Info::putChunkMEXP
      //
      void Info::putChunkMEXP(std::ostream &out)
      {
         if(!numChunkMEXP) return;

         GDCC::Array<GDCC::String> strs{numChunkMEXP};
         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &obj : GDCC::IR::Space::MapReg.obset)
            if(obj->defin) strs[obj->value] = obj->glyph;

         for(auto const &itr : GDCC::IR::Space::MapArs)
            if(itr.second.defin) strs[itr.second.value] = itr.second.glyph;

         putChunk(out, "MEXP", strs, false);
      }

      //
      // Info::putChunkMIMP
      //
      void Info::putChunkMIMP(std::ostream &out)
      {
         if(!numChunkMIMP) return;

         GDCC::Array<GDCC::IR::Object const *> imps{numChunkMIMP};

         auto itr = imps.begin();
         for(auto const &obj : GDCC::IR::Space::MapReg.obset)
            if(!obj->defin) *itr++ = obj;

         GDCC::FastU size = numChunkMIMP * 4;
         for(auto const &imp : imps)
            size += lenString(imp->glyph);

         out.write("MIMP", 4);
         putWord(out, size);

         for(auto const &imp : imps)
         {
            putWord(out, imp->value);
            putString(out, imp->glyph);
         }
      }

      //
      // Info::putChunkMINI
      //
      void Info::putChunkMINI(std::ostream &out)
      {
         if(!numChunkMINI) return;

         auto const &ini = init[&GDCC::IR::Space::MapReg];

         for(std::size_t i = 0, e = ini.vals.size(); i != e; ++i) if(ini.vals[i].val)
         {
            out.write("MINI", 4);
            putWord(out, 8);
            putWord(out, i);
            putWord(out, ini.vals[i].val);
         }
      }

      //
      // Info::putChunkMSTR
      //
      void Info::putChunkMSTR(std::ostream &out)
      {
         if(!numChunkMSTR) return;

         out.write("MSTR", 4);
         putWord(out, numChunkMSTR * 4);

         auto const &ini = init[&GDCC::IR::Space::MapReg];

         for(std::size_t i = 0, e = ini.vals.size(); i != e; ++i)
            if(ini.vals[i].tag == InitTag::StrEn) putWord(out, i);
      }

      //
      // Info::putChunkSFLG
      //
      void Info::putChunkSFLG(std::ostream &out)
      {
         if(!numChunkSFLG) return;

         out.write("SFLG", 4);
         putWord(out, numChunkSFLG * 4);

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::Script  &&
               func.ctype != GDCC::IR::CallType::ScriptI &&
               func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!func.defin) continue;

            GDCC::FastU flags = 0;

            if(func.sflagClS) flags |= 0x0002;
            if(func.sflagNet) flags |= 0x0001;

            if(!flags) continue;

            putHWord(out, func.valueInt);
            putHWord(out, flags);
         }
      }

      //
      // Info::putChunkSNAM
      //
      void Info::putChunkSNAM(std::ostream &out)
      {
         if(!numChunkSNAM) return;

         GDCC::Array<GDCC::String> strs{numChunkSNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            strs[func.valueInt] = func.valueStr;
         }

         putChunk(out, "SNAM", strs, false);
      }

      //
      // Info::putChunkSPTR
      //
      void Info::putChunkSPTR(std::ostream &out)
      {
         if(!numChunkSPTR) return;

         out.write("SPTR", 4);
         putWord(out, numChunkSPTR * 12);

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::Script  &&
               func.ctype != GDCC::IR::CallType::ScriptI &&
               func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!func.defin) continue;

            if(func.ctype == GDCC::IR::CallType::ScriptS)
               putHWord(out, -static_cast<GDCC::FastI>(func.valueInt) - 1);
            else
               putHWord(out, func.valueInt);

            switch(func.stype)
            {
            case GDCC::IR::ScriptType::None:       putHWord(out,  0); break;
            case GDCC::IR::ScriptType::Death:      putHWord(out,  3); break;
            case GDCC::IR::ScriptType::Disconnect: putHWord(out, 14); break;
            case GDCC::IR::ScriptType::Enter:      putHWord(out,  4); break;
            case GDCC::IR::ScriptType::Lightning:  putHWord(out, 12); break;
            case GDCC::IR::ScriptType::Open:       putHWord(out,  1); break;
            case GDCC::IR::ScriptType::Respawn:    putHWord(out,  2); break;
            case GDCC::IR::ScriptType::Return:     putHWord(out, 15); break;
            case GDCC::IR::ScriptType::Unloading:  putHWord(out, 13); break;
            }

            putExpWord(out, ResolveGlyph(func.label));
            putWord(out, func.param);
         }
      }

      //
      // Info::putChunkSTRL
      //
      void Info::putChunkSTRL(std::ostream &out)
      {
         if(!numChunkSTRL) return;

         GDCC::Array<GDCC::String> strs{numChunkSTRL};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::StrEntRange()) if(itr.second.defin)
            strs[itr.second.valueInt] = itr.second.valueStr;

         putChunk(out, "STRL", strs, true);
      }

      //
      // Info::putChunkSVCT
      //
      void Info::putChunkSVCT(std::ostream &out)
      {
         if(!numChunkSVCT) return;

         out.write("SVCT", 4);
         putWord(out, numChunkSFLG * 4);

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::Script  &&
               func.ctype != GDCC::IR::CallType::ScriptI &&
               func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!func.defin) continue;

            if(func.localReg <= 20) continue;

            putHWord(out, func.valueInt);
            putHWord(out, func.localReg);
         }
      }
   }
}

// EOF

