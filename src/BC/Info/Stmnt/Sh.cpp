//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Shift statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_ShLU_W
   //
   void Info::addFunc_ShLU_W(Core::FastU n)
   {
      addFunc_ShLU_W(n, IR::Code::ShLU_W, true, false);
   }

   //
   // Info::addFunc_ShLU_W
   //
   void Info::addFunc_ShLU_W(Core::FastU n, IR::Code code, bool left, bool sign)
   {
      GDCC_BC_AddFuncPre({code, n}, n, n + 1, n + 1, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      // Generate labels.

      Core::Array<IR::Glyph> label0{n};
      Core::Array<IR::Glyph> labelW{n};

      for(Core::FastU i = n; i--;)
      {
         std::ostringstream labelBuf;

         std::string label = (labelBuf << name << "$w" << i, labelBuf).str();
         labelW[i] = {prog, {label.data(), label.size()}};

         label = (labelBuf << "_0", labelBuf).str();
         label0[i] = {prog, {label.data(), label.size()}};
      }

      IR::Glyph labelTab0{prog, name + "$tab0"};

      //
      // fillZeroes
      //
      auto fillZeroes = [&](Core::FastU words)
      {
         if(!words) return;

         if(sign)
         {
            lop.off = n - 1;
            GDCC_BC_AddStmnt({Code::ShRI_W, 1}, stk, lop, 31);

            for(Core::FastU i = words - 1; i--;)
               GDCC_BC_AddStmnt({Code::Copy_W, 1}, stk, stk);
         }
         else
            GDCC_BC_AddStmnt({Code::Move_W, words}, stk, 0);
      };

      // Calculate shiftWords.
      GDCC_BC_AddStmnt({Code::ShRI_W, 1}, stk, rop, 5);

      // Calculate shiftBits
      GDCC_BC_AddStmnt({Code::BAnd_W, 1}, stk, rop, 31);

      // If shiftBits is 0, branch to whole word shift table.
      GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, stk, 0, labelTab0);

      // Otherwise, store shiftBits and branch on shiftWords.
      GDCC_BC_AddStmnt({Code::Move_W, 1}, rop, stk);

      // Partial word shift jump table.
      {
         Core::Array<IR::Arg> args{n * 2 + 1};
         args[0] = stk;

         for(Core::FastU i = n; i--;)
         {
            args[i * 2 + 1] = GDCC_BC_ArgLit(i);
            args[i * 2 + 2] = GDCC_BC_ArgLit(labelW[i]);
         }

         GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, std::move(args));
      }

      // Whole word shift jump table.
      newFunc->block.addLabel(labelTab0);
      {
         Core::Array<IR::Arg> args{n * 2 + 1};
         args[0] = stk;

         for(Core::FastU i = n; i--;)
         {
            args[i * 2 + 1] = GDCC_BC_ArgLit(i);
            args[i * 2 + 2] = GDCC_BC_ArgLit(label0[i]);
         }

         GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, std::move(args));
      }

      // Emergency fallback, return 0.
      GDCC_BC_AddStmnt({Code::Move_W, 1}, nul, stk);
      fillZeroes(n);
      GDCC_BC_AddStmnt({Code::Retn, n}, stk);

      // Generate shift cases.
      for(Core::FastU shiftWords = 0; shiftWords != n; ++shiftWords)
      {
         Core::FastU keepWords = n - shiftWords;

         // Generate partial word shift.
         newFunc->block.addLabel(labelW[shiftWords]);

         if(left)
         {
            lop.off = 0;

            fillZeroes(shiftWords);

            GDCC_BC_AddStmnt({code, 1}, stk, lop, rop);

            for(Core::FastU i = 1; i != keepWords; ++i)
            {
               GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, lop);
               GDCC_BC_AddStmnt({Code::SubU_W, 1}, stk, 32, rop);
               GDCC_BC_AddStmnt({Code::ShRU_W, 1}, stk, stk, stk);

               GDCC_BC_AddStmnt({Code::ShLU_W, 1}, stk, ++lop, rop);

               GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, stk);
            }
         }
         else
         {
            lop.off = shiftWords;

            for(Core::FastU i = 0; i != keepWords - 1; ++i)
            {
               GDCC_BC_AddStmnt({Code::ShRU_W, 1}, stk, lop, rop);

               GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, ++lop);
               GDCC_BC_AddStmnt({Code::SubU_W, 1}, stk, 32, rop);
               GDCC_BC_AddStmnt({Code::ShLU_W, 1}, stk, stk, stk);

               GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, stk);
            }

            GDCC_BC_AddStmnt({code, 1}, stk, lop, rop);

            fillZeroes(shiftWords);
         }

         GDCC_BC_AddStmnt({Code::Retn, n}, stk);

         // Generate full word shift.
         newFunc->block.addLabel(label0[shiftWords]);

         if(left)
         {
            lop.off = 0;
            fillZeroes(shiftWords);
            GDCC_BC_AddStmnt({Code::Move_W, keepWords}, stk, lop);
         }
         else
         {
            lop.off = shiftWords;
            GDCC_BC_AddStmnt({Code::Move_W, keepWords}, stk, lop);
            fillZeroes(shiftWords);
         }

         GDCC_BC_AddStmnt({Code::Retn, n}, stk);
      }

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_ShRI_W
   //
   void Info::addFunc_ShRI_W(Core::FastU n)
   {
      addFunc_ShLU_W(n, IR::Code::ShRI_W, false, true);
   }

   //
   // Info::addFunc_ShRU_W
   //
   void Info::addFunc_ShRU_W(Core::FastU n)
   {
      addFunc_ShLU_W(n, IR::Code::ShRU_W, false, false);
   }
}

// EOF

