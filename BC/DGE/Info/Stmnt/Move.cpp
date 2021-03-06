//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Move statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::chkStmnt_Copy
   //
   void Info::chkStmnt_Copy()
   {
      chkStmntArgB(0, IR::ArgBase::Stk);
      chkStmntArgB(1, IR::ArgBase::Stk);

      if(getStmntSizeW() > 1)
         IR::ErrorCode(stmnt, "unsupported size");
   }

   //
   // Info::chkStmnt_Move
   //
   void Info::chkStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n > 4)
      {
         if(n % 4) IR::ErrorCode(stmnt, "bad size");
         return;
      }

      if(n != 1 && n != 2 && n != 4)
         IR::ErrorCode(stmnt, "unsupported size");
   }

   //
   // Info::chkStmnt_Swap
   //
   void Info::chkStmnt_Swap()
   {
      chkStmntArgB(0, IR::ArgBase::Stk);
      chkStmntArgB(1, IR::ArgBase::Stk);
   }

   //
   // Info::putStmnt_Copy
   //
   void Info::putStmnt_Copy()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      return putCode("Copy");
   }

   //
   // Info::putStmnt_Move
   //
   void Info::putStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n > 4)
      {
         n /= 4;

         if(stmnt->args[0].a != IR::ArgBase::Stk)
         {
            if(stmnt->args[1].a != IR::ArgBase::Stk)
            {
               for(Core::FastU w = 0; w != n; ++w)
               {
                  putStmntPushArg(stmnt->args[1], w);
                  putStmntDropArg(stmnt->args[0], w);
               }
            }
            else
               putStmntDropArg(stmnt->args[0], 0, n);
         }
         else if(stmnt->args[1].a != IR::ArgBase::Stk)
            putStmntPushArg(stmnt->args[1], 0, n);

         return;
      }

      bool b = n == 1;
      bool w = n == 4;

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

         putCode(w ? "Push_Ptr" : b ? "Push_PtrB" : "Push_PtrH");
         break;

      case IR::ArgBase::Lit:
         putStmntPushArg(stmnt->args[1], 0);
         break;

      case IR::ArgBase::LocReg:
         putCode(w ? "Push_Reg" : b ? "Push_RegB" : "Push_RegH",
            getWord(stmnt->args[1].aLocReg.idx->aLit) + stmnt->args[1].aLocReg.off);
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[1].aSta.off)
         {
            putCode("Push_Lit", stmnt->args[1].aSta.off);
            putCode("AddU");
         }

         putCode(w ? "Push_Ptr" : b ? "Push_PtrB" : "Push_PtrH");
         break;

      case IR::ArgBase::StrArs:
         if(stmnt->args[1].aStrArs.off)
         {
            putCode("Push_Lit", stmnt->args[1].aStrArs.off);
            putCode("AddU");
         }

         putCode(w ? "Push_Str" : b ? "Push_StrB" : "Push_StrH");
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

         putCode(w ? "Drop_Ptr" : b ? "Drop_PtrB" : "Drop_PtrH");
         break;

      case IR::ArgBase::LocReg:
         putCode(w ? "Drop_Reg" : b ? "Drop_RegB" : "Drop_RegH",
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

         putCode(w ? "Drop_Ptr" : b ? "Drop_PtrB" : "Drop_PtrH");
         break;

      default:
         putStmntDropArg(stmnt->args[0], 0);
         break;
      }

      // ???
      else
         IR::ErrorCode(stmnt, "bad put");
   }

   //
   // Info::putStmnt_Swap
   //
   void Info::putStmnt_Swap()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode("Swap");

      putStmntDropTmp(0, n * 2);
      putStmntPushTmp(n, n * 2);
      putStmntPushTmp(0, n);
   }

   //
   // Info::trStmnt_Move
   //
   void Info::trStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n > 4)
         return;

      #define moveIdx(name, i) \
         moveArgStk_src(*stmnt->args[i].a##name.idx)

      // Push to stack?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Lit:    break;
      case IR::ArgBase::LocReg: break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    1); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    1); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 1); break;

      default:
         IR::ErrorCode(stmnt, "bad tr push");
      }

      // Drop from stack?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::LocReg: break;
      case IR::ArgBase::Nul:    break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    0); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    0); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 0); break;

      default:
         IR::ErrorCode(stmnt, "bad tr drop");
      }

      // Neither stack, split move and rescan.
      else
         moveArgStk_src(stmnt->args[1]);

      #undef moveIdx
   }

   //
   // Info::trStmnt_Swap
   //
   void Info::trStmnt_Swap()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      trStmntTmp(n * 2);
   }
}

// EOF

