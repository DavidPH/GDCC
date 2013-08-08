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
#include "GDCC/IR/Object.hpp"


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
         putChunkARAY(out);
         putChunkFNAM(out);
         putChunkFUNC(out);
         putChunkSFLG(out);
         putChunkSPTR(out);
         putChunkSVCT(out);
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

            if(space.exdef) continue;

            putWord(out, space.value);
            putWord(out, space.words);
         }
      }

      //
      // Info::putChunkFNAM
      //
      void Info::putChunkFNAM(std::ostream &out)
      {
         if(numChunkFNAM == 1) return;

         GDCC::Array<GDCC::String> strarr{numChunkFNAM};
         GDCC::Array<GDCC::FastU>  stroff{numChunkFNAM};
         auto stritr = strarr.begin();
         auto offitr = stroff.begin();
         std::size_t off = numChunkFNAM * 4 + 4;

         *stritr++ = GDCC::STR_;
         *offitr++ = off; off += 1;
         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::LangACS)
               continue;

            *stritr++ = func.glyph;
            *offitr++ = off; off += func.glyph.getData().len + 1;
         }

         out.write("FNAM", 4);
         putWord(out, off);

         putWord(out, numChunkFNAM);

         for(auto const &i : stroff)
            putWord(out, i);

         for(auto const &s : strarr)
         {
            auto const &data = s.getData();
            out.write(data.str, data.len + 1);
         }
      }

      //
      // Info::putChunkFUNC
      //
      void Info::putChunkFUNC(std::ostream &out)
      {
         if(numChunkFUNC == 1) return;

         out.write("FUNC", 4);
         putWord(out, numChunkFUNC * 8);

         // Write dummy function 0.
         out.write("\0\0\0\0\0\0\0\0", 8);

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::LangACS)
               continue;

            putByte(out, func.param);
            putByte(out, std::max(func.localReg, func.param));
            putByte(out, !!func.retrn);
            putByte(out, 0);

            if(func.exdef)
               putWord(out, 0);
            else
               putExpWord(out, ResolveGlyph(func.label));
         }
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

            if(func.exdef) continue;

            GDCC::FastU flags = 0;

            if(func.sflagClS) flags |= 0x0002;
            if(func.sflagNet) flags |= 0x0001;

            if(!flags) continue;

            putHWord(out, func.valueInt);
            putHWord(out, flags);
         }
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

            if(func.exdef) continue;

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

            if(func.exdef) continue;

            if(func.localReg <= 20) continue;

            putHWord(out, func.valueInt);
            putHWord(out, func.localReg);
         }
      }
   }
}

// EOF

