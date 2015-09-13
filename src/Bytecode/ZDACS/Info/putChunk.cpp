//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk output.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/CallType.hpp"
#include "IR/Program.hpp"
#include "IR/ScriptType.hpp"


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
            for(auto const &sp : prog->rangeSpaceMapArs())
               if(!sp.defin) *itr++ = &sp;

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
               if(itr.first->space.base == IR::AddrBase::MapArr)
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
               if(itr.first->space.base == IR::AddrBase::MapArr)
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
               if(itr.first->space.base == IR::AddrBase::MapArr)
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

            Core::Array<IR::Function const *> funcs{numChunkFUNC};

            for(auto &f : funcs) f = nullptr;

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
                     throw Core::ExceptStr(f->getOrigin(), "too many registers");

                  putByte(param);
                  putByte(localReg);
                  putByte(!!f->retrn);
                  putByte(0);

                  if(f->defin)
                     putWord(GetWord(resolveGlyph(f->label)));
                  else
                     putWord(0);
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
               putWord(j ? GetWord(resolveGlyph(j->label)) : 0);
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

            for(auto const &itr : prog->rangeObject())
            {
               if(itr.defin && itr.space.base == IR::AddrBase::MapReg)
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

            Core::Array<IR::Object const *> imps{numChunkMIMP};

            {
               auto imp = imps.begin();
               for(auto const &itr : prog->rangeObject())
               {
                  if(!itr.defin && itr.space.base == IR::AddrBase::MapReg)
                     *imp++ = &itr;
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

            for(auto const &itr : init[&prog->getSpaceMapReg()].vals) if(itr.second.val)
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

            for(auto const &itr : init[&prog->getSpaceMapReg()].vals)
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

               if(itr.sflagClS) flags |= 0x0002;
               if(itr.sflagNet) flags |= 0x0001;

               if(!flags) continue;

               putHWord(GetScriptValue(itr));
               putHWord(flags);
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
               if(!IsScriptS(itr.ctype))
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
               if(!IsScript(itr.ctype))
                  continue;

               if(!itr.defin) continue;

               Core::FastU stype;

               // Convert script type.
               switch(itr.stype)
               {
               default:
               case IR::ScriptType::None:        stype =  0; break;
               case IR::ScriptType::BlueReturn:  stype =  6; break;
               case IR::ScriptType::Death:       stype =  3; break;
               case IR::ScriptType::Disconnect:  stype = 14; break;
               case IR::ScriptType::Enter:       stype =  4; break;
               case IR::ScriptType::Event:       stype = 16; break;
               case IR::ScriptType::Lightning:   stype = 12; break;
               case IR::ScriptType::Open:        stype =  1; break;
               case IR::ScriptType::Pickup:      stype =  5; break;
               case IR::ScriptType::RedReturn:   stype =  7; break;
               case IR::ScriptType::Respawn:     stype =  2; break;
               case IR::ScriptType::Return:      stype = 15; break;
               case IR::ScriptType::Unloading:   stype = 13; break;
               case IR::ScriptType::WhiteReturn: stype =  8; break;
               }

               auto paramMax = GetParamMax(itr.ctype);
               auto param    = itr.param < paramMax ? itr.param : paramMax;

               // Write entry.
               if(UseFakeACS0)
               {
                  putHWord(GetScriptValue(itr));
                  putByte(stype);
                  putByte(param);
                  putWord(GetWord(resolveGlyph(itr.label)));
               }
               else
               {
                  putHWord(GetScriptValue(itr));
                  putHWord(stype);
                  putWord(GetWord(resolveGlyph(itr.label)));
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
               }
               else
               {
                  putHWord(InitScriptNumber);
                  putHWord(1);
                  putWord(codeInit);
                  putWord(0);
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
                  throw Core::ExceptStr(itr.getOrigin(), "too many registers");

               putHWord(GetScriptValue(itr));
               putHWord(itr.getLocalReg());
            }
         }
      }
   }
}

// EOF

