//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement glyph generation.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::genStmnt
      //
      void Info::genStmnt()
      {
         // Generate label glyphs.
         if(!stmnt->labs.empty())
         {
            auto val = GDCC::IR::ExpCreate_ValueRoot(
               GDCC::IR::Value_Fixed(CodeBase() + numChunkCODE, TypeWord), stmnt->pos);

            for(auto const &lab : stmnt->labs)
            {
               auto &data = prog->getGlyphData(lab);

               data.type  = TypeWord;
               data.value = val;
            }
         }

         switch(stmnt->code)
         {
         case GDCC::IR::Code::Nop:
            numChunkCODE += 4;
            break;

         case GDCC::IR::Code::AddI_W:
         case GDCC::IR::Code::AddU_W:
         case GDCC::IR::Code::AndU_W:
         case GDCC::IR::Code::DivI_W:
         case GDCC::IR::Code::DivX_W:
         case GDCC::IR::Code::ModI_W:
         case GDCC::IR::Code::MulI_W:
         case GDCC::IR::Code::MulX_W:
         case GDCC::IR::Code::OrIU_W:
         case GDCC::IR::Code::OrXU_W:
         case GDCC::IR::Code::ShLU_W:
         case GDCC::IR::Code::ShRI_W:
         case GDCC::IR::Code::SubI_W:
         case GDCC::IR::Code::SubU_W:
            numChunkCODE += 4;
            break;

         case GDCC::IR::Code::Call:
            genStmnt_Call();
            break;

         case GDCC::IR::Code::Casm:
            numChunkCODE += stmnt->args.size() * 4;
            break;

         case GDCC::IR::Code::CmpI_EQ_W:
         case GDCC::IR::Code::CmpI_GE_W:
         case GDCC::IR::Code::CmpI_GT_W:
         case GDCC::IR::Code::CmpI_LE_W:
         case GDCC::IR::Code::CmpI_LT_W:
         case GDCC::IR::Code::CmpI_NE_W:
         case GDCC::IR::Code::CmpU_EQ_W:
         case GDCC::IR::Code::CmpU_NE_W:
            numChunkCODE += 4;
            break;

         case GDCC::IR::Code::Cnat:
            numChunkCODE += 12;
            break;

         case GDCC::IR::Code::Cspe:
            genStmnt_Cspe();
            break;

         case GDCC::IR::Code::InvU_W:
         case GDCC::IR::Code::NegI_W:
         case GDCC::IR::Code::NotU_W:
            numChunkCODE += 4;
            break;

         case GDCC::IR::Code::Move_W:
            genStmnt_Move_W();
            break;

         case GDCC::IR::Code::Retn:
            genStmnt_Retn();
            break;

         case GDCC::IR::Code::Swap_W:
            numChunkCODE += 4;
            break;

         default:
            std::cerr << "ERROR: " << stmnt->pos << ": cannot gen Code for ZDACS: "
               << stmnt->code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::genStmnt_Call
      //
      void Info::genStmnt_Call()
      {
         auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

         switch(stmnt->args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            if(ret == 0 || ret == 1)
               numChunkCODE += 8;
            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }

            break;

         case GDCC::IR::ArgBase::Stk:
            if(ret == 0)
               numChunkCODE += 8;

            else if(ret == 1)
               numChunkCODE += 4;

            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }

            break;

         default:
            std::cerr << "ERROR: " << stmnt->pos << ": bad Call\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::genStmnt_Cspe
      //
      void Info::genStmnt_Cspe()
      {
         auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

         // No call args.
         if(stmnt->args.size() == 2)
         {
            numChunkCODE += ret ? 48 : 12;
            return;
         }

         switch(stmnt->args[2].a)
         {
         case GDCC::IR::ArgBase::Lit:
            numChunkCODE += 8 + (stmnt->args.size() - 2) * (ret ? 8 : 4);
            break;

         case GDCC::IR::ArgBase::Stk:
            numChunkCODE += 8;

            // Dummy args.
            if(ret) numChunkCODE += (7 - stmnt->args.size()) * 8;

            break;

         default:
            std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::genStmnt_Move_W
      //
      void Info::genStmnt_Move_W()
      {
         switch(stmnt->args[0].a)
         {
         case GDCC::IR::ArgBase::GblArr:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               genStmnt_Move_W__Arr_Stk(stmnt->args[0].aGblArr);
               break;

            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::GblReg:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: numChunkCODE += 8; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::LocReg:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: numChunkCODE += 8; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::MapArr:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               genStmnt_Move_W__Arr_Stk(stmnt->args[0].aMapArr);
               break;

            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::MapReg:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: numChunkCODE += 8; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::Nul:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: numChunkCODE += 4; break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::Stk:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::GblReg: numChunkCODE += 8; break;
            case GDCC::IR::ArgBase::LocReg: numChunkCODE += 8; break;
            case GDCC::IR::ArgBase::MapReg: numChunkCODE += 8; break;
            case GDCC::IR::ArgBase::WldReg: numChunkCODE += 8; break;

            case GDCC::IR::ArgBase::Lit:
               genStmnt_Move_W__Stk_Lit(stmnt->args[1].aLit.value);
               break;

            case GDCC::IR::ArgBase::GblArr:
               genStmnt_Move_W__Stk_Arr(stmnt->args[1].aGblArr);
               break;

            case GDCC::IR::ArgBase::MapArr:
               genStmnt_Move_W__Stk_Arr(stmnt->args[1].aMapArr);
               break;

            case GDCC::IR::ArgBase::WldArr:
               genStmnt_Move_W__Stk_Arr(stmnt->args[1].aWldArr);
               break;

            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::WldArr:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               genStmnt_Move_W__Arr_Stk(stmnt->args[0].aWldArr);
               break;

            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::WldReg:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: numChunkCODE += 8; break;
            default: goto badcase;
            }
            break;

         default:
         badcase:
            std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Move_W("
               << stmnt->args[0].a << ',' << stmnt->args[1].a << ")\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::genStmnt_Move_W__Arr_Stk
      //
      void Info::genStmnt_Move_W__Arr_Stk(GDCC::IR::ArgPtr2 const &arr)
      {
         numChunkCODE += IsExp0(arr.off) ? 12 : 24;
      }

      //
      // Info::genStmnt_Move_W__Stk_Arr
      //
      void Info::genStmnt_Move_W__Stk_Arr(GDCC::IR::ArgPtr2 const &arr)
      {
         numChunkCODE += IsExp0(arr.off) ? 8 : 20;
      }

      //
      // Info::genStmnt_Move_W__Stk_Lit
      //
      void Info::genStmnt_Move_W__Stk_Lit(GDCC::IR::Exp const *exp)
      {
         auto type = exp->getType();

         switch(type.t)
         {
         case GDCC::IR::TypeBase::Funct: numChunkCODE +=  8; break;
         case GDCC::IR::TypeBase::StrEn: numChunkCODE += 12; break;

         default: numChunkCODE += 8; break;
         }
      }

      //
      // Info::genStmnt_Retn
      //
      void Info::genStmnt_Retn()
      {
         auto argc = stmnt->args.size();

         switch(func->ctype)
         {
         case GDCC::IR::CallType::LangACS:
            if(argc == 0 || argc == 1)
               numChunkCODE += 4;
            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }
            break;

         case GDCC::IR::CallType::Script:
         case GDCC::IR::CallType::ScriptI:
         case GDCC::IR::CallType::ScriptS:
            if(argc == 0)
               numChunkCODE += 4;
            else if(argc == 1)
               numChunkCODE += 8;
            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }
            break;

         default:
            std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Retn\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

