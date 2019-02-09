//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Bitwise statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Exception.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::addFunc_Bclz_W1
   //
   void Info::addFunc_Bclz_W1(bool ones)
   {
      GDCC_BC_AddFuncPre(ones ? Code::Bclo : Code::Bclz, 1, 1, 1, 1, __FILE__);
      GDCC_BC_AddFuncObjUna(1);

      // Counting leading ones works the same way if inverted.
      // May be advantageous to invert the conditions, instead, though.
      if(ones)
         GDCC_BC_AddStmnt(Code::BNot, 1, lop, lop);

      // Node codegen, mask-and-branch.
      auto node = [&](char const *label, Core::FastU mask, char const *dst)
      {
         if(label) GDCC_BC_AddLabel(name + label);
         IR::Glyph dstGlyph{prog, name + dst};
         GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop, mask);
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, dstGlyph);
      };

      // Leaf codegen, return literal.
      auto leaf = [&](char const *label, Core::FastU ret)
      {
         if(label) GDCC_BC_AddLabel(name + label);
         GDCC_BC_AddStmnt(Code::Retn, 1, ret);
      };

      // Binary search tree to find first set bit.
      node(nullptr,     0xFFFF0000, "$FFFF0000");
      node(nullptr,     0x0000FF00, "$0000FF00");
      node(nullptr,     0x000000F0, "$000000F0");
      node(nullptr,     0x0000000C, "$0000000C");
      node(nullptr,     0x00000002, "$00000002");
      GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, 32, lop);
      GDCC_BC_AddStmnt(Code::Retn,  1, stk);
      leaf("$00000002",         30);
      node("$0000000C", 0x00000008, "$00000008");
      leaf(nullptr,             29);
      leaf("$00000008",         28);
      node("$000000F0", 0x000000C0, "$000000C0");
      node(nullptr,     0x00000020, "$00000020");
      leaf(nullptr,             27);
      leaf("$00000020",         26);
      node("$000000C0", 0x00000080, "$00000080");
      leaf(nullptr,             25);
      leaf("$00000080",         24);
      node("$0000FF00", 0x0000F000, "$0000F000");
      node(nullptr,     0x00000C00, "$00000C00");
      node(nullptr,     0x00000200, "$00000200");
      leaf(nullptr,             23);
      leaf("$00000200",         22);
      node("$00000C00", 0x00000800, "$00000800");
      leaf(nullptr,             21);
      leaf("$00000800",         20);
      node("$0000F000", 0x0000C000, "$0000C000");
      node(nullptr,     0x00002000, "$00002000");
      leaf(nullptr,             19);
      leaf("$00002000",         18);
      node("$0000C000", 0x00008000, "$00008000");
      leaf(nullptr,             17);
      leaf("$00008000",         16);
      node("$FFFF0000", 0xFF000000, "$FF000000");
      node(nullptr,     0x00F00000, "$00F00000");
      node(nullptr,     0x000C0000, "$000C0000");
      node(nullptr,     0x00020000, "$00020000");
      leaf(nullptr,             15);
      leaf("$00020000",         14);
      node("$000C0000", 0x00080000, "$00080000");
      leaf(nullptr,             13);
      leaf("$00080000",         12);
      node("$00F00000", 0x00C00000, "$00C00000");
      node(nullptr,     0x00200000, "$00200000");
      leaf(nullptr,             11);
      leaf("$00200000",         10);
      node("$00C00000", 0x00800000, "$00800000");
      leaf(nullptr,              9);
      leaf("$00800000",          8);
      node("$FF000000", 0xF0000000, "$F0000000");
      node(nullptr,     0x0C000000, "$0C000000");
      node(nullptr,     0x02000000, "$02000000");
      leaf(nullptr,              7);
      leaf("$02000000",          6);
      node("$0C000000", 0x08000000, "$08000000");
      leaf(nullptr,              5);
      leaf("$08000000",          4);
      node("$F0000000", 0xC0000000, "$C0000000");
      node(nullptr,     0x20000000, "$20000000");
      leaf(nullptr,              3);
      leaf("$20000000",          2);
      node("$C0000000", 0x80000000, "$80000000");
      leaf(nullptr,              1);
      leaf("$80000000",          0);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::chkStmnt_Bget
   //
   void Info::chkStmnt_Bget()
   {
      chkStmntArgB(2, IR::ArgBase::Lit);
      chkStmntArgB(3, IR::ArgBase::Lit);

      if(getStmntSize() != 1)
         IR::ErrorCode(stmnt, "unsupported size");
   }

   //
   // Info::genStmnt_BAnd
   //
   void Info::genStmnt_BAnd()
   {
      auto n = getStmntSize();

      if(stmnt->args[1].a == IR::ArgBase::Stk &&
         stmnt->args[2].a == IR::ArgBase::Stk)
      {
         if(n > 1)
            numChunkCODE += (n * 4 - 2) * 8;
         numChunkCODE += n * 4;
      }
      else
      {
         numChunkCODE += lenPushArg(stmnt->args[1], 0, n);
         numChunkCODE += lenPushArg(stmnt->args[2], 0, n);
         numChunkCODE += n * 4;
      }
   }

   //
   // Info::genStmnt_BNot
   //
   void Info::genStmnt_BNot()
   {
      auto n = getStmntSize();

      if(isPushArg(stmnt->args[1]))
      {
         numChunkCODE += lenPushArg(stmnt->args[1], 0, n);
         numChunkCODE += n * 4;
      }
      else if(isDropArg(stmnt->args[0]))
      {
         numChunkCODE += lenDropArg(stmnt->args[0], 0, n);
         numChunkCODE += n * 4;
      }
      else
      {
         numChunkCODE += n * 4;

         if(n > 1)
            numChunkCODE += 8;

         if(n > 2)
            numChunkCODE += (n - 2) * 16;
      }
   }

   //
   // Info::genStmnt_Bclz
   //
   void Info::genStmnt_Bclz()
   {
      auto n = getStmntSize();

      if(n == 0)
      {
         numChunkCODE += 8;
         return;
      }

      if(n == 1)
      {
         genStmntCall(1);
         return;
      }

      if(stmnt->args[1].a != IR::ArgBase::Stk)
      {
         numChunkCODE += 28 + (n - 1) * 40 + 8
            + lenPushArg(stmnt->args[1], 0, n);
      }
      else if(stmnt->args[0].a != IR::ArgBase::Stk)
      {
         numChunkCODE += 28 + (n - 1) * 40 + 8
            + lenDropArg(stmnt->args[0], 0) * (n + 1);

         for(Core::FastU i = n; --i;)
            numChunkCODE += i * 4;
      }
      else
      {
         numChunkCODE += 36 + (n - 1) * 48 + 24;

         for(Core::FastU i = n; --i;)
            numChunkCODE += i * 4;
      }
   }

   //
   // Info::genStmnt_Bges
   //
   void Info::genStmnt_Bges()
   {
      numChunkCODE += 24;
   }

   //
   // Info::genStmnt_Bget
   //
   void Info::genStmnt_Bget()
   {
      auto offs = getWord(stmnt->args[3].aLit);

      if(offs)
         numChunkCODE += 12;

      numChunkCODE += 12;
   }

   //
   // Info::genStmnt_Bset
   //
   void Info::genStmnt_Bset()
   {
      auto offs = getWord(stmnt->args[3].aLit);

      if(offs)
         numChunkCODE += 12;

      numChunkCODE += 12 + lenPushArg(stmnt->args[0], 0) + 16 + lenDropArg(stmnt->args[0], 0);
   }

   //
   // Info::preStmnt_Bclz
   //
   void Info::preStmnt_Bclz(bool ones)
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      addFunc_Bclz_W1(ones);
   }

   //
   // Info::putStmnt_BAnd
   //
   void Info::putStmnt_BAnd(Code code)
   {
      auto n = getStmntSize();

      if(stmnt->args[1].a == IR::ArgBase::Stk &&
         stmnt->args[2].a == IR::ArgBase::Stk)
      {
         if(n > 1)
         {
            for(Core::FastU i = 0, e = n * 2 - 1; i != e; ++i)
               putCode(Code::Drop_LocReg, func->localReg + i);

            putCode(Code::Push_LocReg, func->localReg + n - 1);
         }

         putCode(code);

         if(n > 1) for(Core::FastU i = n - 1; i--;)
         {
            putCode(Code::Push_LocReg, func->localReg + n + i);
            putCode(Code::Push_LocReg, func->localReg + i);
            putCode(code);
         }
      }
      else
      {
         for(Core::FastU i = 0; i != n; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putStmntPushArg(stmnt->args[2], i);
            putCode(code);
         }
      }
   }

   //
   // Info::putStmnt_BNot
   //
   void Info::putStmnt_BNot()
   {
      auto n = getStmntSize();

      if(isPushArg(stmnt->args[1]))
      {
         for(Core::FastU i = 0; i != n; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putCode(Code::BNot);
         }
      }
      else if(isDropArg(stmnt->args[0]))
      {
         if(auto i = n) while(i--)
         {
            putCode(Code::BNot);
            putStmntDropArg(stmnt->args[0], i);
         }
      }
      else
      {
         if(n > 2)
         {
            for(Core::FastU i = n - 2; i--;)
            {
               putCode(Code::BNot);
               putCode(Code::Drop_LocReg, func->localReg + i);
            }
         }

         if(n > 0)
            putCode(Code::BNot);

         if(n > 1)
         {
            putCode(Code::Swap);
            putCode(Code::BNot);
            putCode(Code::Swap);
         }

         if(n > 2)
         {
            for(Core::FastU i = 0, e = n - 2; i != e; ++i)
               putCode(Code::Push_LocReg, func->localReg + i);
         }
      }
   }

   //
   // Info::putStmnt_Bclz
   //
   void Info::putStmnt_Bclz(bool ones)
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Push_Lit, 0);

      Core::String name = getFuncName(ones ? IR::CodeBase::Bclo : IR::CodeBase::Bclz, 1);

      if(n == 1)
         return putStmntCall(name, 1);

      Core::FastU skip = ones ? 0xFFFFFFFF : 0;
      Core::FastU fIdx = getWord(resolveGlyph(name));

      if(stmnt->args[1].a != IR::ArgBase::Stk)
      {
         Core::FastU i = n - 1;

         putStmntPushArg(stmnt->args[1], i);
         putCode(Code::Jcnd_Lit, skip, putPos + 28);
         putCode(Code::Call_Lit, fIdx);
         putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8
            + lenPushArg(stmnt->args[1], 0, i));

         while(i--)
         {
            putStmntPushArg(stmnt->args[1], i);
            putCode(Code::Jcnd_Lit, skip, putPos + 40);
            putCode(Code::Call_Lit, fIdx);
            putCode(Code::Push_Lit, (n - 1 - i) * 32);
            putCode(Code::AddU);
            putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8
               + lenPushArg(stmnt->args[1], 0, i));
         }

         putCode(Code::Push_Lit, n * 32);
      }
      else if(stmnt->args[0].a != IR::ArgBase::Stk)
      {
         Core::FastU i = n - 1;

         Core::FastU dropLen = 0;
         for(auto j = i; --j;) dropLen += j * 4;

         putCode(Code::Jcnd_Lit, skip, putPos + 28 + i * 4
               + lenDropArg(stmnt->args[1], 0));
         putCode(Code::Call_Lit, fIdx);
         putStmntDropArg(stmnt->args[1], 0);
         for(auto j = i; j--;) putCode(Code::Drop_Nul);
         putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
            + lenDropArg(stmnt->args[1], 0) * (i + 1));

         while(i--)
         {
            dropLen -= i * 4;

            putCode(Code::Jcnd_Lit, skip, putPos + 40 + i * 4
               + lenDropArg(stmnt->args[1], 0));
            putCode(Code::Call_Lit, fIdx);
            putCode(Code::Push_Lit, (n - 1 - i) * 32);
            putCode(Code::AddU);
            putStmntDropArg(stmnt->args[1], 0);
            for(auto j = i; j--;) putCode(Code::Drop_Nul);
            putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
               + lenDropArg(stmnt->args[1], 0) * (i + 1));
         }

         putCode(Code::Push_Lit, n * 32);
         putStmntDropArg(stmnt->args[1], 0);
      }
      else
      {
         Core::FastU i = n - 1;

         Core::FastU dropLen = 0;
         for(auto j = i; --j;) dropLen += j * 4;

         putCode(Code::Jcnd_Lit, skip, putPos + 28 + i * 4 + 8);
         putCode(Code::Call_Lit, fIdx);
         putCode(Code::Drop_LocReg, func->localReg + 0);
         for(auto j = i; j--;) putCode(Code::Drop_Nul);
         putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
            + 8 * (i + 1));

         while(i--)
         {
            dropLen -= i * 4;

            putCode(Code::Jcnd_Lit, skip, putPos + 40 + i * 4 + 8);
            putCode(Code::Call_Lit, fIdx);
            putCode(Code::Push_Lit, (n - 1 - i) * 32);
            putCode(Code::AddU);
            putCode(Code::Drop_LocReg, func->localReg + 0);
            for(auto j = i; j--;) putCode(Code::Drop_Nul);
            putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
               + 8 * (i + 1));
         }

         putCode(Code::Push_Lit, n * 32);
         putCode(Code::Drop_LocReg, func->localReg + 0);

         putCode(Code::Push_LocReg, func->localReg + 0);
      }
   }

   //
   // Info::putStmnt_Bges
   //
   void Info::putStmnt_Bges()
   {
      auto bits = getWord(stmnt->args[2].aLit);
      auto offs = getWord(stmnt->args[3].aLit);

      putCode(Code::Push_Lit, 32 - bits - offs);
      putCode(Code::ShLU);

      putCode(Code::Push_Lit, 32 - bits);
      putCode(Code::ShRI);
   }

   //
   // Info::putStmnt_Bget
   //
   void Info::putStmnt_Bget()
   {
      auto bits = getWord(stmnt->args[2].aLit);
      auto offs = getWord(stmnt->args[3].aLit);
      auto mask = (static_cast<Core::FastU>(1) << bits) - 1;

      if(offs)
      {
         putCode(Code::Push_Lit, offs);
         putCode(Code::ShRI);
      }

      putCode(Code::Push_Lit, mask);
      putCode(Code::BAnd);
   }

   //
   // Info::putStmnt_Bset
   //
   void Info::putStmnt_Bset()
   {
      auto bits = getWord(stmnt->args[2].aLit);
      auto offs = getWord(stmnt->args[3].aLit);
      auto mask = (static_cast<Core::FastU>(1) << bits) - 1;

      putCode(Code::Push_Lit, mask);
      putCode(Code::BAnd);

      if(offs)
      {
         putCode(Code::Push_Lit, offs);
         putCode(Code::ShLU);
      }

      putStmntPushArg(stmnt->args[0], 0);
      putCode(Code::Push_Lit, ~(mask << offs));
      putCode(Code::BAnd);
      putCode(Code::BOrI);
      putStmntDropArg(stmnt->args[0], 0);
   }

   //
   // Info::trStmnt_BAnd
   //
   void Info::trStmnt_BAnd()
   {
      auto n = getStmntSize();

      if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
      {
         moveArgStk_dst(stmnt->args[0]);
      }
      else
      {
         trStmntStk3(false);

         if(n > 1)
            func->setLocalTmp(n * 2 - 1);
      }
   }

   //
   // Info::trStmnt_BNot
   //
   void Info::trStmnt_BNot()
   {
      auto n = getStmntSize();

      if(isPushArg(stmnt->args[1]))
         moveArgStk_dst(stmnt->args[0]);
      else if(isDropArg(stmnt->args[0]))
         moveArgStk_src(stmnt->args[1]);
      else
      {
         if(n > 2)
            func->setLocalTmp(n - 2);

         moveArgStk_dst(stmnt->args[0]);
         moveArgStk_src(stmnt->args[1]);
      }
   }

   //
   // Info::trStmnt_Bclz
   //
   void Info::trStmnt_Bclz()
   {
      auto n = getStmntSize();

      if(n <= 1)
      {
         trStmntStk2();
      }
      else if(isPushArg(stmnt->args[1]))
      {
         moveArgStk_dst(stmnt->args[0]);
      }
      else if(isDropArg(stmnt->args[0]))
      {
         moveArgStk_src(stmnt->args[1]);
      }
      else
      {
         trStmntStk2();

         func->setLocalTmp(1);
      }
   }

   //
   // Info::trStmnt_Bget
   //
   void Info::trStmnt_Bget()
   {
      trStmntStk2();
   }

   //
   // Info::trStmnt_Bset
   //
   void Info::trStmnt_Bset()
   {
      moveArgStk_src(stmnt->args[1]);
   }
}

// EOF

