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

#include "IR/Exception.hpp"
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

      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop: break;

      case IR::CodeBase::Add:      putStmnt_Add(); break;
      case IR::CodeBase::AddX:     putStmnt_AddX(); break;
      case IR::CodeBase::BAnd:     putStmnt_BAnd(); break;
      case IR::CodeBase::BNot:     putStmnt_BNot(); break;
      case IR::CodeBase::BOrI:     putStmnt_BOrI(); break;
      case IR::CodeBase::BOrX:     putStmnt_BOrX(); break;
      case IR::CodeBase::Bclo:     putStmnt_Bclo(); break;
      case IR::CodeBase::Bclz:     putStmnt_Bclz(); break;
      case IR::CodeBase::CmpEQ:    putStmnt_CmpEQ(); break;
      case IR::CodeBase::CmpGE:    putStmnt_CmpGE(); break;
      case IR::CodeBase::CmpGT:    putStmnt_CmpGT(); break;
      case IR::CodeBase::CmpLE:    putStmnt_CmpLE(); break;
      case IR::CodeBase::CmpLT:    putStmnt_CmpLT(); break;
      case IR::CodeBase::CmpNE:    putStmnt_CmpNE(); break;
      case IR::CodeBase::Call:     putStmnt_Call(); break;
      case IR::CodeBase::Cnat:     putStmnt_Cnat(); break;
      case IR::CodeBase::Copy:     putStmnt_Copy(); break;
      case IR::CodeBase::Div:      putStmnt_Div(); break;
      case IR::CodeBase::DivX:     putStmnt_DivX(); break;
      case IR::CodeBase::Jcnd_Nil: putStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: putStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: putStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jfar_Pro: putStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: putStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: putStmnt_Jfar_Sta(); break;
      case IR::CodeBase::Jump:     putStmnt_Jump(); break;
      case IR::CodeBase::LAnd:     putStmnt_LAnd(); break;
      case IR::CodeBase::LNot:     putStmnt_LNot(); break;
      case IR::CodeBase::LOrI:     putStmnt_LOrI(); break;
      case IR::CodeBase::Mod:      putStmnt_Mod(); break;
      case IR::CodeBase::Move:     putStmnt_Move(); break;
      case IR::CodeBase::Mul:      putStmnt_Mul(); break;
      case IR::CodeBase::MulX:     putStmnt_MulX(); break;
      case IR::CodeBase::Neg:      putStmnt_Neg(); break;
      case IR::CodeBase::Pltn:     putStmnt_Pltn(); break;
      case IR::CodeBase::Retn:     putStmnt_Retn(); break;
      case IR::CodeBase::Rjnk:     putStmnt_Rjnk(); break;
      case IR::CodeBase::ShL:      putStmnt_ShL(); break;
      case IR::CodeBase::ShR:      putStmnt_ShR(); break;
      case IR::CodeBase::Sub:      putStmnt_Sub(); break;
      case IR::CodeBase::SubX:     putStmnt_SubX(); break;
      case IR::CodeBase::Swap:     putStmnt_Swap(); break;
      case IR::CodeBase::Tr:       putStmnt_Tr(); break;

      default:
         IR::ErrorCode(stmnt, "unsupported put");
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
   void Info::putStmntStkBin(IR::CodeType type, char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code.base+type, n), n * 2);
   }

   //
   // Info::putStmntStkCmp
   //
   void Info::putStmntStkCmp(int res0, IR::CodeType type, char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return putCode("Push_Lit", res0);

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code.base+type, n), n * 2);
   }

   //
   // Info::putStmntStkShi
   //
   void Info::putStmntStkShi(IR::CodeType type, char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code.base+type, n), n + 1);
   }

   //
   // Info::putStmntStkUna
   //
   void Info::putStmntStkUna(IR::CodeType type, char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1 && code)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code.base+type, n), n);
   }
}

// EOF

