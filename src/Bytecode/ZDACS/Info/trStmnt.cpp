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

            case IR::Code::AddF_W:
            case IR::Code::AndU_W:
            case IR::Code::MulF_W:
            case IR::Code::MulI_W:
            case IR::Code::MulK_W:
            case IR::Code::MulU_W:
            case IR::Code::MulX_W:
            case IR::Code::OrIU_W:
            case IR::Code::OrXU_W:
               trStmntStk3(IR::Code::Move_W, IR::Code::Move_W, false);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W: trStmnt_AddU_W(); break;

            case IR::Code::AddF_W2:
            case IR::Code::AddI_W2:
            case IR::Code::AddU_W2:
            case IR::Code::MulF_W2:
            case IR::Code::MulI_W2:
            case IR::Code::MulK_W2:
            case IR::Code::MulU_W2:
            case IR::Code::MulX_W2:
               trStmntStk3(IR::Code::Move_W2, IR::Code::Move_W2, false);
               break;

            case IR::Code::AddI_W3:
            case IR::Code::AddU_W3:
            case IR::Code::MulI_W3:
            case IR::Code::MulU_W3:
               trStmntStk3(IR::Code::Move_W3, IR::Code::Move_W3, false);
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

            case IR::Code::CmpF_EQ_W:
            case IR::Code::CmpF_NE_W:
            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_NE_W:
               trStmntStk3(IR::Code::Move_W, IR::Code::Move_W, false);
               break;

            case IR::Code::CmpF_EQ_W2:
            case IR::Code::CmpF_NE_W2:
            case IR::Code::CmpI_EQ_W2:
            case IR::Code::CmpI_NE_W2:
            case IR::Code::CmpU_EQ_W2:
            case IR::Code::CmpU_NE_W2:
               trStmnt_CmpU_EQ(IR::Code::Move_W2);
               break;

            case IR::Code::CmpI_EQ_W3:
            case IR::Code::CmpI_NE_W3:
            case IR::Code::CmpU_EQ_W3:
            case IR::Code::CmpU_NE_W3:
               trStmnt_CmpU_EQ(IR::Code::Move_W3);
               break;

            case IR::Code::CmpF_GE_W:
            case IR::Code::CmpF_GT_W:
            case IR::Code::CmpF_LE_W:
            case IR::Code::CmpF_LT_W:
            case IR::Code::CmpI_GE_W:
            case IR::Code::CmpI_GT_W:
            case IR::Code::CmpI_LE_W:
            case IR::Code::CmpI_LT_W:
            case IR::Code::CmpU_GE_W:
            case IR::Code::CmpU_GT_W:
            case IR::Code::CmpU_LE_W:
            case IR::Code::CmpU_LT_W:
               trStmntStk3(IR::Code::Move_W, IR::Code::Move_W, true);
               break;

            case IR::Code::CmpF_GE_W2:
            case IR::Code::CmpF_GT_W2:
            case IR::Code::CmpF_LE_W2:
            case IR::Code::CmpF_LT_W2:
            case IR::Code::CmpI_GE_W2:
            case IR::Code::CmpI_GT_W2:
            case IR::Code::CmpI_LE_W2:
            case IR::Code::CmpI_LT_W2:
            case IR::Code::CmpU_GE_W2:
            case IR::Code::CmpU_GT_W2:
            case IR::Code::CmpU_LE_W2:
            case IR::Code::CmpU_LT_W2:
               trStmntStk3(IR::Code::Move_W, IR::Code::Move_W2, true);
               break;

            case IR::Code::CmpI_GE_W3:
            case IR::Code::CmpI_GT_W3:
            case IR::Code::CmpI_LE_W3:
            case IR::Code::CmpI_LT_W3:
            case IR::Code::CmpU_GE_W3:
            case IR::Code::CmpU_GT_W3:
            case IR::Code::CmpU_LE_W3:
            case IR::Code::CmpU_LT_W3:
               trStmntStk3(IR::Code::Move_W, IR::Code::Move_W3, true);
               break;

            case IR::Code::Cnat:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               CheckArgB(stmnt, 1, IR::ArgBase::Lit);
               for(auto n = stmnt->args.size(); n-- != 2;)
                  CheckArgB(stmnt, n, IR::ArgBase::Stk);
               break;

            case IR::Code::Copy_W:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               break;

            case IR::Code::Cspe:
               trStmnt_Cspe();
               break;

            case IR::Code::DiXI_W: trStmnt_DiXI_W(); break;

            case IR::Code::DiXI_W2:
            case IR::Code::DiXU_W2:
               trStmntStk3(IR::Code::Move_W4, IR::Code::Move_W2, false);
               break;

            case IR::Code::DiXI_W3:
            case IR::Code::DiXU_W3:
               trStmntStk3(IR::Code::Move_W6, IR::Code::Move_W3, false);
               break;

            case IR::Code::DiXU_W:
               trStmntStk3(IR::Code::Move_W2, IR::Code::Move_W, false);
               break;

            case IR::Code::DivF_W:
            case IR::Code::DivI_W:
            case IR::Code::DivK_W:
            case IR::Code::DivU_W:
            case IR::Code::DivX_W:
            case IR::Code::ModI_W:
            case IR::Code::ModU_W:
            case IR::Code::ShLU_W:
            case IR::Code::ShRI_W:
            case IR::Code::SubF_W:
               trStmntStk3(IR::Code::Move_W, IR::Code::Move_W, true);
               break;

            case IR::Code::DivF_W2:
            case IR::Code::DivI_W2:
            case IR::Code::DivK_W2:
            case IR::Code::DivU_W2:
            case IR::Code::DivX_W2:
            case IR::Code::ModI_W2:
            case IR::Code::ModU_W2:
            case IR::Code::SubF_W2:
            case IR::Code::SubI_W2:
            case IR::Code::SubU_W2:
               trStmntStk3(IR::Code::Move_W2, IR::Code::Move_W2, true);
               break;

            case IR::Code::DivI_W3:
            case IR::Code::DivU_W3:
            case IR::Code::ModI_W3:
            case IR::Code::ModU_W3:
            case IR::Code::SubI_W3:
            case IR::Code::SubU_W3:
               trStmntStk3(IR::Code::Move_W3, IR::Code::Move_W3, true);
               break;

            case IR::Code::InvU_W:
            case IR::Code::NegF_W:
            case IR::Code::NegI_W:
            case IR::Code::NotU_W:
               trStmntStk2(IR::Code::Move_W, IR::Code::Move_W);
               break;

            case IR::Code::InvU_W2: trStmnt_InvU_W2(); break;
            case IR::Code::InvU_W3: trStmnt_InvU_W3(); break;

            case IR::Code::Jump:
               CheckArgC(stmnt, 1);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               break;

            case IR::Code::Move_W:  trStmnt_Move_W(); break;
            case IR::Code::Move_W2: trStmnt_Move_Wx(); break;
            case IR::Code::Move_W3: trStmnt_Move_Wx(); break;
            case IR::Code::Move_W4: trStmnt_Move_Wx(); break;
            case IR::Code::Move_W5: trStmnt_Move_Wx(); break;
            case IR::Code::Move_W6: trStmnt_Move_Wx(); break;

            case IR::Code::MuXU_W:
               trStmntStk3(IR::Code::Move_W2, IR::Code::Move_W, false);
               break;

            case IR::Code::MuXU_W2:
               trStmntStk3(IR::Code::Move_W4, IR::Code::Move_W2, false);
               break;

            case IR::Code::NegF_W2:
               trStmntStk2(IR::Code::Move_W2, IR::Code::Move_W2);
               break;

            case IR::Code::NegI_W2: trStmnt_NegI_W2(); break;
            case IR::Code::NegI_W3: trStmnt_NegI_W3(); break;

            case IR::Code::NotU_W2:
               trStmntStk2(IR::Code::Move_W, IR::Code::Move_W2);
               break;

            case IR::Code::NotU_W3:
               trStmntStk2(IR::Code::Move_W, IR::Code::Move_W3);
               break;

            case IR::Code::Plsa:
               CheckArgC(stmnt, 1);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               break;

            case IR::Code::Plsd:
            case IR::Code::Plsi:
            case IR::Code::Plsr:
               CheckArgC(stmnt, 1);
               moveArgStk_src(stmnt->args[0], IR::Code::Move_W);
               break;

            case IR::Code::Plsf:
               break;

            case IR::Code::Plss:
               CheckArgC(stmnt, 1);
               moveArgStk_dst(stmnt->args[0], IR::Code::Move_W);
               break;

            case IR::Code::Pltn:
               trStmntStk2(IR::Code::Move_W, IR::Code::Move_W);
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

            case IR::Code::SubI_W:
            case IR::Code::SubU_W: trStmnt_SubU_W(); break;

            case IR::Code::Swap_W:  trStmnt_Swap_Wx(1); break;
            case IR::Code::Swap_W2: trStmnt_Swap_Wx(2); break;
            case IR::Code::Swap_W3: trStmnt_Swap_Wx(3); break;

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
         // Info::trStmntStk2
         //
         void Info::trStmntStk2(IR::Code moveDst, IR::Code moveSrc)
         {
            CheckArgC(stmnt, 2);

            moveArgStk_dst(stmnt->args[0], moveDst);
            moveArgStk_src(stmnt->args[1], moveSrc);
         }

         //
         // Info::trStmntStk3
         //
         void Info::trStmntStk3(IR::Code moveDst, IR::Code moveSrc, bool ordered)
         {
            CheckArgC(stmnt, 3);

            if(ordered && stmnt->args[1].a != IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
               throw Core::ExceptStr(stmnt->pos, "trStmntStk3 disorder");

            moveArgStk_dst(stmnt->args[0], moveDst);
            if(moveArgStk_src(stmnt->args[1], moveSrc)) return;
            moveArgStk_src(stmnt->args[2], moveSrc);
         }
      }
   }
}

// EOF

