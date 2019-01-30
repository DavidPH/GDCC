//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati statement output.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::putStmnt
   //
   void Info::putStmnt()
   {
      putOrigin(stmnt->pos);

      for(auto const &label : stmnt->labs)
         putNTS("label"), putNTS('('), putNTS(label), putNTS(')');

      switch(stmnt->code)
      {
      case IR::Code::Nop: break;

      case IR::Code::AdXU: putStmnt_AdXU(); break;
      case IR::Code::AddF: putStmnt_AddF(); break;
      case IR::Code::AddI: putStmnt_AddI(); break;
      case IR::Code::AddU: putStmnt_AddU(); break;
      case IR::Code::BAnd: putStmnt_BAnd(); break;
      case IR::Code::BNot: putStmnt_BNot(); break;
      case IR::Code::BOrI: putStmnt_BOrI(); break;
      case IR::Code::BOrX: putStmnt_BOrX(); break;
      case IR::Code::Bclo: putStmnt_Bclo(); break;
      case IR::Code::Bclz: putStmnt_Bclz(); break;

      case IR::Code::CmpF_EQ: putStmnt_CmpF_EQ(); break;
      case IR::Code::CmpF_GE: putStmnt_CmpF_GE(); break;
      case IR::Code::CmpF_GT: putStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_LE: putStmnt_CmpF_LE(); break;
      case IR::Code::CmpF_LT: putStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_NE: putStmnt_CmpF_NE(); break;
      case IR::Code::CmpI_EQ: putStmnt_CmpI_EQ(); break;
      case IR::Code::CmpI_GE: putStmnt_CmpI_GE(); break;
      case IR::Code::CmpI_GT: putStmnt_CmpI_GT(); break;
      case IR::Code::CmpI_LE: putStmnt_CmpI_LE(); break;
      case IR::Code::CmpI_LT: putStmnt_CmpI_LT(); break;
      case IR::Code::CmpI_NE: putStmnt_CmpI_NE(); break;
      case IR::Code::CmpU_EQ: putStmnt_CmpU_EQ(); break;
      case IR::Code::CmpU_GE: putStmnt_CmpU_GE(); break;
      case IR::Code::CmpU_GT: putStmnt_CmpU_GT(); break;
      case IR::Code::CmpU_LE: putStmnt_CmpU_LE(); break;
      case IR::Code::CmpU_LT: putStmnt_CmpU_LT(); break;
      case IR::Code::CmpU_NE: putStmnt_CmpU_NE(); break;

      case IR::Code::Call: putStmnt_Call(); break;
      case IR::Code::Cnat: putStmnt_Cnat(); break;
      case IR::Code::Copy: putStmnt_Copy(); break;
      case IR::Code::DiXI: putStmnt_DiXI(); break;
      case IR::Code::DiXU: putStmnt_DiXU(); break;
      case IR::Code::DivA: putStmnt_DivA(); break;
      case IR::Code::DivF: putStmnt_DivF(); break;
      case IR::Code::DivI: putStmnt_DivI(); break;
      case IR::Code::DivK: putStmnt_DivK(); break;
      case IR::Code::DivR: putStmnt_DivR(); break;
      case IR::Code::DivU: putStmnt_DivU(); break;
      case IR::Code::DivX: putStmnt_DivX(); break;

      case IR::Code::Jcnd_Nil: putStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: putStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: putStmnt_Jcnd_Tru(); break;
      case IR::Code::Jfar_Pro: putStmnt_Jfar_Pro(); break;
      case IR::Code::Jfar_Set: putStmnt_Jfar_Set(); break;
      case IR::Code::Jfar_Sta: putStmnt_Jfar_Sta(); break;

      case IR::Code::Jump: putStmnt_Jump(); break;
      case IR::Code::LAnd: putStmnt_LAnd(); break;
      case IR::Code::LNot: putStmnt_LNot(); break;
      case IR::Code::LOrI: putStmnt_LOrI(); break;
      case IR::Code::ModI: putStmnt_ModI(); break;
      case IR::Code::ModU: putStmnt_ModU(); break;
      case IR::Code::Move: putStmnt_Move(); break;
      case IR::Code::MuXU: putStmnt_MuXU(); break;
      case IR::Code::MulA: putStmnt_MulA(); break;
      case IR::Code::MulF: putStmnt_MulF(); break;
      case IR::Code::MulI: putStmnt_MulI(); break;
      case IR::Code::MulK: putStmnt_MulK(); break;
      case IR::Code::MulR: putStmnt_MulR(); break;
      case IR::Code::MulU: putStmnt_MulU(); break;
      case IR::Code::MulX: putStmnt_MulX(); break;
      case IR::Code::NegF: putStmnt_NegF(); break;
      case IR::Code::NegI: putStmnt_NegI(); break;
      case IR::Code::Pltn: putStmnt_Pltn(); break;
      case IR::Code::Retn: putStmnt_Retn(); break;
      case IR::Code::Rjnk: putStmnt_Rjnk(); break;
      case IR::Code::ShLF: putStmnt_ShLF(); break;
      case IR::Code::ShLU: putStmnt_ShLU(); break;
      case IR::Code::ShRF: putStmnt_ShRF(); break;
      case IR::Code::ShRI: putStmnt_ShRI(); break;
      case IR::Code::ShRU: putStmnt_ShRU(); break;
      case IR::Code::SuXU: putStmnt_SuXU(); break;
      case IR::Code::SubF: putStmnt_SubF(); break;
      case IR::Code::SubI: putStmnt_SubI(); break;
      case IR::Code::SubU: putStmnt_SubU(); break;
      case IR::Code::Swap: putStmnt_Swap(); break;

      default:
         Core::Error(stmnt->pos, "DGE cannot put Code: ", stmnt->code);
      }
   }

   //
   // Info::putStmnt_Pltn
   //
   void Info::putStmnt_Pltn()
   {
      putCode("Push_Reg", getStkPtrIdx());
      putCode("AddU");
   }

   //
   // Info::putStmntCall
   //
   void Info::putStmntCall(Core::String name, Core::FastU argc)
   {
      putCode("Push_Lit", name);
      putCode("Call",     argc);
   }

   //
   // Info::putStmntDropArg
   //
   void Info::putStmntDropArg(IR::Arg const &arg, Core::FastU w)
   {
      //
      // putAut
      //
      auto putAut = [&](IR::Arg_Aut const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode("Push_Lit", getWord(a.idx->aLit) + a.off + w * 4);
            putCode("Push_Reg", getStkPtrIdx());
            putCode("AddU");
            putCode("Drop_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            putCode("Push_Reg", getStkPtrIdx());
            putCode("AddU");

            if(a.off + w)
            {
               putCode("Push_Lit", a.off + w * 4);
               putCode("AddU");
            }

            putCode("Drop_Ptr");
         }
      };

      //
      // putReg
      //
      auto putReg = [&](IR::ArgPtr1 const &a)
      {
         putCode("Drop_Reg", getWord(a.idx->aLit) + a.off + w * 4);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putNTS("Push_Lit");
            putNTS('(');
            putExpAdd(a.idx->aLit.value, a.off + w * 4);
            putNTS(')');
            putCode("Drop_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode("Push_Lit", a.off + w * 4);
               putCode("AddU");
            }

            putCode("Drop_Ptr");
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Aut:    putAut(arg.aAut);    break;
      case IR::ArgBase::LocReg: putReg(arg.aLocReg); break;
      case IR::ArgBase::Nul:    putCode("Drop_Nul"); break;
      case IR::ArgBase::Sta:    putSta(arg.aSta);    break;

      default:
         Core::Error(stmnt->pos, "bad putStmntDropArg");
      }
   }

   //
   // Info::putStmntDropArg
   //
   void Info::putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      while(hi-- != lo)
         putStmntDropArg(arg, hi);
   }

   //
   // Info::putStmntDropTmp
   //
   void Info::putStmntDropTmp(Core::FastU w)
   {
      putCode("Drop_Reg", (func->localReg + w) * 4);
   }

   //
   // Info::putStmntDropTmp
   //
   void Info::putStmntDropTmp(Core::FastU lo, Core::FastU hi)
   {
      while(hi-- != lo)
         putStmntDropTmp(hi);
   }

   //
   // Info::putStmntPushArg
   //
   void Info::putStmntPushArg(IR::Arg const &arg, Core::FastU w)
   {
      //
      // putAut
      //
      auto putAut = [&](IR::Arg_Aut const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode("Push_Lit", getWord(a.idx->aLit) + a.off + w * 4);
            putCode("Push_Reg", getStkPtrIdx());
            putCode("AddU");
            putCode("Push_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            putCode("Push_Reg", getStkPtrIdx());
            putCode("AddU");

            if(a.off + w)
            {
               putCode("Push_Lit", a.off + w * 4);
               putCode("AddU");
            }

            putCode("Push_Ptr");
         }
      };

      //
      // putLit
      //
      auto putLit = [&](IR::Arg_Lit a)
      {
         a.off += w;
         putCode("Push_Lit", a);
      };

      //
      // putReg
      //
      auto putReg = [&](IR::ArgPtr1 const &a)
      {
         putCode("Push_Reg", getWord(a.idx->aLit) + a.off + w * 4);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putNTS("Push_Lit");
            putNTS('(');
            putExpAdd(a.idx->aLit.value, a.off + w * 4);
            putNTS(')');
            putCode("Push_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode("Push_Lit", a.off + w * 4);
               putCode("AddU");
            }

            putCode("Push_Ptr");
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Aut:    putAut(arg.aAut);    break;
      case IR::ArgBase::Lit:    putLit(arg.aLit);    break;
      case IR::ArgBase::LocReg: putReg(arg.aLocReg); break;
      case IR::ArgBase::Sta:    putSta(arg.aSta);    break;

      default:
         Core::Error(stmnt->pos, "bad putStmntPushArg");
      }
   }

   //
   // Info::putStmntPushIdx
   //
   void Info::putStmntPushIdx(IR::Arg const &arg, Core::FastU w)
   {
      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putNTS("Push_Lit");
            putNTS('(');
            putExpAdd(a.idx->aLit.value, a.off + w * 4);
            putNTS(')');
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode("Push_Lit", a.off + w * 4);
               putCode("AddU");
            }
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Sta: putSta(arg.aSta); break;

      default:
         Core::Error(stmnt->pos, "bad putStmntPushIdx");
      }
   }

   //
   // Info::putStmntPushArg
   //
   void Info::putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      for(; lo != hi; ++lo)
         putStmntPushArg(arg, lo);
   }

   //
   // Info::putStmntPushTmp
   //
   void Info::putStmntPushTmp(Core::FastU w)
   {
      putCode("Push_Reg", (func->localReg + w) * 4);
   }

   //
   // Info::putStmntPushTmp
   //
   void Info::putStmntPushTmp(Core::FastU lo, Core::FastU hi)
   {
      for(; lo != hi; ++lo)
         putStmntPushTmp(lo);
   }

   //
   // Info::putStmntStkBin
   //
   void Info::putStmntStkBin(char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code, n), n * 2);
   }

   //
   // Info::putStmntStkCmp
   //
   void Info::putStmntStkCmp(int res0, char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return putCode("Push_Lit", res0);

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code, n), n * 2);
   }

   //
   // Info::putStmntStkShi
   //
   void Info::putStmntStkShi(char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code, n), n + 1);
   }

   //
   // Info::putStmntStkUna
   //
   void Info::putStmntStkUna(char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code, n), n);
   }
}

// EOF

