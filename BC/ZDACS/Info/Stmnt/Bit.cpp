//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2024 David Hill
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
#include "BC/ZDACS/Module.hpp"

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
      GDCC_BC_AddFuncPre((ones ? Code::Bclo : Code::Bclz, 1), 1, 1, 1, __FILE__);
      GDCC_BC_AddFuncObjUna(1);

      // Counting leading ones works the same way if inverted.
      // May be advantageous to invert the conditions, instead, though.
      if(ones)
         GDCC_BC_AddStmnt(Code::BNot, 1, src, src);

      // Node codegen, mask-and-branch.
      auto node = [&](char const *label, Core::FastU mask, char const *dst)
      {
         if(label) GDCC_BC_AddLabel(name + label);
         IR::Glyph dstGlyph{prog, name + dst};
         GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src, mask);
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
      GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, 32, src);
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
   void Info::genStmnt_BAnd(Code code)
   {
      auto n = getStmntSize();

      if(stmnt->args[1].a == IR::ArgBase::Stk &&
         stmnt->args[2].a == IR::ArgBase::Stk)
      {
         if(n > 1)
         {
            for(Core::FastU i = 0, e = n * 2 - 1; i != e; ++i)
               genStmntDropTmp(i);

            genStmntPushTmp(n - 1);
         }

         genCode(code);

         if(n > 1) for(Core::FastU i = n - 1; i--;)
         {
            genStmntPushTmp(n + i);
            genStmntPushTmp(i);
            genCode(code);
         }
      }
      else
      {
         for(Core::FastU i = 0; i != n; ++i)
         {
            genStmntPushArg(stmnt->args[1], i);
            genStmntPushArg(stmnt->args[2], i);
            genCode(code);
         }
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
         for(Core::FastU i = 0; i != n; ++i)
         {
            genStmntPushArg(stmnt->args[1], i);
            genCode(Code::BNot);
         }
      }
      else if(isDropArg(stmnt->args[0]))
      {
         if(auto i = n) while(i--)
         {
            genCode(Code::BNot);
            genStmntDropArg(stmnt->args[0], i);
         }
      }
      else
      {
         if(n > 2)
         {
            for(Core::FastU i = n - 2; i--;)
            {
               genCode(Code::BNot);
               genStmntDropTmp(i);
            }
         }

         if(n > 0)
            genCode(Code::BNot);

         if(n > 1)
         {
            genCode(Code::Swap);
            genCode(Code::BNot);
            genCode(Code::Swap);
         }

         if(n > 2)
         {
            for(Core::FastU i = 0, e = n - 2; i != e; ++i)
               genStmntPushTmp(i);
         }
      }
   }

   //
   // Info::genStmnt_Bclz
   //
   void Info::genStmnt_Bclz(bool ones)
   {
      auto n = getStmntSize();

      if(n == 0)
         return genCode(Code::Push_Lit, 0);

      Core::String name = getFuncName(ones ? IR::CodeBase::Bclo : IR::CodeBase::Bclz, 1);

      if(n == 1)
         return genStmntCall(name, 1);

      Core::FastU skip = ones ? 0xFFFFFFFF : 0;

      Core::Array<std::size_t> jumpEnd{Core::Size, n};
      std::size_t              jumpNext;

      if(stmnt->args[1].a != IR::ArgBase::Stk)
      {
         Core::FastU i = n - 1;

         genStmntPushArg(stmnt->args[1], i);
         jumpNext = module->chunkCODE.size();
         genCode(Code::Jcnd_Lit, skip, 0);
         genCode(Code::Call_Lit, name);
         jumpEnd[i] = module->chunkCODE.size();
         genCode(Code::Jump_Lit, 0);
         module->chunkCODE[jumpNext].args[1] = getCodePos();

         while(i--)
         {
            genStmntPushArg(stmnt->args[1], i);
            jumpNext = module->chunkCODE.size();
            genCode(Code::Jcnd_Lit, skip, 0);
            genCode(Code::Call_Lit, name);
            genCode(Code::Push_Lit, (n - 1 - i) * 32);
            genCode(Code::AddU);
            jumpEnd[i] = module->chunkCODE.size();
            genCode(Code::Jump_Lit, 0);
            module->chunkCODE[jumpNext].args[1] = getCodePos();
         }

         genCode(Code::Push_Lit, n * 32);
      }
      else if(stmnt->args[0].a != IR::ArgBase::Stk)
      {
         Core::FastU i = n - 1;

         jumpNext = module->chunkCODE.size();
         genCode(Code::Jcnd_Lit, skip, 0);
         genCode(Code::Call_Lit, name);
         genStmntDropArg(stmnt->args[1], 0);
         for(auto j = i; j--;) genCode(Code::Drop_Nul);
         jumpEnd[i] = module->chunkCODE.size();
         genCode(Code::Jump_Lit, 0);
         module->chunkCODE[jumpNext].args[1] = getCodePos();

         while(i--)
         {
            jumpNext = module->chunkCODE.size();
            genCode(Code::Jcnd_Lit, skip, 0);
            genCode(Code::Call_Lit, name);
            genCode(Code::Push_Lit, (n - 1 - i) * 32);
            genCode(Code::AddU);
            genStmntDropArg(stmnt->args[1], 0);
            for(auto j = i; j--;) genCode(Code::Drop_Nul);
            jumpEnd[i] = module->chunkCODE.size();
            genCode(Code::Jump_Lit, 0);
            module->chunkCODE[jumpNext].args[1] = getCodePos();
         }

         genCode(Code::Push_Lit, n * 32);
         genStmntDropArg(stmnt->args[1], 0);
      }
      else
      {
         Core::FastU i = n - 1;

         jumpNext = module->chunkCODE.size();
         genCode(Code::Jcnd_Lit, skip, 0);
         genCode(Code::Call_Lit, name);
         genStmntDropTmp(0);
         for(auto j = i; j--;) genCode(Code::Drop_Nul);
         genStmntPushTmp(0);
         jumpEnd[i] = module->chunkCODE.size();
         genCode(Code::Jump_Lit, 0);
         module->chunkCODE[jumpNext].args[1] = getCodePos();

         while(i--)
         {
            jumpNext = module->chunkCODE.size();
            genCode(Code::Jcnd_Lit, skip, 0);
            genCode(Code::Call_Lit, name);
            genCode(Code::Push_Lit, (n - 1 - i) * 32);
            genCode(Code::AddU);
            if(i)
            {
               genStmntDropTmp(0);
               for(auto j = i; j--;) genCode(Code::Drop_Nul);
               genStmntPushTmp(0);
            }
            jumpEnd[i] = module->chunkCODE.size();
            genCode(Code::Jump_Lit, 0);
            module->chunkCODE[jumpNext].args[1] = getCodePos();
         }

         genCode(Code::Push_Lit, n * 32);
      }

      for(auto &jump : jumpEnd)
         module->chunkCODE[jump].args[0] = getCodePos();
   }

   //
   // Info::genStmnt_Bges
   //
   void Info::genStmnt_Bges()
   {
      auto bits = getWord(stmnt->args[2].aLit);
      auto offs = getWord(stmnt->args[3].aLit);

      genCode(Code::Push_Lit, 32 - bits - offs);
      genCode(Code::ShLU);

      genCode(Code::Push_Lit, 32 - bits);
      genCode(Code::ShRI);
   }

   //
   // Info::genStmnt_Bget
   //
   void Info::genStmnt_Bget()
   {
      auto bits = getWord(stmnt->args[2].aLit);
      auto offs = getWord(stmnt->args[3].aLit);
      auto mask = (static_cast<Core::FastU>(1) << bits) - 1;

      if(offs)
      {
         genCode(Code::Push_Lit, offs);
         genCode(Code::ShRI);
      }

      genCode(Code::Push_Lit, mask);
      genCode(Code::BAnd);
   }

   //
   // Info::genStmnt_Bset
   //
   void Info::genStmnt_Bset()
   {
      auto bits = getWord(stmnt->args[2].aLit);
      auto offs = getWord(stmnt->args[3].aLit);
      auto mask = (static_cast<Core::FastU>(1) << bits) - 1;

      genCode(Code::Push_Lit, mask);
      genCode(Code::BAnd);

      if(offs)
      {
         genCode(Code::Push_Lit, offs);
         genCode(Code::ShLU);
      }

      genStmntPushArg(stmnt->args[0], 0);
      genCode(Code::Push_Lit, ~(mask << offs));
      genCode(Code::BAnd);
      genCode(Code::BOrI);
      genStmntDropArg(stmnt->args[0], 0);
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

