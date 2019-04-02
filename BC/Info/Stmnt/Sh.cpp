//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
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
   // Info::addFunc_ShL_FW
   //
   void Info::addFunc_ShL_FW(Core::FastU n)
   {
      addFunc_ShL_FW(n, IR::CodeBase::ShL+'F', true);
   }

   //
   // Info::addFunc_ShL_FW
   //
   void Info::addFunc_ShL_FW(Core::FastU n, IR::Code code, bool left)
   {
      GDCC_BC_AddFuncPre((code, n), n, n + 1, n + 1, __FILE__);
      GDCC_BC_AddFuncObjBin(n, 1);

      FloatInfo fi = getFloatInfo(n, true);

      IR::Glyph labelEMax{prog, name + "$emax"};
      IR::Glyph labelInf {prog, name + "$inf"};

      if(!left)
         GDCC_BC_AddStmnt(Code::ShL+U, 1, rop, rop, 31 - fi.bitsExp);

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelEMax, 0, labelEMax);

      if(left)
      {
         GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, stk, 31 - fi.bitsExp);
         GDCC_BC_AddStmnt(Code::Add+U, 1, rop, rop, stk);

         GDCC_BC_AddStmnt(Code::CmpGE+I,  1, stk, rop, fi.maxExp);
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelInf);

         GDCC_BC_AddStmnt(Code::Move,  n, stk, lop);
         GDCC_BC_AddStmnt(Code::BAnd,  1, stk, stk, ~fi.maskExp);
         GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, rop, 31 - fi.bitsExp);
         GDCC_BC_AddStmnt(Code::BOrI,  1, stk, stk, stk);
      }
      else
      {
         GDCC_BC_AddStmnt(Code::CmpLE+I,  1, stk, stk, rop);
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelInf);

         GDCC_BC_AddStmnt(Code::Move,  n, stk, lop);
         GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, stk, rop);
      }

      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // Return INF or zero.
      GDCC_BC_AddLabel(labelInf);

      if(n > 1)
         GDCC_BC_AddStmnt(Code::Move, n - 1, stk, 0);

      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, lop.hi, 0x80000000);

      if(left)
         GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskExp);

      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // If lop is INF, NaN, or zero, return as-is.
      GDCC_BC_AddLabel(labelEMax);

      GDCC_BC_AddStmnt(Code::Retn, n, lop);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_ShL_UW
   //
   void Info::addFunc_ShL_UW(Core::FastU n)
   {
      addFunc_ShL_UW(n, IR::CodeBase::ShL+'U', true, false);
   }

   //
   // Info::addFunc_ShL_UW
   //
   void Info::addFunc_ShL_UW(Core::FastU n, IR::Code code, bool left, bool sign)
   {
      GDCC_BC_AddFuncPre((code, n), n, n + 1, n + 1, __FILE__);
      GDCC_BC_AddFuncObjBin(n, 1);

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
            GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, lop.hi, 31);

            for(Core::FastU i = words - 1; i--;)
               GDCC_BC_AddStmnt(Code::Copy, 1, stk, stk);
         }
         else
            GDCC_BC_AddStmnt(Code::Move, words, stk, 0);
      };

      // Calculate shiftWords.
      GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, rop, 5);

      // Calculate shiftBits
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, rop, 31);

      // If shiftBits is 0, branch to whole word shift table.
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, 0, labelTab0);

      // Otherwise, store shiftBits and branch on shiftWords.
      GDCC_BC_AddStmnt(Code::Move, 1, rop, stk);

      // Partial word shift jump table.
      {
         Core::Array<IR::Arg> args{n * 2 + 1};
         args[0] = IR::Arg_Stk(w);

         for(Core::FastU i = n; i--;)
         {
            args[i * 2 + 1] = GDCC_BC_ArgLit(1, i);
            args[i * 2 + 2] = GDCC_BC_ArgLit(1, labelW[i]);
         }

         GDCC_BC_AddStmntArgs(Code::Jcnd_Tab, 1, std::move(args));
      }

      // Whole word shift jump table.
      GDCC_BC_AddLabel(labelTab0);
      {
         Core::Array<IR::Arg> args{n * 2 + 1};
         args[0] = IR::Arg_Stk(w);

         for(Core::FastU i = n; i--;)
         {
            args[i * 2 + 1] = GDCC_BC_ArgLit(1, i);
            args[i * 2 + 2] = GDCC_BC_ArgLit(1, label0[i]);
         }

         GDCC_BC_AddStmntArgs(Code::Jcnd_Tab, 1, std::move(args));
      }

      // Emergency fallback, return 0.
      GDCC_BC_AddStmnt(Code::Move, 1, nul, stk);
      fillZeroes(n);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // Generate shift cases.
      for(Core::FastU shiftWords = 0; shiftWords != n; ++shiftWords)
      {
         Core::FastU keepWords = n - shiftWords;

         // Generate partial word shift.
         GDCC_BC_AddLabel(labelW[shiftWords]);

         if(left)
         {
            fillZeroes(shiftWords);

            GDCC_BC_AddStmnt(code, 1, stk, lop[0], rop);

            for(Core::FastU i = 1; i != keepWords; ++i)
            {
               GDCC_BC_AddStmnt(Code::Move,  1, stk, lop[i - 1]);
               GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, 32, rop);
               GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, stk, stk);

               GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, lop[i], rop);

               GDCC_BC_AddStmnt(Code::BOrI,  1, stk, stk, stk);
            }
         }
         else
         {
            for(Core::FastU i = 0; i != keepWords - 1;)
            {
               GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, lop[shiftWords + i], rop);

               GDCC_BC_AddStmnt(Code::Move,  1, stk, lop[shiftWords + ++i]);
               GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, 32, rop);
               GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, stk, stk);

               GDCC_BC_AddStmnt(Code::BOrI,  1, stk, stk, stk);
            }

            GDCC_BC_AddStmnt(code, 1, stk, lop.hi, rop);

            fillZeroes(shiftWords);
         }

         GDCC_BC_AddStmnt(Code::Retn, n, stk);

         // Generate full word shift.
         GDCC_BC_AddLabel(label0[shiftWords]);

         if(left)
         {
            fillZeroes(shiftWords);
            for(Core::FastU i = 0; i != keepWords; ++i)
               GDCC_BC_AddStmnt(Code::Move, 1, stk, lop[i]);
         }
         else
         {
            for(Core::FastU i = 0; i != keepWords; ++i)
               GDCC_BC_AddStmnt(Code::Move, 1, stk, lop[shiftWords + i]);
            fillZeroes(shiftWords);
         }

         GDCC_BC_AddStmnt(Code::Retn, n, stk);
      }

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_ShR_FW
   //
   void Info::addFunc_ShR_FW(Core::FastU n)
   {
      addFunc_ShL_FW(n, IR::CodeBase::ShR+'F', false);
   }

   //
   // Info::addFunc_ShR_IW
   //
   void Info::addFunc_ShR_IW(Core::FastU n)
   {
      addFunc_ShL_UW(n, IR::CodeBase::ShR+'I', false, true);
   }

   //
   // Info::addFunc_ShR_UW
   //
   void Info::addFunc_ShR_UW(Core::FastU n)
   {
      addFunc_ShL_UW(n, IR::CodeBase::ShR+'U', false, false);
   }
}

// EOF

