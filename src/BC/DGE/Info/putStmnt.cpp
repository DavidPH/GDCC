//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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

#include <iostream>


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
      for(auto const &label : stmnt->labs)
         putNTS("label"), putNTS('('), putNTS(label), putNTS(')');

      switch(stmnt->op.code)
      {
      case IR::Code::AdXU_W: putStmnt_AdXU_W(); break;
      case IR::Code::AddF_W: putStmnt_AddF_W(); break;
      case IR::Code::AddI_W: putStmnt_AddI_W(); break;
      case IR::Code::AddU_W: putStmnt_AddU_W(); break;
      case IR::Code::BAnd_W: putStmnt_BAnd_W(); break;
      case IR::Code::BNot_W: putStmnt_BNot_W(); break;
      case IR::Code::BOrI_W: putStmnt_BOrI_W(); break;
      case IR::Code::BOrX_W: putStmnt_BOrX_W(); break;
      case IR::Code::Bclo_W: putStmnt_Bclo_W(); break;
      case IR::Code::Bclz_W: putStmnt_Bclz_W(); break;

      case IR::Code::CmpF_EQ_W: putStmnt_CmpF_EQ_W(); break;
      case IR::Code::CmpF_GE_W: putStmnt_CmpF_GE_W(); break;
      case IR::Code::CmpF_GT_W: putStmnt_CmpF_LT_W(); break;
      case IR::Code::CmpF_LE_W: putStmnt_CmpF_LE_W(); break;
      case IR::Code::CmpF_LT_W: putStmnt_CmpF_LT_W(); break;
      case IR::Code::CmpF_NE_W: putStmnt_CmpF_NE_W(); break;
      case IR::Code::CmpI_EQ_W: putStmnt_CmpI_EQ_W(); break;
      case IR::Code::CmpI_GE_W: putStmnt_CmpI_GE_W(); break;
      case IR::Code::CmpI_GT_W: putStmnt_CmpI_LT_W(); break;
      case IR::Code::CmpI_LE_W: putStmnt_CmpI_LE_W(); break;
      case IR::Code::CmpI_LT_W: putStmnt_CmpI_LT_W(); break;
      case IR::Code::CmpI_NE_W: putStmnt_CmpI_NE_W(); break;
      case IR::Code::CmpU_EQ_W: putStmnt_CmpU_EQ_W(); break;
      case IR::Code::CmpU_GE_W: putStmnt_CmpU_GE_W(); break;
      case IR::Code::CmpU_GT_W: putStmnt_CmpU_GT_W(); break;
      case IR::Code::CmpU_LE_W: putStmnt_CmpU_LE_W(); break;
      case IR::Code::CmpU_LT_W: putStmnt_CmpU_LT_W(); break;
      case IR::Code::CmpU_NE_W: putStmnt_CmpU_NE_W(); break;

      case IR::Code::Call: putStmnt_Call(); break;
      case IR::Code::Cnat: putStmnt_Cnat(); break;

      case IR::Code::DiXI_W: putStmnt_DiXI_W(); break;
      case IR::Code::DiXU_W: putStmnt_DiXU_W(); break;
      case IR::Code::DivF_W: putStmnt_DivF_W(); break;
      case IR::Code::DivI_W: putStmnt_DivI_W(); break;
      case IR::Code::DivU_W: putStmnt_DivU_W(); break;

      case IR::Code::Jcnd_Nil: putStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: putStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: putStmnt_Jcnd_Tru(); break;

      case IR::Code::Jfar: putStmnt_Jfar(); break;
      case IR::Code::Jump: putStmnt_Jump(); break;
      case IR::Code::LAnd: putStmnt_LAnd(); break;
      case IR::Code::LNot: putStmnt_LNot(); break;
      case IR::Code::LOrI: putStmnt_LOrI(); break;

      case IR::Code::ModI_W: putStmnt_ModI_W(); break;
      case IR::Code::ModU_W: putStmnt_ModU_W(); break;
      case IR::Code::Move_B: putStmnt_Move_B(); break;
      case IR::Code::Move_W: putStmnt_Move_W(); break;
      case IR::Code::MuXU_W: putStmnt_MuXU_W(); break;
      case IR::Code::MulF_W: putStmnt_MulF_W(); break;
      case IR::Code::MulI_W: putStmnt_MulI_W(); break;
      case IR::Code::MulU_W: putStmnt_MulU_W(); break;
      case IR::Code::NegF_W: putStmnt_NegF_W(); break;
      case IR::Code::NegI_W: putStmnt_NegI_W(); break;

      case IR::Code::Pltn: putStmnt_Pltn(); break;
      case IR::Code::Retn: putStmnt_Retn(); break;

      case IR::Code::ShLU_W: putStmnt_ShLU_W(); break;
      case IR::Code::ShRI_W: putStmnt_ShRI_W(); break;
      case IR::Code::ShRU_W: putStmnt_ShRU_W(); break;
      case IR::Code::SuXU_W: putStmnt_SuXU_W(); break;
      case IR::Code::SubF_W: putStmnt_SubF_W(); break;
      case IR::Code::SubI_W: putStmnt_SubI_W(); break;
      case IR::Code::SubU_W: putStmnt_SubU_W(); break;
      case IR::Code::Swap_W: putStmnt_Swap_W(); break;

      default:
         std::cerr << "ERROR: " << stmnt->pos
            << ": cannot output Code for DGE: " << stmnt->op << '\n';
         throw EXIT_FAILURE;
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
            putCode("Push_Lit", GetWord(a.idx->aLit) + (a.off + w) * 4);
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
               putCode("Push_Lit", (a.off + w) * 4);
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
         putCode("Drop_Reg", GetWord(a.idx->aLit) + (a.off + w) * 4);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode("Push_Lit", GetWord(a.idx->aLit.value) + (a.off + w) * 4);
            putCode("Drop_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode("Push_Lit", (a.off + w) * 4);
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
         throw Core::ExceptStr(stmnt->pos, "bad putStmntDropArg");
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
            putCode("Push_Lit", GetWord(a.idx->aLit) + (a.off + w) * 4);
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
               putCode("Push_Lit", (a.off + w) * 4);
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
         putCode("Push_Reg", GetWord(a.idx->aLit) + (a.off + w) * 4);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode("Push_Lit", GetWord(a.idx->aLit.value) + (a.off + w) * 4);
            putCode("Push_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode("Push_Lit", (a.off + w) * 4);
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
         throw Core::ExceptStr(stmnt->pos, "bad putStmntPushArg");
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
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->op), stmnt->op.size * 2);
   }

   //
   // Info::putStmntStkCmp
   //
   void Info::putStmntStkCmp(int res0, char const *code)
   {
      if(stmnt->op.size == 0)
         return putCode("Push_Lit", res0);

      if(stmnt->op.size == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->op), stmnt->op.size * 2);
   }

   //
   // Info::putStmntStkUna
   //
   void Info::putStmntStkUna(char const *code)
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->op), stmnt->op.size);
   }
}

// EOF

