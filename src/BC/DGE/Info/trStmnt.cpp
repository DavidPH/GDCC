//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati statement translations.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace DGE
      {
         //
         // Info::trStmnt
         //
         void Info::trStmnt()
         {
            switch(stmnt->op.code)
            {
            case IR::Code::AddU_W:
            case IR::Code::AndU_W:
            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_NE_W:
            case IR::Code::MulU_W:
            case IR::Code::OrIU_W:
            case IR::Code::OrXU_W:
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);
               break;

            case IR::Code::CmpI_GE_W:
            case IR::Code::CmpI_GT_W:
            case IR::Code::CmpI_LE_W:
            case IR::Code::CmpI_LT_W:
            case IR::Code::CmpU_GE_W:
            case IR::Code::CmpU_GT_W:
            case IR::Code::CmpU_LE_W:
            case IR::Code::CmpU_LT_W:
            case IR::Code::DivI_W:
            case IR::Code::DivU_W:
            case IR::Code::ModI_W:
            case IR::Code::ModU_W:
            case IR::Code::SubU_W:
               trStmntStk3(stmnt->op.size, stmnt->op.size, true);
               break;

            case IR::Code::Call: trStmnt_Call(); break;
            case IR::Code::Cnat: trStmnt_Cnat(); break;

            case IR::Code::Jcnd_Nil:
            case IR::Code::Jcnd_Tru:
               CheckArgC(stmnt, 2);
               moveArgStk_src(stmnt->args[0], stmnt->op.size);
               CheckArgB(stmnt, 1, IR::ArgBase::Lit);
               break;

            case IR::Code::Jump:
               CheckArgC(stmnt, 1);
               if(stmnt->args[0].a != IR::ArgBase::Lit)
                  moveArgStk_src(stmnt->args[0], stmnt->op.size);
               break;

            case IR::Code::Move_W: trStmnt_Move_W(); break;

            case IR::Code::NotU_W: trStmntStk2(1, stmnt->op.size); break;

            case IR::Code::Retn: trStmnt_Retn(); break;

            case IR::Code::Swap_W:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot translate Code for DGE: " << stmnt->op << '\n';
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

            // Multi-word?
            if(stmnt->op.size != 1)
               return;

            #define moveIdx(name, n, w) \
               moveArgStk_src(*stmnt->args[n].a##name.idx, w)

            // Push to stack?
            if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
            {
            case IR::ArgBase::Lit:    break;
            case IR::ArgBase::LocReg: break;

            case IR::ArgBase::Aut:    moveIdx(Aut,    1, 1); break;
            case IR::ArgBase::Sta:    moveIdx(Sta,    1, 1); break;
            case IR::ArgBase::StrArs: moveIdx(StrArs, 1, 2); break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad tr Move_W push");
            }

            // Drop from stack?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::LocReg: break;
            case IR::ArgBase::Nul:    break;

            case IR::ArgBase::Aut:    moveIdx(Aut,    0, 1); break;
            case IR::ArgBase::Sta:    moveIdx(Sta,    0, 1); break;
            case IR::ArgBase::StrArs: moveIdx(StrArs, 0, 2); break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad tr Move_W drop");
            }

            // Neither stack, split move and rescan.
            else
               moveArgStk_src(stmnt->args[1], stmnt->op.size);

            #undef moveIdx
         }
      }
   }
}

// EOF

