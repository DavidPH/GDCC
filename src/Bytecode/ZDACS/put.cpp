//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Glyph.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::put
      //
      void Info::put(std::ostream &out)
      {
         // Put header.
         out.write("ACSE", 4);
         putWord(out, jumpPos);
         out.write("GDCC::BC", 8);

         // Put statements.
         for(auto &func : GDCC::IR::FunctionRange()) try
         {
            curFunc = &func.second;
            for(auto const &stmnt : func.second.block)
               putStatement(out, stmnt);
            curFunc = nullptr;
         }
         catch(...) {curFunc = nullptr; throw;}

         // Put chunks.
         putChunk(out);
      }

      //
      // Info::putChunk
      //
      void Info::putChunk(std::ostream &out)
      {
         putChunkSFLG(out);
         putChunkSPTR(out);
         putChunkSVCT(out);
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

      //
      // Info::putExpWord
      //
      void Info::putExpWord(std::ostream &out, GDCC::IR::Exp const *exp)
      {
         putWord(out, ResolveValue(exp->getValue()));
      }

      //
      // Info::putHWord
      //
      void Info::putHWord(std::ostream &out, GDCC::FastU i)
      {
         out.put((i >>  0) & 0xFF);
         out.put((i >>  8) & 0xFF);
      }

      //
      // Info::putStatement
      //
      void Info::putStatement(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            putWord(out, 0);
            break;

         case GDCC::IR::Code::Casm:
            for(auto const &arg : stmnt.args)
               putExpWord(out, arg.aLit.value);
            break;

         case GDCC::IR::Code::Move_W:
            putStatement_Move_W(out, stmnt);
            break;

         case GDCC::IR::Code::Retn:
            putStatement_Retn(out, stmnt);
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": cannot put Code for ZDACS: "
               << stmnt.code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStatement_Move_W
      //
      void Info::putStatement_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         // push_?
         if(stmnt.args[0].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[1].a)
         {
         case GDCC::IR::ArgBase::GblReg:
            putWord(out, 182); // pushglobalvar
            putExpWord(out, stmnt.args[1].aGblReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::Lit:
            putWord(out, 3); // pushnumber
            putExpWord(out, stmnt.args[1].aLit.value);
            break;

         case GDCC::IR::ArgBase::LocReg:
            putWord(out, 28); // pushscriptvar
            putExpWord(out, stmnt.args[1].aLocReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::MapReg:
            putWord(out, 29); // pushmapvar
            putExpWord(out, stmnt.args[1].aMapReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::WldReg:
            putWord(out, 30); // pushworldvar
            putExpWord(out, stmnt.args[1].aWldReg.idx->aLit.value);
            break;

         default:
            std::cerr << "bad Code::Move_W(Stk, ?)\n";
            throw EXIT_FAILURE;
         }

         // drop_?
         else if(stmnt.args[1].a == GDCC::IR::ArgBase::Stk) switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::GblReg:
            putWord(out, 181); // assignglobalvar
            putExpWord(out, stmnt.args[0].aGblReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::LocReg:
            putWord(out, 25); // assignscriptvar
            putExpWord(out, stmnt.args[0].aLocReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::MapReg:
            putWord(out, 26); // assignmapvar
            putExpWord(out, stmnt.args[0].aMapReg.idx->aLit.value);
            break;

         case GDCC::IR::ArgBase::Nul:
            putWord(out, 54); // drop
            break;

         case GDCC::IR::ArgBase::WldReg:
            putWord(out, 27); // assignworldvar
            putExpWord(out, stmnt.args[0].aWldReg.idx->aLit.value);
            break;

         default:
            std::cerr << "bad Code::Move_W(?, Stk)\n";
            throw EXIT_FAILURE;
         }

         // ???
         else
         {
            std::cerr << "bad Code::Move_W\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putStatement_Retn
      //
      void Info::putStatement_Retn(std::ostream &out, GDCC::IR::Statement const &stmnt)
      {
         switch(curFunc->ctype)
         {
         case GDCC::IR::CallType::Script:
         case GDCC::IR::CallType::ScriptI:
         case GDCC::IR::CallType::ScriptS:
            if(stmnt.args.size() == 0)
            {
               putWord(out, 1); // terminate
            }
            else if(stmnt.args.size() == 1)
            {
               putWord(out, 257); // setresultvalue
               putWord(out,   1); // terminate
            }
            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Code::Retn\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::putWord
      //
      void Info::putWord(std::ostream &out, GDCC::FastU i)
      {
         out.put((i >>  0) & 0xFF);
         out.put((i >>  8) & 0xFF);
         out.put((i >> 16) & 0xFF);
         out.put((i >> 24) & 0xFF);
      }
   }
}

// EOF

