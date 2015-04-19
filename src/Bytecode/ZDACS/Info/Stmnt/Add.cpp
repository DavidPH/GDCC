//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Program.hpp"


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
         // Info::genStmnt_AdXU_W
         //
         void Info::genStmnt_AdXU_W()
         {
            Core::FastU lenAdXU_W1 =
               lenPushArg(stmnt->args[1], 0) * 2 +
               lenPushArg(stmnt->args[2], 0) + 16;

            // No carry.
            if(stmnt->args.size() == 3)
               numChunkCODE += lenAdXU_W1;

            // With carry.
            else
               numChunkCODE += lenAdXU_W1 * 2 + 56;
         }

         //
         // Info::genStmnt_AddU_W
         //
         void Info::genStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               genStmntCall(stmnt->op.size);
               return;
            }

            if(stmnt->args[0].a == IR::ArgBase::Stk)
               numChunkCODE += 4;
            else
               numChunkCODE += 8;
         }

         //
         // Info::genStmnt_SuXU_W
         //
         void Info::genStmnt_SuXU_W()
         {
            Core::FastU lenSuXU_W1 =
               lenPushArg(stmnt->args[1], 0) * 2 +
               lenPushArg(stmnt->args[2], 0) + 20;

            // No carry.
            if(stmnt->args.size() == 3)
               numChunkCODE += lenSuXU_W1;

            // With carry.
            else
               numChunkCODE += lenSuXU_W1 * 2 + 64;
         }

         //
         // Info::preStmnt_AddU_W
         //
         void Info::preStmnt_AddU_W(IR::Code code)
         {
            if(stmnt->op.size < 2)
               return;

            Core::String name = getCallName();

            preStmntCall(name, stmnt->op.size, stmnt->op.size * 2);

            IR::Function &newFunc = prog->getFunction(name);

            if(newFunc.defin)
               return;

            newFunc.defin    = true;
            newFunc.label    = name + "$label";
            newFunc.localReg = stmnt->op.size * 2;

            newFunc.block.setOrigin({__FILE__, __LINE__});

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc.block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc.block.getExp(stmnt->op.size))};

            // First words.
            newFunc.block.addStatementArgs({code, 1},
               IR::Arg_Stk(), lop, rop);

            // Mid words.
            for(Core::FastU i = stmnt->op.size - 2; i--;)
            {
               newFunc.block.addStatementArgs({code, 1},
                  IR::Arg_Stk(), ++lop, ++rop, IR::Arg_Stk());
            }

            // Last words.
            newFunc.block.addStatementArgs({stmnt->op.code, 1},
               IR::Arg_Stk(), ++lop, ++rop);
            newFunc.block.addStatementArgs({IR::Code::AddU_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

            // Return.
            newFunc.block.addStatementArgs({IR::Code::Retn, stmnt->op.size},
               IR::Arg_Stk());

            throw ResetFunc();
         }

         //
         // Info::putStmnt_AdXU_W
         //
         void Info::putStmnt_AdXU_W()
         {
            //
            // putAdXU_W1
            //
            auto putAdXU_W1 = [&]()
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::AddU);
               putCode(Code::Copy);
               putStmntPushArg(stmnt->args[1], 0);
               putStmntCall("___GDCC__CmpU_LT_W1", 1);
            };

            // No carry.
            if(stmnt->args.size() == 3)
            {
               putAdXU_W1();
            }

            // With carry.
            else
            {
               Core::FastU lenAdXU_W1 =
                  lenPushArg(stmnt->args[1], 0) * 2 +
                  lenPushArg(stmnt->args[2], 0) + 16;

               Core::FastU lenCarry0 = lenAdXU_W1 +  8;
               Core::FastU lenCarry1 = lenAdXU_W1 + 40;

               putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

               putAdXU_W1();
               putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

               putAdXU_W1();
               putCode(Code::Drop_LocReg, func->localReg + 0);
               putCode(Code::Push_Lit,    1);
               putCode(Code::AddU);
               putCode(Code::Copy);
               putCode(Code::NotU);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::OrIU);
            }
         }

         //
         // Info::putStmnt_AddU_W
         //
         void Info::putStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code add, Code inc)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? inc : add);
               putWord(GetWord(a.idx->aLit) + a.off);
            };

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
               putReg(stmnt->args[0].aGblReg, Code::AddU_GblReg, Code::IncU_GblReg);
               break;

            case IR::ArgBase::LocReg:
               putReg(stmnt->args[0].aLocReg, Code::AddU_LocReg, Code::IncU_LocReg);
               break;

            case IR::ArgBase::MapReg:
               putReg(stmnt->args[0].aMapReg, Code::AddU_MapReg, Code::IncU_MapReg);
               break;

            case IR::ArgBase::WldReg:
               putReg(stmnt->args[0].aWldReg, Code::AddU_WldReg, Code::IncU_WldReg);
               break;

            default:
               putCode(Code::AddU);
               break;
            }
         }

         //
         // Info::putStmnt_SuXU_W
         //
         void Info::putStmnt_SuXU_W()
         {
            //
            // putSuXU_W1
            //
            auto putSuXU_W1 = [&]()
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::SubU);
               putCode(Code::Copy);
               putStmntPushArg(stmnt->args[1], 0);
               putStmntCall("___GDCC__CmpU_GT_W1", 1);
               putCode(Code::NegI);
            };

            // No carry.
            if(stmnt->args.size() == 3)
            {
               putSuXU_W1();
            }

            // With carry.
            else
            {
               Core::FastU lenSuXU_W1 =
                  lenPushArg(stmnt->args[1], 0) * 2 +
                  lenPushArg(stmnt->args[2], 0) + 20;

               Core::FastU lenCarry0 = lenSuXU_W1 +  8;
               Core::FastU lenCarry1 = lenSuXU_W1 + 48;

               putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

               putSuXU_W1();
               putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

               putSuXU_W1();
               putCode(Code::Drop_LocReg, func->localReg + 0);
               putCode(Code::Push_Lit,    1);
               putCode(Code::SubU);
               putCode(Code::Copy);
               putCode(Code::InvU);
               putCode(Code::NotU);
               putCode(Code::NegI);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::OrIU);
            }
         }

         //
         // Info::putStmnt_SubU_W
         //
         void Info::putStmnt_SubU_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code sub, Code dec)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? dec : sub);
               putWord(GetWord(a.idx->aLit) + a.off);
            };

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
               putReg(stmnt->args[0].aGblReg, Code::SubU_GblReg, Code::DecU_GblReg);
               break;

            case IR::ArgBase::LocReg:
               putReg(stmnt->args[0].aLocReg, Code::SubU_LocReg, Code::DecU_LocReg);
               break;

            case IR::ArgBase::MapReg:
               putReg(stmnt->args[0].aMapReg, Code::SubU_MapReg, Code::DecU_MapReg);
               break;

            case IR::ArgBase::WldReg:
               putReg(stmnt->args[0].aWldReg, Code::SubU_WldReg, Code::DecU_WldReg);
               break;

            default:
               putCode(Code::SubU);
               break;
            }
         }

         //
         // Info::trStmnt_AdXU_W
         //
         void Info::trStmnt_AdXU_W()
         {
            CheckArgC(stmnt, 3);

            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "unsupported AdXU_W size");

            moveArgStk_dst(stmnt->args[0], 2);

            if(stmnt->args.size() > 3)
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[3], 1);
            }
         }

         //
         // Info::trStmnt_AddU_W
         //
         void Info::trStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);
               return;
            }

            if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::MapReg:
            case IR::ArgBase::WldReg:
               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  GetWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }
            else if(stmnt->args[0] == stmnt->args[2]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::MapReg:
            case IR::ArgBase::WldReg:
               std::swap(stmnt->args[1], stmnt->args[2]);

               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  GetWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }

            trStmntStk3(stmnt->op.size, stmnt->op.size, false);
         }

         //
         // Info::trStmnt_SuXU_W
         //
         void Info::trStmnt_SuXU_W()
         {
            CheckArgC(stmnt, 3);

            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "unsupported SuXU_W size");

            moveArgStk_dst(stmnt->args[0], 2);

            if(stmnt->args.size() > 3)
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[3], 1);
            }
         }

         //
         // Info::trStmnt_SubU_W
         //
         void Info::trStmnt_SubU_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, true);
               return;
            }

            if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::MapReg:
            case IR::ArgBase::WldReg:
               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  GetWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }

            trStmntStk3(stmnt->op.size, stmnt->op.size, true);
         }
      }
   }
}

// EOF

