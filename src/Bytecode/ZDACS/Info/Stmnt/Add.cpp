//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "IR/Function.hpp"


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
         // Info::genStmnt_AddU_W
         //
         void Info::genStmnt_AddU_W()
         {
            if(stmnt->args[0].a == IR::ArgBase::Stk)
               numChunkCODE += 4;
            else
               numChunkCODE += 8;
         }

         //
         // Info::putStmnt_AddU_W
         //
         void Info::putStmnt_AddU_W()
         {
            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code add, Code inc)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? inc : add);
               putWord(GetWord(a.idx->aLit) + GetWord(a.off));
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
         // Info::putStmnt_SubU_W
         //
         void Info::putStmnt_SubU_W()
         {
            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code sub, Code dec)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? dec : sub);
               putWord(GetWord(a.idx->aLit) + GetWord(a.off));
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
         // Info::trStmnt_AddU_W
         //
         void Info::trStmnt_AddU_W()
         {
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
                  moveArgStk_src(stmnt->args[2], IR::Code::Move_W);
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
                  moveArgStk_src(stmnt->args[2], IR::Code::Move_W);
               }

               return;

            default:
               break;
            }

            trStmntStk3(IR::Code::Move_W, IR::Code::Move_W, false);
         }

         //
         // Info::trStmnt_SubU_W
         //
         void Info::trStmnt_SubU_W()
         {
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
                  moveArgStk_src(stmnt->args[2], IR::Code::Move_W);
               }

               return;

            default:
               break;
            }

            trStmntStk3(IR::Code::Move_W, IR::Code::Move_W, true);
         }
      }
   }
}

// EOF

