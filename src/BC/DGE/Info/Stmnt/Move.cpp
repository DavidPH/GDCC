//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Move statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::putStmnt_Copy_W
   //
   void Info::putStmnt_Copy_W()
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode("Copy");

      throw Core::ExceptStr(stmnt->pos, "unsupported size for Copy_W");
   }

   //
   // Info::putStmnt_Move_B
   //
   void Info::putStmnt_Move_B()
   {
      if(stmnt->op.size != 1 && stmnt->op.size != 2)
         throw Core::ExceptStr(stmnt->pos, "unsupported size for Move_B");

      bool b = stmnt->op.size == 1;

      // push_?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Aut:
         putCode("Push_Reg", getStkPtrIdx());
         putCode("AddU");

         if(stmnt->args[1].aAut.off)
         {
            putCode("Push_Lit", stmnt->args[1].aAut.off);
            putCode("AddU");
         }

         putCode(b ? "Push_PtrB" : "Push_PtrH");
         break;

      case IR::ArgBase::Lit:
         putStmntPushArg(stmnt->args[1], 0);
         break;

      case IR::ArgBase::LocReg:
         putCode(b ? "Push_RegB" : "Push_RegH",
            getWord(stmnt->args[1].aLocReg.idx->aLit) + stmnt->args[1].aLocReg.off);
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[1].aSta.off)
         {
            putCode("Push_Lit", stmnt->args[1].aSta.off);
            putCode("AddU");
         }

         putCode(b ? "Push_PtrB" : "Push_PtrH");
         break;

      case IR::ArgBase::StrArs:
         if(stmnt->args[1].aStrArs.off)
         {
            putCode("Push_Lit", stmnt->args[1].aStrArs.off);
            putCode("AddU");
         }

         putCode(b ? "Push_StrB" : "Push_StrH");
         break;

      default:
         throw Core::ExceptStr(stmnt->pos, "bad put Move_B push");
      }

      // drop_?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::Aut:
         putCode("Push_Reg", getStkPtrIdx());
         putCode("AddU");

         if(stmnt->args[0].aAut.off)
         {
            putCode("Push_Lit", stmnt->args[0].aAut.off);
            putCode("AddU");
         }

         putCode(b ? "Drop_PtrB" : "Drop_PtrH");
         break;

      case IR::ArgBase::LocReg:
         putCode(b ? "Drop_RegB" : "Drop_RegH",
            getWord(stmnt->args[0].aLocReg.idx->aLit) + stmnt->args[0].aLocReg.off);
         break;

      case IR::ArgBase::Nul:
         putCode("Drop_Nul");
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[0].aSta.off)
         {
            putCode("Push_Lit", stmnt->args[0].aSta.off);
            putCode("AddU");
         }

         putCode(b ? "Drop_PtrB" : "Drop_PtrH");
         break;

      default:
         throw Core::ExceptStr(stmnt->pos, "bad put Move_B drop");
      }

      // ???
      else
         throw Core::ExceptStr(stmnt->pos, "bad put Move_B");
   }

   //
   // Info::putStmnt_Move_W
   //
   void Info::putStmnt_Move_W()
   {
      // Multi-word?
      if(stmnt->op.size != 1)
      {
         if(stmnt->args[0].a != IR::ArgBase::Stk)
         {
            if(stmnt->args[1].a != IR::ArgBase::Stk)
            {
               for(Core::FastU w = 0; w != stmnt->op.size; ++w)
               {
                  putStmntPushArg(stmnt->args[1], w);
                  putStmntDropArg(stmnt->args[0], w);
               }
            }
            else
               putStmntDropArg(stmnt->args[0], 0, stmnt->op.size);
         }
         else if(stmnt->args[1].a != IR::ArgBase::Stk)
            putStmntPushArg(stmnt->args[1], 0, stmnt->op.size);

         return;
      }

      // push_?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Aut:
         putCode("Push_Reg", getStkPtrIdx());
         putCode("AddU");

         if(stmnt->args[1].aAut.off)
         {
            putCode("Push_Lit", stmnt->args[1].aAut.off);
            putCode("AddU");
         }

         putCode("Push_Ptr");
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[1].aSta.off)
         {
            putCode("Push_Lit", stmnt->args[1].aSta.off);
            putCode("AddU");
         }

         putCode("Push_Ptr");
         break;

      case IR::ArgBase::StrArs:
         if(stmnt->args[1].aStrArs.off)
         {
            putCode("Push_Lit", stmnt->args[1].aStrArs.off);
            putCode("AddU");
         }

         putCode("Push_Str");
         break;

      default:
         putStmntPushArg(stmnt->args[1], 0);
         break;
      }

      // drop_?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::Aut:
         putCode("Push_Reg", getStkPtrIdx());
         putCode("AddU");

         if(stmnt->args[0].aAut.off)
         {
            putCode("Push_Lit", stmnt->args[0].aAut.off);
            putCode("AddU");
         }

         putCode("Drop_Ptr");
         break;

      case IR::ArgBase::Nul:
         putCode("Drop_Nul");
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[0].aSta.off)
         {
            putCode("Push_Lit", stmnt->args[0].aSta.off);
            putCode("AddU");
         }

         putCode("Drop_Ptr");
         break;

      default:
         putStmntDropArg(stmnt->args[0], 0);
         break;
      }

      // ???
      else
         throw Core::ExceptStr(stmnt->pos, "bad put Move_W");
   }

   //
   // Info::putStmnt_Swap_W
   //
   void Info::putStmnt_Swap_W()
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode("Swap");

      putStmntDropTmp(0, stmnt->op.size * 2);
      putStmntPushTmp(stmnt->op.size, stmnt->op.size * 2);
      putStmntPushTmp(0, stmnt->op.size);
   }

   //
   // Info::trStmnt_Copy_W
   //
   void Info::trStmnt_Copy_W()
   {
      CheckArgC(stmnt, 2);
      CheckArgB(stmnt, 0, IR::ArgBase::Stk);
      CheckArgB(stmnt, 1, IR::ArgBase::Stk);
   }

   //
   // Info::trStmnt_Move_B
   //
   void Info::trStmnt_Move_B()
   {
      CheckArgC(stmnt, 2);
      CheckArg(stmnt->args[0], stmnt->pos);
      CheckArg(stmnt->args[1], stmnt->pos);

      if(stmnt->op.size != 1 && stmnt->op.size != 2)
         throw Core::ExceptStr(stmnt->pos, "unsupported size for Move_B");

      #define moveIdx(name, n, w) \
         moveArgStk_src(*stmnt->args[n].a##name.idx, w)

      // Push to stack?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Lit:    break;
      case IR::ArgBase::LocReg: break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    1, 1); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    1, 1); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 1, 2); break;

      default:
         throw Core::ExceptStr(stmnt->pos, "bad tr Move_B push");
      }

      // Drop from stack?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::LocReg: break;
      case IR::ArgBase::Nul:    break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    0, 1); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    0, 1); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 0, 2); break;

      default:
         throw Core::ExceptStr(stmnt->pos, "bad tr Move_B drop");
      }

      // Neither stack, split move and rescan.
      else
         moveArgStkB_src(stmnt->args[1], stmnt->op.size);

      #undef moveIdx
   }

   //
   // Info::trStmnt_Move_W
   //
   void Info::trStmnt_Move_W()
   {
      CheckArgC(stmnt, 2);
      CheckArg(stmnt->args[0], stmnt->pos);
      CheckArg(stmnt->args[1], stmnt->pos);

      // Multi-word?
      if(stmnt->op.size != 1)
         return;

      #define moveIdx(name, n, w) \
         moveArgStk_src(*stmnt->args[n].a##name.idx, w)

      // Push to stack?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Lit:    break;
      case IR::ArgBase::LocReg: break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    1, 1); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    1, 1); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 1, 2); break;

      default:
         throw Core::ExceptStr(stmnt->pos, "bad tr Move_W push");
      }

      // Drop from stack?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::LocReg: break;
      case IR::ArgBase::Nul:    break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    0, 1); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    0, 1); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 0, 2); break;

      default:
         throw Core::ExceptStr(stmnt->pos, "bad tr Move_W drop");
      }

      // Neither stack, split move and rescan.
      else
         moveArgStk_src(stmnt->args[1], stmnt->op.size);

      #undef moveIdx
   }

   //
   // Info::trStmnt_Swap_W
   //
   void Info::trStmnt_Swap_W()
   {
      CheckArgC(stmnt, 2);
      CheckArgB(stmnt, 0, IR::ArgBase::Stk);
      CheckArgB(stmnt, 1, IR::ArgBase::Stk);

      if(stmnt->op.size <= 1)
         return;

      trStmntTmp(stmnt->op.size * 2);
   }
}

// EOF

