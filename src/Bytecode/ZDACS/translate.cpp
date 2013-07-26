//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code translation.
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
      // Info::translateFunction
      //
      void Info::translateFunction(GDCC::IR::Function &func)
      {
         // Generate label glyph.
         {
            auto type = GDCC::IR::Type_Fixed(32, 0, false, false);
            auto val = GDCC::IR::ExpCreate_ValueRoot(
               GDCC::IR::Value_Fixed(jumpPos, type), GDCC::Origin(GDCC::STRNULL, 0));

            auto &data = GDCC::IR::Glyph::GetData(func.label);

            data.type  = type;
            data.value = val;
         }

         InfoBase::translateFunction(func);

         if(!func.exdef) switch(func.ctype)
         {
         case GDCC::IR::CallType::Script:
         case GDCC::IR::CallType::ScriptI:
         case GDCC::IR::CallType::ScriptS:
            if(func.sflagClS || func.sflagNet) ++numChunkSFLG;
            ++numChunkSPTR;
            if(func.localReg > 20) ++numChunkSVCT;
            break;

         default: break;
         }
      }

      //
      // Info::translateStatement
      //
      void Info::translateStatement(GDCC::IR::Statement &stmnt)
      {
         // Generate label glyphs.
         if(!stmnt.labs.empty())
         {
            auto type = GDCC::IR::Type_Fixed(32, 0, false, false);
            auto val = GDCC::IR::ExpCreate_ValueRoot(
               GDCC::IR::Value_Fixed(jumpPos, type), stmnt.pos);

            for(auto const &lab : stmnt.labs)
            {
               auto &data = GDCC::IR::Glyph::GetData(lab);

               data.type  = type;
               data.value = val;
            }
         }

         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            jumpPos += 4;
            break;

         case GDCC::IR::Code::Casm:
            CheckArgC(stmnt, 1);
            for(auto n = stmnt.args.size(); --n;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Lit);
            jumpPos += stmnt.args.size() * 4;
            break;

         case GDCC::IR::Code::Move_W:
            translateStatement_Move_W(stmnt);
            break;

         case GDCC::IR::Code::Retn:
            translateStatement_Retn(stmnt);
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": cannot translate Code for ZDACS: "
               << stmnt.code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::translateStatement_Move_W
      //
      void Info::translateStatement_Move_W(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArg(stmnt.args[0], stmnt.pos);
         CheckArg(stmnt.args[1], stmnt.pos);

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::GblReg:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: jumpPos += 8; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::Nul:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: jumpPos += 4; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::Stk:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::GblReg: jumpPos += 8; break;
            case GDCC::IR::ArgBase::Lit:    jumpPos += 8; break;
            case GDCC::IR::ArgBase::LocReg: jumpPos += 8; break;
            case GDCC::IR::ArgBase::MapReg: jumpPos += 8; break;
            case GDCC::IR::ArgBase::WldReg: jumpPos += 8; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::LocReg:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: jumpPos += 8; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::MapReg:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: jumpPos += 8; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::WldReg:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: jumpPos += 8; break;
            default: goto badcase;
            }
            break;

         default:
         badcase:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Code::Move_W("
               << stmnt.args[0].a << ',' << stmnt.args[1].a << ")\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::translateStatement_Retn
      //
      void Info::translateStatement_Retn(GDCC::IR::Statement &stmnt)
      {
         for(auto n = stmnt.args.size(); n--;)
            CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);

         switch(curFunc->ctype)
         {
         case GDCC::IR::CallType::Script:
         case GDCC::IR::CallType::ScriptI:
         case GDCC::IR::CallType::ScriptS:
            if(stmnt.args.size() == 0)
               jumpPos += 4;
            else if(stmnt.args.size() == 1)
               jumpPos += 8;
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
   }
}

// EOF

