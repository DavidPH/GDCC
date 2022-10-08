//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2022 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk output.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

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
      if(!numChunkAIMP) return;

      Core::Array<IR::Space const *> imps{numChunkAIMP};

      auto itr = imps.begin();
      for(auto const &sp : prog->rangeSpaceModArs())
         if(!sp.defin && spaceUsed[&sp]) *itr++ = &sp;

      Core::FastU size = numChunkAIMP * 8 + 4;
      for(auto const &imp : imps)
         size += lenString(imp->glyph);

      putData("AIMP", 4);
      putWord(size);
      putWord(numChunkAIMP);

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
         if(itr.first->space.base == IR::AddrBase::ModArr &&
            itr.first->defin && !itr.second.onlyNil)
      {
         putData("AINI", 4);
         putWord(itr.second.max * 4 + 4);
         putWord(itr.first->value);

         for(Core::FastU i = 0, e = itr.second.max; i != e; ++i)
         {
            auto val = itr.second.vals.find(i);
            if(val != itr.second.vals.end())
               putWord(val->second.val);
            else
               putWord(0);
         }
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

      for(auto const &itr : prog->rangeSpaceModArs())
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
         if(itr.first->space.base == IR::AddrBase::ModArr && itr.first->defin)
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
         if(itr.first->space.base == IR::AddrBase::ModArr && itr.first->defin)
      {
         if(!itr.second.needTag || itr.second.onlyStr) continue;

         putData("ATAG", 4);
         putWord(itr.second.max + 5);

         putByte(0); // version
         putWord(itr.first->value);

         for(Core::FastU i = 0, e = itr.second.max; i != e; ++i)
         {
            auto val = itr.second.vals.find(i);
            if(val != itr.second.vals.end()) switch(val->second.tag)
            {
            case InitTag::Empty: putByte(0); break;
            case InitTag::Fixed: putByte(0); break;
            case InitTag::Funct: putByte(2); break;
            case InitTag::StrEn: putByte(1); break;
            }
            else
               putByte(0);
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
      for(auto &itr : prog->rangeFunction())
         putFunc(itr);

      // Put initializers.
      putIniti();
   }

   //
   // Info::putChunkFARY
   //
   void Info::putChunkFARY()
   {
      for(auto &itr : prog->rangeFunction())
      {
         if(itr.ctype != IR::CallType::StdCall &&
            itr.ctype != IR::CallType::StkCall)
            continue;

         if(itr.localArr.empty()) continue;

         putData("FARY", 4);
         putWord(itr.localArr.size() * 4 + 2);

         putHWord(itr.valueInt);

         for(Core::FastU arr = 0; arr != itr.localArr.size(); ++arr)
               putWord(itr.localArr[arr]);
      }
   }

   //
   // Info::putChunkFNAM
   //
   void Info::putChunkFNAM()
   {
      if(!numChunkFNAM) return;

      Core::Array<Core::String> strs{numChunkFNAM};

      for(auto &s : strs) s = Core::STR_;

      for(auto const &itr : prog->rangeFunction())
      {
         if(itr.ctype != IR::CallType::StdCall &&
            itr.ctype != IR::CallType::StkCall)
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

      Core::Array<IR::Function const *> funcs{numChunkFUNC, nullptr};

      for(auto const &itr : prog->rangeFunction())
      {
         if(itr.ctype != IR::CallType::StdCall &&
            itr.ctype != IR::CallType::StkCall)
            continue;

         funcs[itr.valueInt] = &itr;
      }

      putData("FUNC", 4);
      putWord(numChunkFUNC * 8);

      for(auto f : funcs)
      {
         if(f)
         {
            auto paramMax = GetParamMax(f->ctype);
            auto param    = f->param < paramMax ? f->param : paramMax;

            auto localReg = std::max(f->getLocalReg(), f->param);

            if(localReg > 255)
               Core::Error(f->getOrigin(), "too many registers");

            putByte(param);
            putByte(localReg);
            putByte(!!f->retrn);
            putByte(0);

            if(f->defin)
            {
               putWord(getWord(resolveGlyph(f->label)));
            }
            else
            {
               // Must have imports to import from.
               if(!prog->sizeImport())
                  Core::ErrorUndef("Function", f->glyph);

               putWord(0);
            }
         }
         else
            putData("\0\0\0\0\0\0\0\0", 8);
      }
   }

   //
   // Info::putChunkJUMP
   //
   void Info::putChunkJUMP()
   {
      if(!numChunkJUMP) return;

      Core::Array<IR::DJump const *> jumps{numChunkJUMP, nullptr};

      for(auto const &itr : prog->rangeDJump())
         jumps[itr.value] = &itr;

      putData("JUMP", 4);
      putWord(numChunkJUMP * 4);

      for(auto j : jumps)
         putWord(j ? getWord(resolveGlyph(j->label)) : 0);
   }

   //
   // Info::putChunkLOAD
   //
   void Info::putChunkLOAD()
   {
      numChunkLOAD = prog->sizeImport();

      if(!numChunkLOAD) return;

      Core::FastU size = 0;

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

      Core::Array<Core::String> strs{numChunkMEXP};
      for(auto &s : strs) s = Core::STR_;

      for(auto const &itr : prog->rangeObjectBySpace({IR::AddrBase::ModReg, Core::STR_}))
      {
         if(itr->defin)
            strs[itr->value] = itr->glyph;
      }

      for(auto const &itr : prog->rangeSpaceModArs())
         if(itr.defin) strs[itr.value] = itr.glyph;

      putChunk("MEXP", strs, false);
   }

   //
   // Info::putChunkMIMP
   //
   void Info::putChunkMIMP()
   {
      if(!numChunkMIMP) return;

      Core::Array<IR::Object const *> imps{numChunkMIMP};

      {
         auto imp = imps.begin();
         for(auto const &itr : prog->rangeObjectBySpace({IR::AddrBase::ModReg, Core::STR_}))
         {
            if(!itr->defin)
               *imp++ = itr;
         }
      }

      Core::FastU size = numChunkMIMP * 4;
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

      for(auto const &itr : init[&prog->getSpaceModReg()].vals) if(itr.second.val)
      {
         putData("MINI", 4);
         putWord(8);
         putWord(itr.first);
         putWord(itr.second.val);
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

      for(auto const &itr : init[&prog->getSpaceModReg()].vals)
         if(itr.second.tag == InitTag::StrEn) putWord(itr.first);
   }

   //
   // Info::putChunkSARY
   //
   void Info::putChunkSARY()
   {
      for(auto &itr : prog->rangeFunction())
      {
         if(!IsScript(itr.ctype))
            continue;

         if(itr.localArr.empty()) continue;

         putData("SARY", 4);
         putWord(itr.localArr.size() * 4 + 2);

         putHWord(GetScriptValue(itr));

         for(Core::FastU arr = 0; arr != itr.localArr.size(); ++arr)
               putWord(itr.localArr[arr]);
      }
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
         if(!IsScript(itr.ctype))
            continue;

         if(!itr.defin) continue;

         Core::FastU flags = 0;

         // Convert script flag.
         for(auto const &st : itr.stype)
         {
            if(auto flag = ScriptFlags.find(st))
               flags |= *flag;

            else switch(st)
            {
            case Core::STR_clientside: flags |= 0x0002; break;
            case Core::STR_net:        flags |= 0x0001; break;
            default: break;
            }
         }

         if(!flags) continue;

         putHWord(GetScriptValue(itr));
         putHWord(flags);
      }

      if(codeInit && Target::EngineCur == Target::Engine::Zandronum)
      {
         putHWord(InitScriptNumber + 1);
         putHWord(0x0002);
      }
   }

   //
   // Info::putChunkSNAM
   //
   void Info::putChunkSNAM()
   {
      if(!numChunkSNAM) return;

      Core::Array<Core::String> strs{numChunkSNAM};

      for(auto &s : strs) s = Core::STR_;

      for(auto const &itr : prog->rangeFunction())
      {
         if(!itr.defin || !IsScriptS(itr.ctype))
            continue;

         strs[itr.valueInt] = itr.valueStr;
      }

      if(codeInit && InitScriptNamed)
      {
         *(strs.end() - 1) = InitScriptName;

         if(Target::EngineCur == Target::Engine::Zandronum)
            *(strs.end() - 2) = InitScriptName + "_ClS";
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
         if(!IsScript(itr.ctype))
            continue;

         if(!itr.defin) continue;

         Core::FastU stype = 0;

         // Convert script type.
         for(auto const &st : itr.stype)
         {
            if(auto type = ScriptTypes.find(st))
               stype = *type;

            else switch(st)
            {
            case Core::STR_bluereturn:  stype =  6; break;
            case Core::STR_death:       stype =  3; break;
            case Core::STR_disconnect:  stype = 14; break;
            case Core::STR_enter:       stype =  4; break;
            case Core::STR_event:       stype = 16; break;
            case Core::STR_kill:        stype = 17; break;
            case Core::STR_lightning:   stype = 12; break;
            case Core::STR_open:        stype =  1; break;
            case Core::STR_pickup:      stype =  5; break;
            case Core::STR_redreturn:   stype =  7; break;
            case Core::STR_reopen:      stype = 18; break;
            case Core::STR_respawn:     stype =  2; break;
            case Core::STR_return:      stype = 15; break;
            case Core::STR_unloading:   stype = 13; break;
            case Core::STR_whitereturn: stype =  8; break;
            default: break;
            }
         }

         auto paramMax = GetParamMax(itr.ctype);
         auto param    = itr.param < paramMax ? itr.param : paramMax;

         // Write entry.
         if(UseFakeACS0)
         {
            putHWord(GetScriptValue(itr));
            putByte(stype);
            putByte(param);
            putWord(getWord(resolveGlyph(itr.label)));
         }
         else
         {
            putHWord(GetScriptValue(itr));
            putHWord(stype);
            putWord(getWord(resolveGlyph(itr.label)));
            putWord(param);
         }
      }

      // Initializer script.
      if(codeInit)
      {
         if(UseFakeACS0)
         {
            putHWord(InitScriptNumber);
            putByte(1);
            putByte(0);
            putWord(codeInit);

            if(Target::EngineCur == Target::Engine::Zandronum)
            {
               putHWord(InitScriptNumber + 1);
               putByte(1);
               putByte(0);
               putWord(codeInit);
            }
         }
         else
         {
            putHWord(InitScriptNumber);
            putHWord(1);
            putWord(codeInit);
            putWord(0);

            if(Target::EngineCur == Target::Engine::Zandronum)
            {
               putHWord(InitScriptNumber + 1);
               putHWord(1);
               putWord(codeInit);
               putWord(0);
            }
         }
      }
   }

   //
   // Info::putChunkSTRL
   //
   void Info::putChunkSTRL()
   {
      if(!numChunkSTRL) return;

      Core::Array<Core::String> strs{numChunkSTRL};

      for(auto &s : strs) s = Core::STR_;

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
      putWord(numChunkSVCT * 4);

      for(auto const &itr : prog->rangeFunction())
      {
         if(!IsScript(itr.ctype))
            continue;

         if(!itr.defin) continue;

         if(itr.getLocalReg() <= 20) continue;

         if(itr.getLocalReg() > 65535)
            Core::Error(itr.getOrigin(), "too many registers");

         putHWord(GetScriptValue(itr));
         putHWord(itr.getLocalReg());
      }
   }
}

// EOF

