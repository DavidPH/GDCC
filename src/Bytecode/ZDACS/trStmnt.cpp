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
      void Info::trStmnt(GDCC::IR::Statement &stmnt)
      {
         // Generate label glyphs.
         if(!stmnt.labs.empty())
         {
            auto val = GDCC::IR::ExpCreate_ValueRoot(
               GDCC::IR::Value_Fixed(jumpPos, TypeWord), stmnt.pos);

            for(auto const &lab : stmnt.labs)
            {
               auto &data = GDCC::IR::Glyph::GetData(lab);

               data.type  = TypeWord;
               data.value = val;
            }
         }

         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            jumpPos += 4;
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
            jumpPos += 4;
            break;

         case GDCC::IR::Code::Call:
            trStmnt_Call(stmnt);
            break;

         case GDCC::IR::Code::Casm:
            CheckArgC(stmnt, 1);
            for(auto n = stmnt.args.size(); --n;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Lit);
            jumpPos += stmnt.args.size() * 4;
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
            jumpPos += 4;
            break;

         case GDCC::IR::Code::Cnat:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Lit);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);
            for(auto n = stmnt.args.size(); n-- != 2;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);
            jumpPos += 12;
            break;

         case GDCC::IR::Code::Cspe:
            trStmnt_Cspe(stmnt);
            break;

         case GDCC::IR::Code::InvU_W:
         case GDCC::IR::Code::NegI_W:
         case GDCC::IR::Code::NotU_W:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            jumpPos += 4;
            break;

         case GDCC::IR::Code::Move_W:
            trStmnt_Move_W(stmnt);
            break;

         case GDCC::IR::Code::Retn:
            trStmnt_Retn(stmnt);
            break;

         case GDCC::IR::Code::Swap_W:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            jumpPos += 4;
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": cannot translate Code for ZDACS: "
               << stmnt.code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::trStmnt_Call
      //
      void Info::trStmnt_Call(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);
         for(auto n = stmnt.args.size(); --n != 1;)
            CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);

         auto ret = ResolveValue(stmnt.args[1].aLit.value->getValue());

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            if(ret == 0)
               jumpPos += 8;

            else if(ret == 1)
               jumpPos += 8;

            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }

            break;

         case GDCC::IR::ArgBase::Stk:
            if(ret == 0)
               jumpPos += 8;

            else if(ret == 1)
               jumpPos += 4;

            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }

            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Call\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::trStmnt_Cspe
      //
      void Info::trStmnt_Cspe(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Lit);
         CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);

         auto ret = ResolveValue(stmnt.args[1].aLit.value->getValue());

         if(ret > 1)
         {
            std::cerr << "ERROR: " << stmnt.pos << ": bad Cspe ret\n";
            throw EXIT_FAILURE;
         }

         // Too many call args.
         if(stmnt.args.size() > 7)
         {
            std::cerr << "ERROR: " << stmnt.pos << ": bad Cspe arg count\n";
            throw EXIT_FAILURE;
         }

         // No call args.
         if(stmnt.args.size() == 2)
         {
            jumpPos += ret ? 48 : 12;
            return;
         }

         switch(stmnt.args[2].a)
         {
         case GDCC::IR::ArgBase::Lit:
            for(auto n = stmnt.args.size(); n-- != 3;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Lit);
            jumpPos += 8 + (stmnt.args.size() - 2) * (ret ? 8 : 4);
            break;

         case GDCC::IR::ArgBase::Stk:
            for(auto n = stmnt.args.size(); n-- != 3;)
               CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);
            jumpPos += 8;

            // Dummy args.
            if(ret) jumpPos += (7 - stmnt.args.size()) * 8;

            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Cspe\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::trStmnt_Move_W
      //
      void Info::trStmnt_Move_W(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArg(stmnt.args[0], stmnt.pos);
         CheckArg(stmnt.args[1], stmnt.pos);

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::GblArr:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               trStmnt_Move_W__Arr_Stk(stmnt, stmnt.args[0].aGblArr);
               break;

            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::GblReg:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: jumpPos += 8; break;
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

         case GDCC::IR::ArgBase::MapArr:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               trStmnt_Move_W__Arr_Stk(stmnt, stmnt.args[0].aMapArr);
               break;

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
            case GDCC::IR::ArgBase::LocReg: jumpPos += 8; break;
            case GDCC::IR::ArgBase::MapReg: jumpPos += 8; break;
            case GDCC::IR::ArgBase::WldReg: jumpPos += 8; break;

            case GDCC::IR::ArgBase::Lit:
               trStmnt_Move_W__Stk_Lit(stmnt, stmnt.args[1].aLit.value);
               break;

            case GDCC::IR::ArgBase::GblArr:
               trStmnt_Move_W__Stk_Arr(stmnt, stmnt.args[1].aGblArr);
               break;

            case GDCC::IR::ArgBase::MapArr:
               trStmnt_Move_W__Stk_Arr(stmnt, stmnt.args[1].aMapArr);
               break;

            case GDCC::IR::ArgBase::WldArr:
               trStmnt_Move_W__Stk_Arr(stmnt, stmnt.args[1].aWldArr);
               break;

            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::WldArr:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk:
               trStmnt_Move_W__Arr_Stk(stmnt, stmnt.args[0].aWldArr);
               break;

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
      // Info::trStmnt_Move_W__Arr_Stk
      //
      void Info::trStmnt_Move_W__Arr_Stk(GDCC::IR::Statement &stmnt, GDCC::IR::ArgPtr2 const &arr)
      {
         CheckArgB(*arr.idx, GDCC::IR::ArgBase::Stk, stmnt.pos);

         jumpPos += IsExp0(arr.off) ? 24 : 12;
      }

      //
      // Info::trStmnt_Move_W__Stk_Arr
      //
      void Info::trStmnt_Move_W__Stk_Arr(GDCC::IR::Statement &stmnt, GDCC::IR::ArgPtr2 const &arr)
      {
         CheckArgB(*arr.idx, GDCC::IR::ArgBase::Stk, stmnt.pos);

         jumpPos += IsExp0(arr.off) ? 20 : 8;
      }

      //
      // Info::trStmnt_Move_W__Stk_Lit
      //
      void Info::trStmnt_Move_W__Stk_Lit(GDCC::IR::Statement &, GDCC::IR::Exp const *exp)
      {
         auto type = exp->getType();

         switch(type.t)
         {
         case GDCC::IR::TypeBase::Funct: jumpPos += 8; break;
         case GDCC::IR::TypeBase::StrEn: jumpPos += 12; break;

         default: jumpPos += 8; break;
         }
      }

      //
      // Info::trStmnt_Retn
      //
      void Info::trStmnt_Retn(GDCC::IR::Statement &stmnt)
      {
         for(auto n = stmnt.args.size(); n--;)
            CheckArgB(stmnt, n, GDCC::IR::ArgBase::Stk);

         switch(curFunc->ctype)
         {
         case GDCC::IR::CallType::LangACS:
            if(stmnt.args.size() == 0)
               jumpPos += 4;
            else if(stmnt.args.size() == 1)
               jumpPos += 4;
            else
            {
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;
            }
            break;

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

