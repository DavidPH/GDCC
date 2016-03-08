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

            Core::String name = getCallName({IR::Code::DiXI_W, stmnt->op.size});
            auto newFunc = preStmntCallDef(name, stmnt->op.size * 2,
               stmnt->op.size * 2, stmnt->op.size * 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Glyph labelL0R1{prog, name + "$l0r1"};
            IR::Glyph labelL1  {prog, name + "$l1"};
            IR::Glyph labelL1R1{prog, name + "$l1r1"};

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            AS_Stmnt({IR::Code::AndU_W,   1}, stk, lop + (stmnt->op.size - 1), 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Tru, 1}, stk, labelL1);

            AS_Stmnt({IR::Code::AndU_W,   1}, stk, rop + (stmnt->op.size - 1), 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Tru, 1}, stk, labelL0R1);

            // +dividend, +divisor -> +quotient, +remainder
            AS_Stmnt({IR::Code::DiXU_W, stmnt->op.size},     stk, lop, rop);
            AS_Stmnt({IR::Code::Retn,   stmnt->op.size * 2}, stk);

            newFunc->block.addLabel(labelL0R1);
            AS_Stmnt({IR::Code::NegI_W, stmnt->op.size}, rop, rop);

            // +dividend, -divisor -> -quotient, -remainder
            AS_Stmnt({IR::Code::DiXU_W, stmnt->op.size},     stk, lop, rop);
            AS_Stmnt({IR::Code::NegI_W, stmnt->op.size},     rop, stk);
            AS_Stmnt({IR::Code::NegI_W, stmnt->op.size},     stk, stk);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size},     stk, rop);
            AS_Stmnt({IR::Code::Retn,   stmnt->op.size * 2}, stk);

            newFunc->block.addLabel(labelL1);
            AS_Stmnt({IR::Code::NegI_W, stmnt->op.size}, lop, lop);

            AS_Stmnt({IR::Code::AndU_W,   1}, stk, rop + (stmnt->op.size - 1), 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Tru, 1}, stk, labelL1R1);

            // -dividend, +divisor -> -quotient, +remainder
            AS_Stmnt({IR::Code::DiXU_W, stmnt->op.size},     stk, lop, rop);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size},     rop, stk);
            AS_Stmnt({IR::Code::NegI_W, stmnt->op.size},     stk, stk);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size},     stk, rop);
            AS_Stmnt({IR::Code::Retn,   stmnt->op.size * 2}, stk);

            newFunc->block.addLabel(labelL1R1);
            AS_Stmnt({IR::Code::NegI_W, stmnt->op.size}, rop, rop);

            // -dividend, -divisor -> +quotient, -remainder
            AS_Stmnt({IR::Code::DiXU_W, stmnt->op.size},     stk, lop, rop);
            AS_Stmnt({IR::Code::NegI_W, stmnt->op.size},     stk, stk);
            AS_Stmnt({IR::Code::Retn,   stmnt->op.size * 2}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_DiXU_W
         //
         void Info::preStmnt_DiXU_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               preStmnt_DiXU_W1();
               return;
            }

            Core::String name = getCallName({IR::Code::DiXU_W, stmnt->op.size});
            auto newFunc = preStmntCallDef(name, stmnt->op.size * 2,
               stmnt->op.size * 2, stmnt->op.size * 5, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Glyph labelFull    {prog, name + "$full"};
            IR::Glyph labelLoopBody{prog, name + "$loop_body"};
            IR::Glyph labelLoopCond{prog, name + "$loop_cond"};
            IR::Glyph labelLoopShft{prog, name + "$loop_shft"};
            IR::Glyph labelRetn    {prog, name + "$retn"};

            IR::Arg_LocReg lop {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 0))};
            IR::Arg_LocReg rop {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 1))};
            IR::Arg_LocReg quot{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 2))};
            IR::Arg_LocReg rem {IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3))};
            IR::Arg_LocReg mask{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 4))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            // If both operands have the high word clear, defer to smaller op.
            AS_Stmnt({IR::Code::OrIU_W,   1}, stk,
               lop + (stmnt->op.size - 1), rop + (stmnt->op.size - 1));
            AS_Stmnt({IR::Code::Jcnd_Tru, 1}, stk, labelFull);

            AS_Stmnt({IR::Code::DiXU_W, stmnt->op.size - 1}, stk, lop, rop);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size - 1}, rop, stk);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size - 1}, lop, stk);

            AS_Stmnt({IR::Code::Retn, stmnt->op.size * 2}, lop);

            newFunc->block.addLabel(labelFull);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, quot, 0);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, rem,  lop);

            // If lop < rop, return now.
            AS_Stmnt({IR::Code::CmpU_LT_W, stmnt->op.size}, stk, lop, rop);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1},              stk, labelRetn);

            // Calculate mask and adjust divisor.
            AS_Stmnt({IR::Code::Bclz_W, stmnt->op.size}, stk, rop);
            AS_Stmnt({IR::Code::Bclz_W, stmnt->op.size}, stk, lop);
            AS_Stmnt({IR::Code::SubU_W, 1}, mask, stk, stk);

            AS_Stmnt({IR::Code::ShLU_W, stmnt->op.size}, rop,  rop, mask);
            AS_Stmnt({IR::Code::ShLU_W, stmnt->op.size}, mask, 1,   mask);

            // Division loop.
            AS_Stmnt({IR::Code::Jump, 1}, labelLoopCond);

            newFunc->block.addLabel(labelLoopBody);
            AS_Stmnt({IR::Code::CmpU_GE_W, stmnt->op.size}, stk, rem, rop);
            AS_Stmnt({IR::Code::Jcnd_Nil,  1}, stk, labelLoopShft);

            AS_Stmnt({IR::Code::OrIU_W, stmnt->op.size}, quot, quot, mask);
            AS_Stmnt({IR::Code::SubU_W, stmnt->op.size}, rem,  rem,  rop);

            newFunc->block.addLabel(labelLoopShft);
            AS_Stmnt({IR::Code::ShRU_W, stmnt->op.size}, rop,  rop,  1);
            AS_Stmnt({IR::Code::ShRU_W, stmnt->op.size}, mask, mask, 1);

            newFunc->block.addLabel(labelLoopCond);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, mask);
            for(Core::FastU n = stmnt->op.size; --n;)
               AS_Stmnt({IR::Code::OrIU_W, 1}, stk, stk, stk);
            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, rem);
            for(Core::FastU n = stmnt->op.size; --n;)
               AS_Stmnt({IR::Code::OrIU_W, 1}, stk, stk, stk);
            AS_Stmnt({IR::Code::LAnd, 1}, stk, stk, stk);
            AS_Stmnt({IR::Code::Jcnd_Tru, 1}, stk, labelLoopBody);

            newFunc->block.addLabel(labelRetn);
            AS_Stmnt({IR::Code::Retn, stmnt->op.size * 2}, quot);

            #undef AS_Stmnt

            throw ResetFunc();
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

            AS_Stmnt({IR::Code::AndU_W,   1}, stk, lop, 0x80000000);
            AS_Stmnt({IR::Code::Jcnd_Nil, 1}, stk, labelL0);

            // l has high bit set.
            AS_Stmnt({IR::Code::AndU_W,   1}, stk, rop, 0x80000000);
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
            AS_Stmnt({IR::Code::AndU_W,   1}, stk, rop, 1);
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
            AS_Stmnt({IR::Code::AndU_W,   1}, stk, rop, 0x80000000);
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
            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, stmnt->op.size,
               stmnt->op.size * 2, stmnt->op.size * 3 + 3, __FILE__, __LINE__);

            if(!newFunc)
               return;

            FloatInfo fi = GetFloatInfo(stmnt->op.size);

            IR::Glyph labelL0  {prog, name + "$l0"};
            IR::Glyph labelLINF{prog, name + "$linf"};
            IR::Glyph labelLNaN{prog, name + "$lnan"};
            IR::Glyph labelR0  {prog, name + "$r0"};
            IR::Glyph labelRINF{prog, name + "$rinf"};
            IR::Glyph labelRNaN{prog, name + "$rnan"};

            IR::Glyph labelLoop   {prog, name + "$loop"};
            IR::Glyph labelLoopNil{prog, name + "$loopnil"};

            IR::Glyph label0  {prog, name + "$0"};
            IR::Glyph labelINF{prog, name + "$inf"};
            IR::Glyph labelNaN{prog, name + "$nan"};

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 0))};
            IR::Arg_LocReg lhi{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 1 - 1))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 1))};
            IR::Arg_LocReg rhi{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 2 - 1))};
            IR::Arg_LocReg man{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 2))};
            IR::Arg_LocReg mhi{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3 - 1))};
            IR::Arg_LocReg sig{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3 + 0))};
            IR::Arg_LocReg exp{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3 + 1))};
            IR::Arg_LocReg tmp{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size * 3 + 2))};

            IR::Arg_Nul nul{};
            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            // Determine result sign.
            AS_Stmnt({IR::Code::OrXU_W,    1}, stk, lhi, rhi);
            AS_Stmnt({IR::Code::AndU_W,    1}, sig, stk, 0x80000000);

            // Clear operand signs.
            AS_Stmnt({IR::Code::AndU_W,    1}, lhi, lhi, 0x7FFFFFFF);
            AS_Stmnt({IR::Code::AndU_W,    1}, rhi, rhi, 0x7FFFFFFF);

            // Check for special operands.
            AS_Stmnt({IR::Code::CmpI_GT_W, 1}, stk, lhi, fi.maskExp);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelLNaN);
            AS_Stmnt({IR::Code::Move_W,    1}, stk, rhi);
            AS_Stmnt({IR::Code::Jcnd_Tab,  1}, stk, 0, labelR0, fi.maskExp, labelRINF);
            AS_Stmnt({IR::Code::CmpI_GT_W, 1}, stk, stk, fi.maskExp);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelRNaN);
            AS_Stmnt({IR::Code::Move_W,    1}, stk, lhi);
            AS_Stmnt({IR::Code::Jcnd_Tab,  1}, stk, 0, labelL0, fi.maskExp, labelLINF);
            AS_Stmnt({IR::Code::Move_W,    1}, nul, stk);

            // Determine result exponent. Will be adjusted, so no range check.
            AS_Stmnt({IR::Code::Move_W,    1}, stk, fi.offExp);
            AS_Stmnt({IR::Code::ShRI_W,    1}, stk, rhi, fi.bitsMan);
            AS_Stmnt({IR::Code::SubU_W,    1}, stk, stk, stk);
            AS_Stmnt({IR::Code::ShRI_W,    1}, stk, lhi, fi.bitsMan);
            AS_Stmnt({IR::Code::AddU_W,    1}, exp, stk, stk);

            // Clear operand exponents and add implicit bit.
            AS_Stmnt({IR::Code::AndU_W,    1}, stk, lhi, fi.maskMan);
            AS_Stmnt({IR::Code::OrIU_W,    1}, lhi, stk, fi.maskMan + 1);
            AS_Stmnt({IR::Code::AndU_W,    1}, stk, rhi, fi.maskMan);
            AS_Stmnt({IR::Code::OrIU_W,    1}, rhi, stk, fi.maskMan + 1);

            // Division loop.
            AS_Stmnt({IR::Code::Move_W,    stmnt->op.size}, man, 0);
            AS_Stmnt({IR::Code::Move_W,    1},              tmp, fi.bitsManFull);

            // Division check.
            newFunc->block.addLabel(labelLoop);
            AS_Stmnt({IR::Code::CmpI_GE_W, stmnt->op.size}, stk, lop, rop);
            AS_Stmnt({IR::Code::Jcnd_Nil,  1},              stk, labelLoopNil);
            AS_Stmnt({IR::Code::SubU_W,    stmnt->op.size}, lop, lop, rop);
            AS_Stmnt({IR::Code::AddU_W,    1},              man, man, 1);

            newFunc->block.addLabel(labelLoopNil);
            AS_Stmnt({IR::Code::ShLU_W,    stmnt->op.size}, lop, lop, 1);
            AS_Stmnt({IR::Code::ShLU_W,    stmnt->op.size}, man, man, 1);

            AS_Stmnt({IR::Code::SubU_W,    1},              tmp, tmp, 1);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1},              tmp, labelLoop);

            // Final division check.
            AS_Stmnt({IR::Code::CmpI_GE_W, stmnt->op.size}, stk, lop, rop);
            AS_Stmnt({IR::Code::AddU_W,    1},              man, man, stk);

            // Shift 1 into implicit bit.
            AS_Stmnt({IR::Code::Bclz_W,    stmnt->op.size}, tmp, man);
            AS_Stmnt({IR::Code::SubU_W,    1},              tmp, tmp, fi.bitsExp);

            AS_Stmnt({IR::Code::ShLU_W,    stmnt->op.size}, man, man, tmp);
            AS_Stmnt({IR::Code::SubU_W,    1},              exp, exp, tmp);

            AS_Stmnt({IR::Code::AndU_W,    1},              mhi, mhi, fi.maskMan);

            // Check for out of range exponent.
            AS_Stmnt({IR::Code::CmpI_GE_W, 1},              stk, exp, fi.maxExp);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1},              stk, labelINF);
            AS_Stmnt({IR::Code::CmpI_LE_W, 1},              stk, exp, 0);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1},              stk, label0);

            // Return result.
            AS_Stmnt({IR::Code::Move_W,    stmnt->op.size}, stk, man);
            AS_Stmnt({IR::Code::ShLU_W,    1},              stk, exp, fi.bitsMan);
            AS_Stmnt({IR::Code::OrIU_W,    1},              stk, stk, stk);
            AS_Stmnt({IR::Code::OrIU_W,    1},              stk, stk, sig);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            // Return NaN.
            newFunc->block.addLabel(labelNaN);
            for(auto n = stmnt->op.size - 1; n--;)
               AS_Stmnt({IR::Code::Move_W, 1},              stk, 0xFFFFFFFF);
            AS_Stmnt({IR::Code::OrIU_W,    1},              stk, sig, fi.maskExp | fi.maskMan);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            // l is NaN. Therefore, result is l.
            newFunc->block.addLabel(labelLNaN);
            // l is 0, r is normal. Therefore, result is l.
            newFunc->block.addLabel(labelL0);
            // l is INF, r is normal. Therefore, result is l.
            newFunc->block.addLabel(labelLINF);
            AS_Stmnt({IR::Code::Move_W,    stmnt->op.size}, stk, lop);
            AS_Stmnt({IR::Code::OrIU_W,    1},              stk, stk, sig);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            // r is NaN. Therefore, result is r.
            newFunc->block.addLabel(labelRNaN);
            AS_Stmnt({IR::Code::Move_W,    stmnt->op.size},              stk, rop);
            AS_Stmnt({IR::Code::OrIU_W,    1},              stk, stk, sig);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            // r is 0.
            newFunc->block.addLabel(labelR0);

            // 0 / 0 = NaN.
            AS_Stmnt({IR::Code::Jcnd_Nil, stmnt->op.size}, lop, labelNaN);

            // Otherwise, result is INF.
            newFunc->block.addLabel(labelINF);
            for(auto n = stmnt->op.size - 1; n--;)
               AS_Stmnt({IR::Code::Move_W, 1},              stk, 0);
            AS_Stmnt({IR::Code::OrIU_W,    1},              stk, sig, fi.maskExp);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            // r is INF.
            newFunc->block.addLabel(labelRINF);

            // INF / INF = NaN.
            AS_Stmnt({IR::Code::CmpU_EQ_W, 1},              stk, lhi, fi.maskMan);
            AS_Stmnt({IR::Code::Jcnd_Tru,  1},              stk, labelNaN);

            // Otherwie result is 0.
            newFunc->block.addLabel(label0);
            for(auto n = stmnt->op.size - 1; n--;)
               AS_Stmnt({IR::Code::Move_W, 1},              stk, 0);
            AS_Stmnt({IR::Code::Move_W,    1},              stk, sig);
            AS_Stmnt({IR::Code::Retn,      stmnt->op.size}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
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

