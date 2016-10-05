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

#include "IR/Statement.hpp"

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
      case IR::Code::AddI_W:
      case IR::Code::AddU_W: putCode("AddU"); break;
      case IR::Code::AndU_W: putCode("AndU"); break;

      case IR::Code::CmpI_EQ_W: putCode("CmpU_EQ"); break;
      case IR::Code::CmpI_LE_W: putCode("CmpI_LE"); break;
      case IR::Code::CmpI_LT_W: putCode("CmpI_LT"); break;
      case IR::Code::CmpI_GE_W: putCode("CmpI_GE"); break;
      case IR::Code::CmpI_GT_W: putCode("CmpI_GT"); break;
      case IR::Code::CmpI_NE_W: putCode("CmpU_NE"); break;
      case IR::Code::CmpU_EQ_W: putCode("CmpU_EQ"); break;
      case IR::Code::CmpU_LE_W: putCode("CmpU_LE"); break;
      case IR::Code::CmpU_LT_W: putCode("CmpU_LT"); break;
      case IR::Code::CmpU_GE_W: putCode("CmpU_GE"); break;
      case IR::Code::CmpU_GT_W: putCode("CmpU_GT"); break;
      case IR::Code::CmpU_NE_W: putCode("CmpU_NE"); break;

      case IR::Code::Call: putStmnt_Call(); break;
      case IR::Code::Cnat: putStmnt_Cnat(); break;

      case IR::Code::DivI_W: putCode("DivI"); break;
      case IR::Code::DivU_W: putCode("DivU"); break;

      case IR::Code::Jcnd_Nil: putCode("Jcnd_Nil", stmnt->args[1].aLit); break;
      case IR::Code::Jcnd_Tru: putCode("Jcnd_Tru", stmnt->args[1].aLit); break;

      case IR::Code::Jfar: putStmnt_Jfar(); break;
      case IR::Code::Jump: putStmnt_Jump(); break;

      case IR::Code::ModI_W: putCode("ModI"); break;
      case IR::Code::ModU_W: putCode("ModU"); break;

      case IR::Code::Move_B: putStmnt_Move_B(); break;
      case IR::Code::Move_W: putStmnt_Move_W(); break;

      case IR::Code::MulU_W: putCode("MulU"); break;
      case IR::Code::NotU_W: putCode("LNot"); break;
      case IR::Code::OrIU_W: putCode("OrIU"); break;
      case IR::Code::OrXU_W: putCode("OrXU"); break;

      case IR::Code::Pltn:
         putCode("Push_Reg", getStkPtrIdx());
         putCode("AddU");
         break;

      case IR::Code::Retn: putStmnt_Retn(); break;

      case IR::Code::SubI_W:
      case IR::Code::SubU_W: putCode("SubU"); break;
      case IR::Code::Swap_W: putCode("Swap"); break;

      default:
         std::cerr << "ERROR: " << stmnt->pos
            << ": cannot output Code for DGE: " << stmnt->op << '\n';
         throw EXIT_FAILURE;
      }
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
            putCode("Push_Lit", GetWord(a.idx->aLit) + a.off + w);
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
               putCode("Push_Lit", a.off + w);
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
         putCode("Drop_Reg", GetWord(a.idx->aLit) + a.off + w);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode("Push_Lit", GetWord(a.idx->aLit.value) + a.off + w);
            putCode("Drop_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode("Push_Lit", a.off + w);
               putCode("AddU");
            }

            putCode("Drop_Ptr");
         }
      };

      // Offset is in words, but output offset is in bytes.
      w *= 4;

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
            putCode("Push_Lit", GetWord(a.idx->aLit) + a.off + w);
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
               putCode("Push_Lit", a.off + w);
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
         putCode("Push_Reg", GetWord(a.idx->aLit) + a.off + w);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode("Push_Lit", GetWord(a.idx->aLit.value) + a.off + w);
            putCode("Push_Ptr");
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode("Push_Lit", a.off + w);
               putCode("AddU");
            }

            putCode("Push_Ptr");
         }
      };

      // Offset is in words, but output offset is in bytes.
      w *= 4;

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
}

// EOF

