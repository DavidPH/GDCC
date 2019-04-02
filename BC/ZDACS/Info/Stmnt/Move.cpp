//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Move statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "IR/Exception.hpp"
#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::chkStmnt_Copy
   //
   void Info::chkStmnt_Copy()
   {
      chkStmntArgB(0, IR::ArgBase::Stk);
      chkStmntArgB(1, IR::ArgBase::Stk);

      if(getStmntSize() > 1)
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
   // Info::genStmnt_Copy
   //
   void Info::genStmnt_Copy()
   {
      numChunkCODE += 4;
   }

   //
   // Info::genStmnt_Move
   //
   void Info::genStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n != 1)
      {
         if(stmnt->args[0].a != IR::ArgBase::Stk)
            numChunkCODE += lenDropArg(stmnt->args[0], 0, n);

         if(stmnt->args[1].a != IR::ArgBase::Stk)
            numChunkCODE += lenPushArg(stmnt->args[1], 0, n);

         return;
      }

      // push_?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Aut:
         numChunkCODE += stmnt->args[1].aAut.off ? 32 : 20;
         break;

      case IR::ArgBase::GblArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aGblArr);
         break;

      case IR::ArgBase::HubArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aHubArr);
         break;

      case IR::ArgBase::LocArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aLocArr);
         break;

      case IR::ArgBase::ModArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aModArr);
         break;

      case IR::ArgBase::Sta:
         numChunkCODE += stmnt->args[1].aSta.off ? 20 : 8;
         break;

      case IR::ArgBase::StrArs:
         numChunkCODE += stmnt->args[1].aStrArs.off ? 24 : 12;
         break;

      default:
         genStmntPushArg(stmnt->args[1], 0);
         break;
      }

      // drop_?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::Aut:
         numChunkCODE += stmnt->args[0].aAut.off ? 36 : 24;
         break;

      case IR::ArgBase::Nul: numChunkCODE += 4; break;

      case IR::ArgBase::GblArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aGblArr);
         break;

      case IR::ArgBase::HubArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aHubArr);
         break;

      case IR::ArgBase::LocArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aLocArr);
         break;

      case IR::ArgBase::ModArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aModArr);
         break;

      case IR::ArgBase::Sta:
         numChunkCODE += stmnt->args[0].aSta.off ? 24 : 12;
         break;

      default:
         genStmntDropArg(stmnt->args[0], 0);
         break;
      }

      // ???
      else
         Core::Error(stmnt->pos, "bad gen Move_W");
   }

   //
   // Info::genStmnt_Move__Arr_Stk
   //
   void Info::genStmnt_Move__Arr_Stk(IR::ArgPtr2 const &arr)
   {
      numChunkCODE += arr.off ? 24 : 12;
   }

   //
   // Info::genStmnt_Move__Stk_Arr
   //
   void Info::genStmnt_Move__Stk_Arr(IR::ArgPtr2 const &arr)
   {
      numChunkCODE += arr.off ? 20 : 8;
   }

   //
   // Info::genStmnt_Swap
   //
   void Info::genStmnt_Swap()
   {
      auto n = getStmntSize();

      if(n == 1)
         numChunkCODE += 4;
      else
         numChunkCODE += n * 32;
   }

   //
   // Info::putStmnt_Copy
   //
   void Info::putStmnt_Copy()
   {
      putCode(Code::Copy);
   }

   //
   // Info::putStmnt_Move
   //
   void Info::putStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n != 1)
      {
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

      // push_?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Aut:
         putCode(Code::Push_LocReg);
         putWord(getStkPtrIdx());
         putCode(Code::AddU);

         if(stmnt->args[1].aAut.off)
         {
            putCode(Code::Push_Lit);
            putWord(stmnt->args[1].aAut.off);
            putCode(Code::AddU);
         }

         putCode(Code::Push_GblArr);
         putWord(StaArray);
         break;

      case IR::ArgBase::GblArr:
         putStmnt_Move__Stk_Arr(stmnt->args[1].aGblArr, Code::Push_GblArr);
         break;

      case IR::ArgBase::HubArr:
         putStmnt_Move__Stk_Arr(stmnt->args[1].aHubArr, Code::Push_HubArr);
         break;

      case IR::ArgBase::LocArr:
         putStmnt_Move__Stk_Arr(stmnt->args[1].aLocArr, Code::Push_LocArr);
         break;

      case IR::ArgBase::ModArr:
         putStmnt_Move__Stk_Arr(stmnt->args[1].aModArr, Code::Push_ModArr);
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[1].aSta.off)
         {
            putCode(Code::Push_Lit);
            putWord(stmnt->args[1].aSta.off);
            putCode(Code::AddU);
         }

         putCode(Code::Push_GblArr);
         putWord(StaArray);
         break;

      case IR::ArgBase::StrArs:
         if(stmnt->args[1].aStrArs.off)
         {
            putCode(Code::Push_Lit);
            putWord(stmnt->args[1].aStrArs.off);
            putCode(Code::AddU);
         }

         putCode(Code::Cnat);
         putWord(2);
         putWord(15);
         break;

      default:
         putStmntPushArg(stmnt->args[1], 0);
         break;
      }

      // drop_?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::Aut:
         putCode(Code::Push_LocReg);
         putWord(getStkPtrIdx());
         putCode(Code::AddU);

         if(stmnt->args[0].aAut.off)
         {
            putCode(Code::Push_Lit);
            putWord(stmnt->args[0].aAut.off);
            putCode(Code::AddU);
         }

         putCode(Code::Swap);
         putCode(Code::Drop_GblArr);
         putWord(StaArray);
         break;

      case IR::ArgBase::GblArr:
         putStmnt_Move__Arr_Stk(stmnt->args[0].aGblArr, Code::Drop_GblArr);
         break;

      case IR::ArgBase::LocArr:
         putStmnt_Move__Arr_Stk(stmnt->args[0].aLocArr, Code::Drop_LocArr);
         break;

      case IR::ArgBase::ModArr:
         putStmnt_Move__Arr_Stk(stmnt->args[0].aModArr, Code::Drop_ModArr);
         break;

      case IR::ArgBase::Nul:
         putCode(Code::Drop_Nul);
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[0].aSta.off)
         {
            putCode(Code::Push_Lit);
            putWord(stmnt->args[0].aSta.off);
            putCode(Code::AddU);
         }

         putCode(Code::Swap);
         putCode(Code::Drop_GblArr);
         putWord(StaArray);
         break;

      case IR::ArgBase::HubArr:
         putStmnt_Move__Arr_Stk(stmnt->args[0].aHubArr, Code::Drop_HubArr);
         break;

      default:
         putStmntDropArg(stmnt->args[0], 0);
         break;
      }

      // ???
      else
         Core::Error(stmnt->pos, "bad put Move_W");
   }

   //
   // Info::putStmnt_Move__Arr_Stk
   //
   void Info::putStmnt_Move__Arr_Stk(IR::ArgPtr2 const &arr, Code code)
   {
      if(arr.off)
      {
         putCode(Code::Push_Lit);
         putWord(arr.off);
         putCode(Code::AddU);
      }

      putCode(Code::Swap);
      putCode(code);
      putWord(getWord(arr.arr->aLit));
   }

   //
   // Info::putStmnt_Move__Stk_Arr
   //
   void Info::putStmnt_Move__Stk_Arr(IR::ArgPtr2 const &arr, Code code)
   {
      if(arr.off)
      {
         putCode(Code::Push_Lit);
         putWord(arr.off);
         putCode(Code::AddU);
      }

      putCode(code);
      putWord(getWord(arr.arr->aLit));
   }

   //
   // Info::putStmnt_Swap
   //
   void Info::putStmnt_Swap()
   {
      auto n = getStmntSize();

      if(n == 1)
      {
         putCode(Code::Swap);
         return;
      }

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i);
      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i + n);

      for(Core::FastU i = 0; i != n; ++i)
         putCode(Code::Push_LocReg, func->localReg + i);
      for(Core::FastU i = 0; i != n; ++i)
         putCode(Code::Push_LocReg, func->localReg + i + n);
   }

   //
   // Info::trStmnt_Move
   //
   void Info::trStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n != 1)
         return;

      #define moveIdx(name, i) \
         moveArgStk_src(*stmnt->args[i].a##name.idx)

      // Push to stack?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::GblReg: break;
      case IR::ArgBase::HubReg: break;
      case IR::ArgBase::Lit:    break;
      case IR::ArgBase::LocReg: break;
      case IR::ArgBase::ModReg: break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    1); break;
      case IR::ArgBase::GblArr: moveIdx(GblArr, 1); break;
      case IR::ArgBase::HubArr: moveIdx(HubArr, 1); break;
      case IR::ArgBase::LocArr: moveIdx(LocArr, 1); break;
      case IR::ArgBase::ModArr: moveIdx(ModArr, 1); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    1); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 1); break;

      default:
         Core::Error(stmnt->pos, "bad tr Move_W push");
      }

      // Drop from stack?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg: break;
      case IR::ArgBase::HubReg: break;
      case IR::ArgBase::LocReg: break;
      case IR::ArgBase::ModReg: break;
      case IR::ArgBase::Nul:    break;

      case IR::ArgBase::Aut:    moveIdx(Aut,    0); break;
      case IR::ArgBase::GblArr: moveIdx(GblArr, 0); break;
      case IR::ArgBase::HubArr: moveIdx(HubArr, 0); break;
      case IR::ArgBase::LocArr: moveIdx(LocArr, 0); break;
      case IR::ArgBase::ModArr: moveIdx(ModArr, 0); break;
      case IR::ArgBase::Sta:    moveIdx(Sta,    0); break;
      case IR::ArgBase::StrArs: moveIdx(StrArs, 0); break;

      default:
         Core::Error(stmnt->pos, "bad tr Move_W drop");
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
      auto n = getStmntSize();

      if(n != 1)
         func->setLocalTmp(n * 2);
   }
}

// EOF

