//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement translation.
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
      // Info::trStmnt
      //
      void Info::trStmnt()
      {
         // Generate label glyphs.
         if(!stmnt->labs.empty())
         {
            auto val = GDCC::IR::ExpCreate_ValueRoot(
               GDCC::IR::Value_Fixed(CodeBase() + numChunkCODE, TypeWord), stmnt->pos);

            for(auto const &lab : stmnt->labs)
            {
               auto &data = GDCC::IR::Glyph::GetData(lab);

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
            CheckArgC(stmnt, 3);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 2, GDCC::IR::ArgBase::Stk);
            numChunkCODE += 4;
            break;

         case GDCC::IR::Code::Call:
            trStmnt_Call();
            break;

         case GDCC::IR::Code::Casm:
            CheckArgC(stmnt, 1);
            for(auto n = stmnt->args.size(); --n;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Lit);
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
            CheckArgC(stmnt, 3);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 2, GDCC::IR::ArgBase::Stk);
            numChunkCODE += 4;
            break;

         case GDCC::IR::Code::Cnat:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Lit);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);
            for(auto n = stmnt->args.size(); n-- != 2;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);
            numChunkCODE += 12;
            break;

         case GDCC::IR::Code::Cspe:
            trStmnt_Cspe();
            break;

         case GDCC::IR::Code::InvU_W:
         case GDCC::IR::Code::NegI_W:
         case GDCC::IR::Code::NotU_W:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            numChunkCODE += 4;
            break;

         case GDCC::IR::Code::Move_W:
            trStmnt_Move_W();
            break;

         case GDCC::IR::Code::Retn:
            trStmnt_Retn();
            break;

         case GDCC::IR::Code::Swap_W:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            numChunkCODE += 4;
            break;

         default:
            std::cerr << "ERROR: " << stmnt->pos << ": cannot translate Code for ZDACS: "
               << stmnt->code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::trStmnt_Call
      //
      void Info::trStmnt_Call()
      {
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);
         for(auto n = stmnt->args.size(); --n != 1;)
            CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);

         auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

         switch(stmnt->args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            if(ret == 0)
               numChunkCODE += 8;

            else if(ret == 1)
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
      // Info::trStmnt_Cspe
      //
      void Info::trStmnt_Cspe()
      {
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Lit);
         CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);

         auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

         if(ret > 1)
         {
            std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe ret\n";
            throw EXIT_FAILURE;
         }

         // Too many call args.
         if(stmnt->args.size() > 7)
         {
            std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe arg count\n";
            throw EXIT_FAILURE;
         }

         // No call args.
         if(stmnt->args.size() == 2)
         {
            numChunkCODE += ret ? 48 : 12;
            return;
         }

         switch(stmnt->args[2].a)
         {
         case GDCC::IR::ArgBase::Lit:
            for(auto n = stmnt->args.size(); n-- != 3;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Lit);
            numChunkCODE += 8 + (stmnt->args.size() - 2) * (ret ? 8 : 4);
            break;

         case GDCC::IR::ArgBase::Stk:
            for(auto n = stmnt->args.size(); n-- != 3;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);
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
      // Info::trStmnt_Move_W
      //
      void Info::trStmnt_Move_W()
      {
         CheckArgC(stmnt, 2);
         CheckArg(stmnt->args[0], stmnt->pos);
         CheckArg(stmnt->args[1], stmnt->pos);

         switch(stmnt->args[0].a)
         {
         case GDCC::IR::ArgBase::GblArr:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               trStmnt_Move_W__Arr_Stk(stmnt->args[0].aGblArr);
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
               trStmnt_Move_W__Arr_Stk(stmnt->args[0].aMapArr);
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
               trStmnt_Move_W__Stk_Lit(stmnt->args[1].aLit.value);
               break;

            case GDCC::IR::ArgBase::GblArr:
               trStmnt_Move_W__Stk_Arr(stmnt->args[1].aGblArr);
               break;

            case GDCC::IR::ArgBase::MapArr:
               trStmnt_Move_W__Stk_Arr(stmnt->args[1].aMapArr);
               break;

            case GDCC::IR::ArgBase::WldArr:
               trStmnt_Move_W__Stk_Arr(stmnt->args[1].aWldArr);
               break;

            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::WldArr:
            switch(stmnt->args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               trStmnt_Move_W__Arr_Stk(stmnt->args[0].aWldArr);
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
      // Info::trStmnt_Move_W__Arr_Stk
      //
      void Info::trStmnt_Move_W__Arr_Stk(GDCC::IR::ArgPtr2 const &arr)
      {
         CheckArgB(*arr.idx, GDCC::IR::ArgBase::Stk, stmnt->pos);

         numChunkCODE += IsExp0(arr.off) ? 12 : 24;
      }

      //
      // Info::trStmnt_Move_W__Stk_Arr
      //
      void Info::trStmnt_Move_W__Stk_Arr(GDCC::IR::ArgPtr2 const &arr)
      {
         CheckArgB(*arr.idx, GDCC::IR::ArgBase::Stk, stmnt->pos);

         numChunkCODE += IsExp0(arr.off) ? 8 : 20;
      }

      //
      // Info::trStmnt_Move_W__Stk_Lit
      //
      void Info::trStmnt_Move_W__Stk_Lit(GDCC::IR::Exp const *exp)
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
      // Info::trStmnt_Retn
      //
      void Info::trStmnt_Retn()
      {
         for(auto n = stmnt->args.size(); n--;)
            CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);

         switch(func->ctype)
         {
         case GDCC::IR::CallType::LangACS:
            if(stmnt->args.size() == 0)
               numChunkCODE += 4;
            else if(stmnt->args.size() == 1)
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
            if(stmnt->args.size() == 0)
               numChunkCODE += 4;
            else if(stmnt->args.size() == 1)
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

