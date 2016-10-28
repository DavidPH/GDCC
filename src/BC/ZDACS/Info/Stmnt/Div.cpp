//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Division statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_DiXI_W
         //
         void Info::genStmnt_DiXI_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               if(stmnt->args[1].a == IR::ArgBase::Stk &&
                  stmnt->args[2].a == IR::ArgBase::Stk)
               {
                  numChunkCODE += 56;
               }
               else
               {
                  numChunkCODE += 8
                     + lenPushArg(stmnt->args[1], 0, 1) * 2
                     + lenPushArg(stmnt->args[2], 0, 1) * 2;
               }

               return;
            }

            genStmntCall(stmnt->op.size * 2);
         }

         //
         // Info::genStmnt_DivI_W
         //
         void Info::genStmnt_DivI_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               numChunkCODE += 4;
               return;
            }

            genStmntCall(stmnt->op.size * 2);

            numChunkCODE += stmnt->op.size * 4;
         }

         //
         // Info::genStmnt_DivU_W
         //
         void Info::genStmnt_DivU_W()
         {
            if(stmnt->op.size == 0)
               return;

            genStmntCall(stmnt->op.size * 2);

            numChunkCODE += stmnt->op.size * 4;
         }

         //
         // Info::genStmnt_DivX_W
         //
         void Info::genStmnt_DivX_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               numChunkCODE += 4;
               return;
            }

            genStmntCall(stmnt->op.size);
         }

         //
         // Info::genStmnt_ModI_W
         //
         void Info::genStmnt_ModI_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               numChunkCODE += 4;
               return;
            }

            genStmntCall(stmnt->op.size * 2);

            numChunkCODE += stmnt->op.size * 20;
         }

         //
         // Info::genStmnt_ModU_W
         //
         void Info::genStmnt_ModU_W()
         {
            if(stmnt->op.size == 0)
               return;

            genStmntCall(stmnt->op.size * 2);

            numChunkCODE += stmnt->op.size * 20;
         }

         //
         // Info::preStmnt_DiXI_W
         //
         void Info::preStmnt_DiXI_W()
         {
            if(stmnt->op.size <= 1)
               return;

            addFunc_DiXI_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_DiXU_W
         //
         void Info::preStmnt_DiXU_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
               return preStmnt_DiXU_W1();

            addFunc_DiXU_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_DiXU_W1
         //
         void Info::preStmnt_DiXU_W1()
         {
            Core::String name = getCallName({IR::Code::DiXU_W, 1});
            auto newFunc = preStmntCallDef(name, 2, 2, 3, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Glyph labelL0     {prog, name + "$l0"};
            IR::Glyph labelL0R0   {prog, name + "$l0r0"};
            IR::Glyph labelL1R0   {prog, name + "$l1r0"};
            IR::Glyph labelL1R00  {prog, name + "$l1r00"};
            IR::Glyph labelL1R01_1{prog, name + "$l1r01_1"};
            IR::Glyph labelL1R01_r{prog, name + "$l1r01_r"};
            IR::Glyph labelL1R1_lt{prog, name + "$l1r1_lt"};

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(1))};
            IR::Arg_LocReg tmp{IR::Arg_Lit(newFunc->block.getExp(2))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, lop, 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1}, stk, labelL0);

            // l has high bit set.
            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, rop, 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1}, stk, labelL1R0);

            // l and r have high bit set.
            // If l < r, result is {0, l}, otherwise result is {1, l - r}.
            AS_Stmnt({IR::Code::CmpI_LT_W, 1}, stk, lop, rop);
            AS_Stmnt({IR::Code::Jcnd_Tru, 1}, stk, labelL1R1_lt);

            AS_Stmnt({IR::Code::Move_W, 1}, stk, 1);
            AS_Stmnt({IR::Code::SubU_W, 1}, stk, lop, rop);
            AS_Stmnt({IR::Code::Retn,   2}, stk);

            newFunc->block.addLabel(labelL1R1_lt);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, 1);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, lop);
            AS_Stmnt({IR::Code::Retn,   2}, stk);

            // l has high bit set, r does not.
            newFunc->block.addLabel(labelL1R0);
            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, rop, 1);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1}, stk, labelL1R00);

            // r has low bit set.

            // Halve l and divide by r, then double result.
            // If the result of the division is 0, then return {1, l - r}.
            AS_Stmnt({IR::Code::ShRU_W,   1}, stk, lop, 1);
            AS_Stmnt({IR::Code::DivI_W,   1}, stk, stk, rop);
            AS_Stmnt({IR::Code::Jcnd_Tab, 1}, stk, 0, labelL1R01_1);
            AS_Stmnt({IR::Code::ShLU_W,   1}, tmp, stk, 1);

            // If l - res * r >= r, add one to result.
            AS_Stmnt({IR::Code::Move_W,    1}, stk, lop);
            AS_Stmnt({IR::Code::MulU_W,    1}, stk, rop, tmp);
            AS_Stmnt({IR::Code::SubU_W,    1}, stk, stk, stk);
            AS_Stmnt({IR::Code::CmpI_GE_W, 1}, stk, stk, rop);
            AS_Stmnt({IR::Code::Jcnd_Nil,  1}, stk, labelL1R01_r);
            AS_Stmnt({IR::Code::AddU_W,    1}, tmp, tmp, 1);

            newFunc->block.addLabel(labelL1R01_r);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, tmp);

            AS_Stmnt({IR::Code::Move_W, 1}, stk, lop);
            AS_Stmnt({IR::Code::MulU_W, 1}, stk, tmp, rop);
            AS_Stmnt({IR::Code::SubU_W, 1}, stk, stk, stk);

            AS_Stmnt({IR::Code::Retn,   2}, stk);

            newFunc->block.addLabel(labelL1R01_1);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, 1);
            AS_Stmnt({IR::Code::SubU_W, 1}, stk, lop, rop);
            AS_Stmnt({IR::Code::Retn,   2}, stk);

            // r has low bit unset.
            // Therefore, div result is (l >> 1) / (r >> 1).
            // Therefore, mod result is l - quot * r.
            newFunc->block.addLabel(labelL1R00);
            AS_Stmnt({IR::Code::ShRU_W, 1}, stk, lop, 1);
            AS_Stmnt({IR::Code::ShRI_W, 1}, stk, rop, 1);
            AS_Stmnt({IR::Code::DivI_W, 1}, tmp, stk, stk);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, tmp);

            AS_Stmnt({IR::Code::Move_W, 1}, stk, lop);
            AS_Stmnt({IR::Code::MulU_W, 1}, stk, tmp, rop);
            AS_Stmnt({IR::Code::SubU_W, 1}, stk, stk, stk);

            AS_Stmnt({IR::Code::Retn,   2}, stk);

            // l does not have high bit set.
            newFunc->block.addLabel(labelL0);
            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, rop, 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1}, stk, labelL0R0);

            // l does not have high bit set, r does.
            // Therefore, l < r, result is {0, l}.
            AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, lop);
            AS_Stmnt({IR::Code::Retn,   2}, stk);

            // l and r do not have high bit set.
            // Therefore, signed division will work.
            newFunc->block.addLabel(labelL0R0);
            AS_Stmnt({IR::Code::DivI_W, 1}, stk, lop, rop);
            AS_Stmnt({IR::Code::ModI_W, 1}, stk, lop, rop);
            AS_Stmnt({IR::Code::Retn,   2}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_DivF_W
         //
         void Info::preStmnt_DivF_W()
         {
            if(stmnt->op.size == 0)
               return;

            addFunc_DivF_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_DivX_W
         //
         void Info::preStmnt_DivX_W(IR::Code code)
         {
            if(stmnt->op.size == 0)
               return;

            bool sign = code == IR::Code::DiXI_W;

            if(stmnt->op.size == 1)
            {
               if(sign)
                  return;
            }

            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, stmnt->op.size * 2,
               stmnt->op.size * 2, stmnt->op.size * 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            Core::FastU fracWords;
            Core::FastU divWords;

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size))};

            IR::Arg_Nul nul{};
            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            if(stmnt->op.size == 1)
            {
               fracWords = 1;
               divWords  = 2;

               AS_Stmnt({IR::Code::Move_W, 1}, stk, lop);
               AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);
               AS_Stmnt({IR::Code::ShLU_W, 1}, stk, stk, 16);

               AS_Stmnt({IR::Code::Move_W, 1}, stk, rop);
               AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);
            }
            else
            {
               fracWords = stmnt->op.size / 2;
               divWords  = stmnt->op.size + fracWords;

               AS_Stmnt({IR::Code::Move_W, fracWords},      stk, 0);
               AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, lop);

               AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, rop);
               if(sign)
               {
                  AS_Stmnt({IR::Code::Copy_W, 1}, stk, stk);
                  AS_Stmnt({IR::Code::ShRI_W, 1}, stk, stk, 31);
                  for(Core::FastU n = fracWords - 1; n--;)
                     AS_Stmnt({IR::Code::Copy_W, 1}, stk, stk);
               }
               else
                  AS_Stmnt({IR::Code::Move_W, fracWords}, stk, 0);
            }

            AS_Stmnt({code,             divWords},  stk, stk, stk);
            AS_Stmnt({IR::Code::Move_W, divWords},  nul, stk);
            AS_Stmnt({IR::Code::Move_W, fracWords}, nul, stk);

            AS_Stmnt({IR::Code::Retn, stmnt->op.size}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::putStmnt_DiXI_W
         //
         void Info::putStmnt_DiXI_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size * 2);
               return;
            }

            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               putCode(Code::Drop_LocReg, func->localReg + 1);
               putCode(Code::Drop_LocReg, func->localReg + 0);

               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 1);
               putCode(Code::DivI);

               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 1);
               putCode(Code::ModI);
            }
            else
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::DivI);

               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::ModI);
            }
         }

         //
         // Info::putStmnt_DivI_W
         //
         void Info::putStmnt_DivI_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               putCode(Code::DivI);
               return;
            }

            putStmntCall(getCallName({IR::Code::DiXI_W, stmnt->op.size}), stmnt->op.size * 2);

            for(Core::FastU n = stmnt->op.size; n--;)
               putCode(Code::Drop_Nul);
         }

         //
         // Info::putStmnt_DivU_W
         //
         void Info::putStmnt_DivU_W()
         {
            if(stmnt->op.size == 0)
               return;

            putStmntCall(getCallName({IR::Code::DiXU_W, stmnt->op.size}), stmnt->op.size * 2);

            for(Core::FastU n = stmnt->op.size; n--;)
               putCode(Code::Drop_Nul);
         }

         //
         // Info::putStmnt_DivX_W
         //
         void Info::putStmnt_DivX_W()
         {
            if(stmnt->op.size == 1)
               putCode(Code::DivX);
            else
               putStmntCall(stmnt->op.size);
         }

         //
         // Info::putStmnt_ModI_W
         //
         void Info::putStmnt_ModI_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               putCode(Code::ModI);
               return;
            }

            putStmntCall(getCallName({IR::Code::DiXI_W, stmnt->op.size}), stmnt->op.size * 2);

            for(Core::FastU n = stmnt->op.size; n--;)
               putCode(Code::Drop_LocReg, func->localReg + n);

            for(Core::FastU n = stmnt->op.size; n--;)
               putCode(Code::Drop_Nul);

            for(Core::FastU n = 0; n != stmnt->op.size; ++n)
               putCode(Code::Push_LocReg, func->localReg + n);
         }

         //
         // Info::putStmnt_ModU_W
         //
         void Info::putStmnt_ModU_W()
         {
            if(stmnt->op.size == 0)
               return;

            putStmntCall(getCallName({IR::Code::DiXU_W, stmnt->op.size}), stmnt->op.size * 2);

            for(Core::FastU n = stmnt->op.size; n--;)
               putCode(Code::Drop_LocReg, func->localReg + n);

            for(Core::FastU n = stmnt->op.size; n--;)
               putCode(Code::Drop_Nul);

            for(Core::FastU n = 0; n != stmnt->op.size; ++n)
               putCode(Code::Push_LocReg, func->localReg + n);
         }

         //
         // Info::trStmnt_DiXI_W
         //
         void Info::trStmnt_DiXI_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size * 2, stmnt->op.size, true);
               return;
            }

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], stmnt->op.size * 2);
            }
            else
            {
               func->setLocalTmp(stmnt->op.size * 2);
               trStmntStk3(stmnt->op.size * 2, stmnt->op.size, true);
            }
         }

         //
         // Info::trStmnt_DiXU_W
         //
         void Info::trStmnt_DiXU_W()
         {
            trStmntStk3(stmnt->op.size * 2, stmnt->op.size, true);
         }

         //
         // Info::trStmnt_ModI_W
         //
         void Info::trStmnt_ModI_W()
         {
            if(stmnt->op.size != 1)
               func->setLocalTmp(stmnt->op.size);

            trStmntStk3(stmnt->op.size, stmnt->op.size, true);
         }

         //
         // Info::trStmnt_ModU_W
         //
         void Info::trStmnt_ModU_W()
         {
            func->setLocalTmp(stmnt->op.size);
            trStmntStk3(stmnt->op.size, stmnt->op.size, true);
         }
      }
   }
}

// EOF

