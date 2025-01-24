//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
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
      genCode(Code::Copy);
   }

   //
   // Info::genStmnt_Move
   //
   void Info::genStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n != 1 ||
         ((stmnt->args[0].a == IR::ArgBase::Stk || isDropArg(stmnt->args[0])) &&
          (stmnt->args[1].a == IR::ArgBase::Stk || isPushArg(stmnt->args[1]))))
      {
         if(stmnt->args[0].a != IR::ArgBase::Stk)
         {
            if(stmnt->args[1].a != IR::ArgBase::Stk)
            {
               for(Core::FastU w = 0; w != n; ++w)
               {
                  genStmntDropArgPre(stmnt->args[0], w);
                  genStmntPushArg(   stmnt->args[1], w);
                  genStmntDropArgSuf(stmnt->args[0], w);
               }
            }
            else
               genStmntDropArg(stmnt->args[0], 0, n);
         }
         else if(stmnt->args[1].a != IR::ArgBase::Stk)
            genStmntPushArg(stmnt->args[1], 0, n);

         return;
      }

      // push_?
      if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
      {
      case IR::ArgBase::Aut:
         genCode(Code::Push_LocReg, getStkPtrIdx());
         genCode(Code::AddU);

         if(stmnt->args[1].aAut.off)
         {
            genCode(Code::Push_Lit, stmnt->args[1].aAut.off);
            genCode(Code::AddU);
         }

         genCode(Code::Push_GblArr, StaArray);
         break;

      case IR::ArgBase::GblArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aGblArr, Code::Push_GblArr);
         break;

      case IR::ArgBase::HubArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aHubArr, Code::Push_HubArr);
         break;

      case IR::ArgBase::LocArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aLocArr, Code::Push_LocArr);
         break;

      case IR::ArgBase::ModArr:
         genStmnt_Move__Stk_Arr(stmnt->args[1].aModArr, Code::Push_ModArr);
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[1].aSta.off)
         {
            genCode(Code::Push_Lit, stmnt->args[1].aSta.off);
            genCode(Code::AddU);
         }

         genCode(Code::Push_GblArr, StaArray);
         break;

      case IR::ArgBase::StrArs:
         if(stmnt->args[1].aStrArs.off)
         {
            genCode(Code::Push_Lit, stmnt->args[1].aStrArs.off);
            genCode(Code::AddU);
         }

         genCode(Code::Cnat, 2, 15);
         break;

      default:
         genStmntPushArg(stmnt->args[1], 0);
         break;
      }

      // drop_?
      else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::Aut:
         genCode(Code::Push_LocReg, getStkPtrIdx());
         genCode(Code::AddU);

         if(stmnt->args[0].aAut.off)
         {
            genCode(Code::Push_Lit, stmnt->args[0].aAut.off);
            genCode(Code::AddU);
         }

         genCode(Code::Swap);
         genCode(Code::Drop_GblArr, StaArray);
         break;

      case IR::ArgBase::GblArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aGblArr, Code::Drop_GblArr);
         break;

      case IR::ArgBase::HubArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aHubArr, Code::Drop_HubArr);
         break;

      case IR::ArgBase::LocArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aLocArr, Code::Drop_LocArr);
         break;

      case IR::ArgBase::ModArr:
         genStmnt_Move__Arr_Stk(stmnt->args[0].aModArr, Code::Drop_ModArr);
         break;

      case IR::ArgBase::Sta:
         if(stmnt->args[0].aSta.off)
         {
            // The higher level codegen does not combine addr-on-stack with
            // offsets, so this is just a fallback for hand-written assembly.
            // It is still pretty gross-looking, though. Maybe add a warning?
            genCode(Code::Swap);
            genCode(Code::Push_Lit, stmnt->args[0].aSta.off);
            genCode(Code::AddU);
            genCode(Code::Swap);
         }

         genCode(Code::Drop_GblArr, StaArray);
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
   void Info::genStmnt_Move__Arr_Stk(IR::ArgPtr2 const &arr, Code code)
   {
      if(arr.off)
      {
         genCode(Code::Swap);
         genCode(Code::Push_Lit, arr.off);
         genCode(Code::AddU);
         genCode(Code::Swap);
      }

      genCode(code, arr.arr->aLit.value);
   }

   //
   // Info::genStmnt_Move__Stk_Arr
   //
   void Info::genStmnt_Move__Stk_Arr(IR::ArgPtr2 const &arr, Code code)
   {
      if(arr.off)
      {
         genCode(Code::Push_Lit, arr.off);
         genCode(Code::AddU);
      }

      genCode(code, arr.arr->aLit.value);
   }

   //
   // Info::genStmnt_Swap
   //
   void Info::genStmnt_Swap()
   {
      auto n = getStmntSize();

      if(n == 1)
         return genCode(Code::Swap);

      for(Core::FastU i = n; i--;)
         genStmntDropTmp(i);
      for(Core::FastU i = n; i--;)
         genStmntDropTmp(i + n);

      for(Core::FastU i = 0; i != n; ++i)
         genStmntPushTmp(i);
      for(Core::FastU i = 0; i != n; ++i)
         genStmntPushTmp(i + n);
   }

   //
   // Info::trStmnt_Move
   //
   void Info::trStmnt_Move()
   {
      auto n = getStmntSize();

      // Multi-word?
      if(n != 1 ||
         ((stmnt->args[0].a == IR::ArgBase::Stk || isDropArg(stmnt->args[0])) &&
          (stmnt->args[1].a == IR::ArgBase::Stk || isPushArg(stmnt->args[1]))))
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

