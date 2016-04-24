//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
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

#include "Core/Exception.hpp"

#include "IR/Program.hpp"


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
         // Info::genStmnt_Bclz_W
         //
         void Info::genStmnt_Bclz_W()
         {
            if(stmnt->op.size == 0)
            {
               numChunkCODE += 8;
               return;
            }

            if(stmnt->op.size == 1)
            {
               genStmntCall(1);
               return;
            }

            if(stmnt->args[1].a != IR::ArgBase::Stk)
            {
               numChunkCODE += 28 + (stmnt->op.size - 1) * 40 + 8
                  + lenPushArg(stmnt->args[1], 0, stmnt->op.size);
            }
            else if(stmnt->args[0].a != IR::ArgBase::Stk)
            {
               numChunkCODE += 28 + (stmnt->op.size - 1) * 40 + 8
                  + lenDropArg(stmnt->args[0], 0) * (stmnt->op.size + 1);

               for(Core::FastU i = stmnt->op.size; --i;)
                  numChunkCODE += i * 4;
            }
            else
            {
               numChunkCODE += 36 + (stmnt->op.size - 1) * 48 + 24;

               for(Core::FastU i = stmnt->op.size; --i;)
                  numChunkCODE += i * 4;
            }
         }

         //
         // Info::genStmnt_Bges_W
         //
         void Info::genStmnt_Bges_W()
         {
            numChunkCODE += 24;
         }

         //
         // Info::genStmnt_Bget_W
         //
         void Info::genStmnt_Bget_W()
         {
            auto offs = GetWord(stmnt->args[3].aLit);

            if(offs)
               numChunkCODE += 12;

            numChunkCODE += 12;
         }

         //
         // Info::genStmnt_Bset_W
         //
         void Info::genStmnt_Bset_W()
         {
            auto offs = GetWord(stmnt->args[3].aLit);

            if(offs)
               numChunkCODE += 12;

            numChunkCODE += 12 + lenPushArg(stmnt->args[0], 0) + 16 + lenDropArg(stmnt->args[0], 0);
         }

         //
         // Info::genStmntBitwise
         //
         void Info::genStmntBitwise()
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               if(stmnt->op.size > 1)
                  numChunkCODE += (stmnt->op.size * 4 - 2) * 8;
               numChunkCODE += stmnt->op.size * 4;
            }
            else
            {
               numChunkCODE += lenPushArg(stmnt->args[1], 0, stmnt->op.size);
               numChunkCODE += lenPushArg(stmnt->args[2], 0, stmnt->op.size);
               numChunkCODE += stmnt->op.size * 4;
            }
         }

         //
         // Info::preStmnt_Bclz_W
         //
         void Info::preStmnt_Bclz_W(bool ones)
         {
            if(stmnt->op.size == 0)
               return;

            Core::String name = getCallName({stmnt->op.code, 1});

            preStmntCall(name, 1, 1);

            IR::Function &newFunc = prog->getFunction(name);

            if(newFunc.defin)
               return;

            newFunc.defin    = true;
            newFunc.label    = name + "$label";
            newFunc.localReg = 1;

            newFunc.block.setOrigin({__FILE__, __LINE__});

            IR::Arg_LocReg src{IR::Arg_Lit(newFunc.block.getExp(0))};

            // Counting leading ones works the same way if inverted.
            // May be advantageous to invert the conditions, instead, though.
            if(ones)
               newFunc.block.addStatementArgs({IR::Code::InvU_W, 1}, src, src);

            // Node codegen, mask-and-branch.
            auto node = [&](char const *label, Core::FastU mask, char const *dst)
            {
               if(label) newFunc.block.addLabel(name + label);
               IR::Glyph dstGlyph{prog, name + dst};
               newFunc.block.addStatementArgs({IR::Code::AndU_W,   1}, IR::Arg_Stk(), src, mask);
               newFunc.block.addStatementArgs({IR::Code::Jcnd_Tru, 1}, IR::Arg_Stk(), dstGlyph);
            };

            // Leaf codegen, return literal.
            auto leaf = [&](char const *label, Core::FastU ret)
            {
               if(label) newFunc.block.addLabel(name + label);
               newFunc.block.addStatementArgs({IR::Code::Retn, 1}, ret);
            };

            // Binary search tree to find first set bit.
            node(nullptr,     0xFFFF0000, "$FFFF0000");
            node(nullptr,     0x0000FF00, "$0000FF00");
            node(nullptr,     0x000000F0, "$000000F0");
            node(nullptr,     0x0000000C, "$0000000C");
            node(nullptr,     0x00000002, "$00000002");
            newFunc.block.addStatementArgs({IR::Code::SubU_W, 1}, IR::Arg_Stk(), 32, src);
            newFunc.block.addStatementArgs({IR::Code::Retn,   1}, IR::Arg_Stk());
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

            throw ResetFunc();
         }

         //
         // Info::putStmnt_Bclz_W
         //
         void Info::putStmnt_Bclz_W(bool ones)
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Push_Lit, 0);
               return;
            }

            Core::String name = ones ? "___GDCC__Bclo_W1" : "___GDCC__Bclz_W1";

            if(stmnt->op.size == 1)
            {
               putStmntCall(name, 1);
               return;
            }

            Core::FastU skip = ones ? 0xFFFFFFFF : 0;
            Core::FastU fIdx = GetWord(resolveGlyph(name));

            if(stmnt->args[1].a != IR::ArgBase::Stk)
            {
               Core::FastU i = stmnt->op.size - 1;

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
                  putCode(Code::Push_Lit, (stmnt->op.size - 1 - i) * 32);
                  putCode(Code::AddU);
                  putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8
                     + lenPushArg(stmnt->args[1], 0, i));
               }

               putCode(Code::Push_Lit, stmnt->op.size * 32);
            }
            else if(stmnt->args[0].a != IR::ArgBase::Stk)
            {
               Core::FastU i = stmnt->op.size - 1;

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
                  putCode(Code::Push_Lit, (stmnt->op.size - 1 - i) * 32);
                  putCode(Code::AddU);
                  putStmntDropArg(stmnt->args[1], 0);
                  for(auto j = i; j--;) putCode(Code::Drop_Nul);
                  putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
                     + lenDropArg(stmnt->args[1], 0) * (i + 1));
               }

               putCode(Code::Push_Lit, stmnt->op.size * 32);
               putStmntDropArg(stmnt->args[1], 0);
            }
            else
            {
               Core::FastU i = stmnt->op.size - 1;

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
                  putCode(Code::Push_Lit, (stmnt->op.size - 1 - i) * 32);
                  putCode(Code::AddU);
                  putCode(Code::Drop_LocReg, func->localReg + 0);
                  for(auto j = i; j--;) putCode(Code::Drop_Nul);
                  putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
                     + 8 * (i + 1));
               }

               putCode(Code::Push_Lit, stmnt->op.size * 32);
               putCode(Code::Drop_LocReg, func->localReg + 0);

               putCode(Code::Push_LocReg, func->localReg + 0);
            }
         }

         //
         // Info::putStmnt_Bges_W
         //
         void Info::putStmnt_Bges_W()
         {
            auto bits = GetWord(stmnt->args[2].aLit);
            auto offs = GetWord(stmnt->args[3].aLit);

            putCode(Code::Push_Lit, 32 - bits - offs);
            putCode(Code::ShLU);

            putCode(Code::Push_Lit, 32 - bits);
            putCode(Code::ShRI);
         }

         //
         // Info::putStmnt_Bget_W
         //
         void Info::putStmnt_Bget_W()
         {
            auto bits = GetWord(stmnt->args[2].aLit);
            auto offs = GetWord(stmnt->args[3].aLit);
            auto mask = (static_cast<Core::FastU>(1) << bits) - 1;

            if(offs)
            {
               putCode(Code::Push_Lit, offs);
               putCode(Code::ShRI);
            }

            putCode(Code::Push_Lit, mask);
            putCode(Code::AndU);
         }

         //
         // Info::putStmnt_Bset_W
         //
         void Info::putStmnt_Bset_W()
         {
            auto bits = GetWord(stmnt->args[2].aLit);
            auto offs = GetWord(stmnt->args[3].aLit);
            auto mask = (static_cast<Core::FastU>(1) << bits) - 1;

            putCode(Code::Push_Lit, mask);
            putCode(Code::AndU);

            if(offs)
            {
               putCode(Code::Push_Lit, offs);
               putCode(Code::ShLU);
            }

            putStmntPushArg(stmnt->args[0], 0);
            putCode(Code::Push_Lit, ~(mask << offs));
            putCode(Code::AndU);
            putCode(Code::OrIU);
            putStmntDropArg(stmnt->args[0], 0);
         }

         //
         // Info::putStmntBitwise
         //
         void Info::putStmntBitwise(Code code)
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               if(stmnt->op.size > 1)
               {
                  for(Core::FastU i = 0, e = stmnt->op.size * 2 - 1; i != e; ++i)
                     putCode(Code::Drop_LocReg, func->localReg + i);

                  putCode(Code::Push_LocReg, func->localReg + stmnt->op.size - 1);
               }

               putCode(code);

               if(stmnt->op.size > 1) for(Core::FastU i = stmnt->op.size - 1; i--;)
               {
                  putCode(Code::Push_LocReg, func->localReg + stmnt->op.size + i);
                  putCode(Code::Push_LocReg, func->localReg + i);
                  putCode(code);
               }
            }
            else
            {
               for(Core::FastU i = 0; i != stmnt->op.size; ++i)
               {
                  putStmntPushArg(stmnt->args[1], i);
                  putStmntPushArg(stmnt->args[2], i);
                  putCode(code);
               }
            }
         }

         //
         // Info::trStmnt_Bclz_W
         //
         void Info::trStmnt_Bclz_W()
         {
            CheckArgC(stmnt, 2);

            if(stmnt->op.size <= 1)
            {
               trStmntStk2(1, stmnt->op.size);
            }
            else if(isPushArg(stmnt->args[1]))
            {
               moveArgStk_dst(stmnt->args[0], 1);
            }
            else if(isDropArg(stmnt->args[0]))
            {
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            }
            else
            {
               trStmntStk2(1, stmnt->op.size);

               func->setLocalTmp(1);
            }
         }

         //
         // Info::trStmnt_Bget_W
         //
         void Info::trStmnt_Bget_W()
         {
            CheckArgC(stmnt, 4);
            CheckArgB(stmnt, 2, IR::ArgBase::Lit);
            CheckArgB(stmnt, 3, IR::ArgBase::Lit);

            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "Bget_W must have size 1");

            trStmntStk2(stmnt->op.size, stmnt->op.size);
         }

         //
         // Info::trStmnt_Bset_W
         //
         void Info::trStmnt_Bset_W()
         {
            CheckArgC(stmnt, 4);
            CheckArgB(stmnt, 2, IR::ArgBase::Lit);
            CheckArgB(stmnt, 3, IR::ArgBase::Lit);

            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "Bset_W must have size 1");

            moveArgStk_src(stmnt->args[1], stmnt->op.size);
         }

         //
         // Info::trStmntBitwise
         //
         void Info::trStmntBitwise()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            }
            else
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);

               if(stmnt->op.size > 1)
                  func->setLocalTmp(stmnt->op.size * 2 - 1);
            }
         }
      }
   }
}

// EOF

