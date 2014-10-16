//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement translation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/CallType.hpp"
#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::trStmnt
         //
         void Info::trStmnt()
         {
            switch(stmnt->code)
            {
            case IR::Code::Nop:
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:
            case IR::Code::AndU_W:
            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
            case IR::Code::MulX_W:
            case IR::Code::OrIU_W:
            case IR::Code::OrXU_W:
               trStmntStk3(IR::Code::Move_W, false);
               break;

            case IR::Code::AndU_W2:
            case IR::Code::OrIU_W2:
            case IR::Code::OrXU_W2:
               trStmntBitwise2();
               break;

            case IR::Code::AndU_W3:
            case IR::Code::OrIU_W3:
            case IR::Code::OrXU_W3:
               trStmntBitwise3();
               break;

            case IR::Code::Call:
               trStmnt_Call();
               break;

            case IR::Code::Casm:
               CheckArgC(stmnt, 1);
               for(auto n = stmnt->args.size(); --n;)
                  CheckArgB(stmnt, n, IR::ArgBase::Lit);
               break;

            case IR::Code::Cjmp_Nil:
            case IR::Code::Cjmp_Tru:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Lit);
               break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpI_GE_W:
            case IR::Code::CmpI_GT_W:
            case IR::Code::CmpI_LE_W:
            case IR::Code::CmpI_LT_W:
            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_GE_W:
            case IR::Code::CmpU_GT_W:
            case IR::Code::CmpU_LE_W:
            case IR::Code::CmpU_LT_W:
            case IR::Code::CmpU_NE_W:
               trStmntStk3(IR::Code::Move_W, true);
               break;

            case IR::Code::Cnat:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               CheckArgB(stmnt, 1, IR::ArgBase::Lit);
               for(auto n = stmnt->args.size(); n-- != 2;)
                  CheckArgB(stmnt, n, IR::ArgBase::Stk);
               break;

            case IR::Code::Cspe:
               trStmnt_Cspe();
               break;

            case IR::Code::DivI_W:
            case IR::Code::DivU_W:
            case IR::Code::DivX_W:
            case IR::Code::ModI_W:
            case IR::Code::ModU_W:
            case IR::Code::ShLU_W:
            case IR::Code::ShRI_W:
            case IR::Code::SubI_W:
            case IR::Code::SubU_W:
               trStmntStk3(IR::Code::Move_W, true);
               break;

            case IR::Code::InvU_W:
            case IR::Code::NegI_W:
            case IR::Code::NotU_W:
               CheckArgC(stmnt, 2);
               moveArgStk_W_dst(stmnt->args[0], IR::Code::Move_W);
               moveArgStk_W_src(stmnt->args[1], IR::Code::Move_W);
               break;

            case IR::Code::Jump:
               CheckArgC(stmnt, 1);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               break;

            case IR::Code::Move_W:
               trStmnt_Move_W();
               break;

            case IR::Code::Plsa:
               CheckArgC(stmnt, 1);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               break;

            case IR::Code::Plsd:
            case IR::Code::Plsi:
            case IR::Code::Plsr:
               CheckArgC(stmnt, 1);
               moveArgStk_W_src(stmnt->args[0], IR::Code::Move_W);
               break;

            case IR::Code::Plsf:
               break;

            case IR::Code::Plss:
               CheckArgC(stmnt, 1);
               moveArgStk_W_dst(stmnt->args[0], IR::Code::Move_W);
               break;

            case IR::Code::Pltn:
               CheckArgC(stmnt, 2);
               moveArgStk_W_dst(stmnt->args[0], IR::Code::Move_W);
               moveArgStk_W_src(stmnt->args[1], IR::Code::Move_W);
               break;

            case IR::Code::Retn:
               trStmnt_Retn();
               break;

            case IR::Code::ShLU_W2: trStmnt_ShLU_W2(); break;
            case IR::Code::ShLU_W3: trStmnt_ShLU_W3(); break;

            case IR::Code::ShRI_W3: trStmnt_ShRI_W3(); break;
            case IR::Code::ShRI_W2: trStmnt_ShRI_W2(); break;

            case IR::Code::ShRU_W:  trStmnt_ShRU_W(); break;
            case IR::Code::ShRU_W2: trStmnt_ShRU_W2(); break;
            case IR::Code::ShRU_W3: trStmnt_ShRU_W3(); break;

            case IR::Code::Swap_W:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               break;

            case IR::Code::Xcod_SID:
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
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);
            for(auto n = stmnt->args.size(); --n != 1;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
            case IR::ArgBase::Stk:
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
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);

            auto ret = stmnt->args[1].aLit.value->getValue().getFastU();

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
               return;

            switch(stmnt->args[2].a)
            {
            case IR::ArgBase::Lit:
               for(auto n = stmnt->args.size(); n-- != 3;)
                  CheckArgB(stmnt, n, IR::ArgBase::Lit);
               break;

            case IR::ArgBase::Stk:
               for(auto n = stmnt->args.size(); n-- != 3;)
                  CheckArgB(stmnt, n, IR::ArgBase::Stk);
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

            #define moveIdx(name, n) \
               moveArgStk_W_src(*stmnt->args[n].a##name.idx, IR::Code::Move_W)

            // Push to stack?
            if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
            {
            case IR::ArgBase::GblReg: break;
            case IR::ArgBase::Lit:    break;
            case IR::ArgBase::LocReg: break;
            case IR::ArgBase::MapReg: break;
            case IR::ArgBase::WldReg: break;

            case IR::ArgBase::GblArr: moveIdx(GblArr, 1); break;
            case IR::ArgBase::Loc:    moveIdx(Loc,    1); break;
            case IR::ArgBase::LocArs: moveIdx(LocArs, 1); break;
            case IR::ArgBase::MapArr: moveIdx(MapArr, 1); break;
            case IR::ArgBase::WldArr: moveIdx(WldArr, 1); break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Move_W(Stk, "
                  << stmnt->args[1].a << ")\n";
               throw EXIT_FAILURE;
            }

            // Drop from stack?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg: break;
            case IR::ArgBase::LocReg: break;
            case IR::ArgBase::MapReg: break;
            case IR::ArgBase::Nul:    break;
            case IR::ArgBase::WldReg: break;

            case IR::ArgBase::GblArr: moveIdx(GblArr, 0); break;
            case IR::ArgBase::Loc:    moveIdx(Loc,    0); break;
            case IR::ArgBase::LocArs: moveIdx(LocArs, 0); break;
            case IR::ArgBase::MapArr: moveIdx(MapArr, 0); break;
            case IR::ArgBase::WldArr: moveIdx(WldArr, 0); break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Move_W("
                  << stmnt->args[0].a << ", Stk)\n";
               throw EXIT_FAILURE;
            }

            // Neither stack, split move and rescan.
            else
               moveArgStk_W_src(stmnt->args[1], IR::Code::Move_W);

            #undef moveIdx
         }

         //
         // Info::trStmnt_Retn
         //
         void Info::trStmnt_Retn()
         {
            auto argc = stmnt->args.size();

            for(auto n = argc; n--;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);

            switch(func->ctype)
            {
            case IR::CallType::StdCall:
            case IR::CallType::StkCall:
               break;

            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               if(argc != 0 && argc != 1)
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

         //
         // Info::trStmntBitwise2
         //
         void Info::trStmntBitwise2()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_W_dst(stmnt->args[0], IR::Code::Move_W2);
            }
            else
            {
               trStmntStk3(IR::Code::Move_W2, false);
               func->setLocalTmp(3);
            }
         }

         //
         // Info::trStmntBitwise3
         //
         void Info::trStmntBitwise3()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_W_dst(stmnt->args[0], IR::Code::Move_W3);
            }
            else
            {
               trStmntStk3(IR::Code::Move_W3, false);
               func->setLocalTmp(5);
            }
         }

         //
         // Info::trStmntStk3
         //
         void Info::trStmntStk3(IR::Code codeMove, bool ordered)
         {
            CheckArgC(stmnt, 3);

            if(ordered && stmnt->args[1].a != IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
               throw Core::ExceptStr(stmnt->pos, "trStmntStk3 disorder");

            moveArgStk_W_dst(stmnt->args[0], codeMove);
            if(moveArgStk_W_src(stmnt->args[1], codeMove)) return;
            moveArgStk_W_src(stmnt->args[2], codeMove);
         }
      }
   }
}

// EOF

