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

namespace GDCC
{
   namespace BC
   {
      namespace DGE
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
            case IR::Code::AddU_W: putCode("AddU"); break;
            case IR::Code::AndU_W: putCode("AndU"); break;

            case IR::Code::CmpI_LE_W: putCode("CmpI_LE"); break;
            case IR::Code::CmpI_LT_W: putCode("CmpI_LT"); break;
            case IR::Code::CmpI_GE_W: putCode("CmpI_GE"); break;
            case IR::Code::CmpI_GT_W: putCode("CmpI_GT"); break;
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

            case IR::Code::Jump:
               if(stmnt->args[0].a == IR::ArgBase::Lit)
                  putCode("Jump_Lit", stmnt->args[0].aLit);
               else
                  putCode("Jump");
               break;

            case IR::Code::ModI_W: putCode("ModI"); break;
            case IR::Code::ModU_W: putCode("ModU"); break;

            case IR::Code::Move_W: putStmnt_Move_W(); break;

            case IR::Code::MulU_W: putCode("MulU"); break;
            case IR::Code::NotU_W: putCode("LNot"); break;
            case IR::Code::OrIU_W: putCode("OrIU"); break;
            case IR::Code::OrXU_W: putCode("OrXU"); break;

            case IR::Code::Retn: putStmnt_Retn(); break;

            case IR::Code::SubU_W: putCode("SubU"); break;
            case IR::Code::Swap_W: putCode("Swap"); break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot output Code for DGE: " << stmnt->op << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Move_W
         //
         void Info::putStmnt_Move_W()
         {
            // Multi-word?
            if(stmnt->op.size != 1)
            {
               // TODO
               throw Core::ExceptStr(stmnt->pos, "multiword Move_W");
            }

            // push_?
            if(stmnt->args[0].a == IR::ArgBase::Stk) switch(stmnt->args[1].a)
            {
            case IR::ArgBase::Sta:
               if(stmnt->args[1].aSta.off)
               {
                  putCode("Push_Lit", stmnt->args[1].aSta.off);
                  putCode("AddU");
               }

               putCode("Push_Ptr");
               break;

            default:
               putStmntPushArg(stmnt->args[1], 0);
               break;
            }

            // drop_?
            else if(stmnt->args[1].a == IR::ArgBase::Stk) switch(stmnt->args[0].a)
            {
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
         // Info::putStmntDropArg
         //
         void Info::putStmntDropArg(IR::Arg const &arg, Core::FastU w)
         {
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

            switch(arg.a)
            {
            case IR::ArgBase::LocReg: putReg(arg.aLocReg); break;
            case IR::ArgBase::Nul:    putCode("Drop_Nul"); break;
            case IR::ArgBase::Sta:    putSta(arg.aSta); break;

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

            switch(arg.a)
            {
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
   }
}

// EOF

