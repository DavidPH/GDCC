//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Multiplication statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

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
         // Info::preStmnt_MuXU_W
         //
         void Info::preStmnt_MuXU_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               preStmnt_MuXU_W1();
               return;
            }

            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, stmnt->op.size * 2,
               stmnt->op.size * 2, stmnt->op.size * 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            AS_Stmnt({IR::Code::MuXU_W, 1}, stk, lop, rop);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

            Core::FastU colEnd = stmnt->op.size * 2 - 1;
            for(Core::FastU col = 1; col != colEnd; ++col)
            {
               if(colEnd - col > 1)
                  AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

               Core::FastU sizeAdd = col < colEnd - 1 ? 3 : 2;

               Core::FastU rowEnd = std::min(col + 1, stmnt->op.size);
               lop.off = std::min(col, stmnt->op.size - 1);
               rop.off = col < stmnt->op.size ? 0 : col - stmnt->op.size + 1;
               for(; rop.off != rowEnd; --lop, ++rop)
               {
                  AS_Stmnt({IR::Code::MuXU_W, 1}, stk, lop, rop);

                  if(sizeAdd == 3)
                     AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

                  AS_Stmnt({IR::Code::AddU_W, sizeAdd}, stk, stk, stk);
               }
            }

            AS_Stmnt({IR::Code::Retn, stmnt->op.size * 2}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_MuXU_W1
         //
         void Info::preStmnt_MuXU_W1()
         {
            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, 2, 2, 5, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(1))};

            IR::Arg_LocReg lop_lo{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg lop_hi{IR::Arg_Lit(newFunc->block.getExp(1))};
            IR::Arg_LocReg rop_lo{IR::Arg_Lit(newFunc->block.getExp(2))};
            IR::Arg_LocReg rop_hi{IR::Arg_Lit(newFunc->block.getExp(3))};
            IR::Arg_LocReg tmp   {IR::Arg_Lit(newFunc->block.getExp(4))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            // Split arguments to 16-bit pieces. Take care due to aliasing.
            AS_Stmnt({IR::Code::AndU_W, 1}, rop_lo, rop, 0xFFFF);
            AS_Stmnt({IR::Code::ShRU_W, 1}, rop_hi, rop, 16);
            AS_Stmnt({IR::Code::ShRU_W, 1}, lop_hi, lop, 16);
            AS_Stmnt({IR::Code::AndU_W, 1}, lop_lo, lop, 0xFFFF);

            // Column 0.
            AS_Stmnt({IR::Code::MulU_W, 1}, stk, lop_lo, rop_lo);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

            // Column 1.
            AS_Stmnt({IR::Code::MulU_W, 1}, tmp, lop_lo, rop_hi);
            AS_Stmnt({IR::Code::ShLU_W, 1}, stk, tmp,    16);
            AS_Stmnt({IR::Code::ShRU_W, 1}, stk, tmp,    16);

            AS_Stmnt({IR::Code::MulU_W, 1}, tmp, lop_hi, rop_lo);
            AS_Stmnt({IR::Code::ShLU_W, 1}, stk, tmp,    16);
            AS_Stmnt({IR::Code::ShRU_W, 1}, stk, tmp,    16);

            // Column 2.
            AS_Stmnt({IR::Code::MulU_W, 1}, stk, lop_hi, rop_hi);
            AS_Stmnt({IR::Code::AddU_W, 1}, stk, stk,    stk);

            AS_Stmnt({IR::Code::AddU_W, 2}, stk, stk,    stk);
            AS_Stmnt({IR::Code::AddU_W, 2}, stk, stk,    stk);

            AS_Stmnt({IR::Code::Retn,   2}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_MulK_W
         //
         void Info::preStmnt_MulK_W()
         {
            if(stmnt->op.size == 0)
               return;

            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, stmnt->op.size,
               stmnt->op.size * 2, stmnt->op.size * 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size))};

            IR::Arg_Nul nul{};
            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            if(stmnt->op.size == 1)
            {
               AS_Stmnt({IR::Code::MuXU_W, 1}, stk, lop, rop);
               AS_Stmnt({IR::Code::ShRU_W, 2}, stk, stk, 16);
               AS_Stmnt({IR::Code::Move_W, 1}, nul, stk);
               AS_Stmnt({IR::Code::Retn,   1}, stk);
            }
            else
            {
               Core::FastU fracWords = stmnt->op.size / 2;
               Core::FastU mulWords  = stmnt->op.size + fracWords;

               AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, lop);
               AS_Stmnt({IR::Code::Move_W, fracWords},      stk, 0);
               AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, rop);
               AS_Stmnt({IR::Code::Move_W, fracWords},      stk, 0);

               AS_Stmnt({IR::Code::MulU_W, mulWords},       stk, stk, stk);

               AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, lop, stk);
               AS_Stmnt({IR::Code::Move_W, fracWords},      nul, stk);

               AS_Stmnt({IR::Code::Retn, stmnt->op.size},   lop);
            }

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_MulU_W
         //
         void Info::preStmnt_MulU_W()
         {
            if(stmnt->op.size <= 1)
               return;

            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, stmnt->op.size,
               stmnt->op.size * 2, stmnt->op.size * 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            AS_Stmnt({IR::Code::MuXU_W, 1}, stk, lop, rop);

            if(stmnt->op.size > 2)
               AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

            for(Core::FastU col = 1; col != stmnt->op.size; ++col)
            {
               if(stmnt->op.size - col > 3)
                  AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

               Core::FastU sizeAdd = std::min<Core::FastU>(3, stmnt->op.size - col);
               IR::Code    codeMul =
                  col < stmnt->op.size - 1 ? IR::Code::MuXU_W : IR::Code::MulU_W;

               Core::FastU rowEnd = col + 1;
               lop.off = col;
               rop.off = 0;
               for(; rop.off != rowEnd; --lop, ++rop)
               {
                  AS_Stmnt({codeMul, 1}, stk, lop, rop);

                  if(sizeAdd == 3)
                     AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

                  AS_Stmnt({IR::Code::AddU_W, sizeAdd}, stk, stk, stk);
               }
            }

            AS_Stmnt({IR::Code::Retn, stmnt->op.size}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_MulX_W
         //
         void Info::preStmnt_MulX_W()
         {
            if(stmnt->op.size <= 1)
               return;

            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, stmnt->op.size,
               stmnt->op.size * 2, stmnt->op.size * 2, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(stmnt->op.size))};

            IR::Arg_Nul nul{};
            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            Core::FastU fracWords = stmnt->op.size / 2;
            Core::FastU mulWords  = stmnt->op.size + fracWords;

            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, lop);
            AS_Stmnt({IR::Code::ShRI_W, 1}, stk, lop + (stmnt->op.size - 1), 31);
            for(Core::FastU n = fracWords - 1; n--;)
               AS_Stmnt({IR::Code::Copy_W, 1}, stk, stk);

            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, stk, rop);
            AS_Stmnt({IR::Code::ShRI_W, 1}, stk, rop + (stmnt->op.size - 1), 31);
            for(Core::FastU n = fracWords - 1; n--;)
               AS_Stmnt({IR::Code::Copy_W, 1}, stk, stk);

            AS_Stmnt({IR::Code::MulI_W, mulWords}, stk, stk, stk);

            AS_Stmnt({IR::Code::Move_W, stmnt->op.size}, lop, stk);
            AS_Stmnt({IR::Code::Move_W, fracWords},      nul, stk);

            AS_Stmnt({IR::Code::Retn, stmnt->op.size}, lop);

            #undef AS_Stmnt

            throw ResetFunc();
         }
      }
   }
}

// EOF
