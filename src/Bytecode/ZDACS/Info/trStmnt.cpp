//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement translation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::trStmnt
         //
         void Info::trStmnt()
         {
            switch(stmnt->code)
            {
            case IR::Code::Nop:
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:
            case IR::Code::AndU_W:
            case IR::Code::DivI_W:
            case IR::Code::DivX_W:
            case IR::Code::ModI_W:
            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
            case IR::Code::MulX_W:
            case IR::Code::OrIU_W:
            case IR::Code::OrXU_W:
            case IR::Code::ShLU_W:
            case IR::Code::ShRI_W:
            case IR::Code::SubI_W:
            case IR::Code::SubU_W:
               CheckArgC(stmnt, 3);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               CheckArgB(stmnt, 2, IR::ArgBase::Stk);
               break;

            case IR::Code::Call:
               trStmnt_Call();
               break;

            case IR::Code::Casm:
               CheckArgC(stmnt, 1);
               for(auto n = stmnt->args.size(); --n;)
                  CheckArgB(stmnt, n, IR::ArgBase::Lit);
               break;

            case IR::Code::Cjmp_Nil:
            case IR::Code::Cjmp_Tru:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Lit);
               break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpI_GE_W:
            case IR::Code::CmpI_GT_W:
            case IR::Code::CmpI_LE_W:
            case IR::Code::CmpI_LT_W:
            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_NE_W:
               CheckArgC(stmnt, 3);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               CheckArgB(stmnt, 2, IR::ArgBase::Stk);
               break;

            case IR::Code::Cnat:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               CheckArgB(stmnt, 1, IR::ArgBase::Lit);
               for(auto n = stmnt->args.size(); n-- != 2;)
                  CheckArgB(stmnt, n, IR::ArgBase::Stk);
               break;

            case IR::Code::Cspe:
               trStmnt_Cspe();
               break;

            case IR::Code::InvU_W:
            case IR::Code::NegI_W:
            case IR::Code::NotU_W:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               break;

            case IR::Code::Jump:
               CheckArgC(stmnt, 1);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               break;

            case IR::Code::Move_W:
               trStmnt_Move_W();
               break;

            case IR::Code::Plsa:
               CheckArgC(stmnt, 1);
               CheckArgB(stmnt, 0, IR::ArgBase::Lit);
               break;

            case IR::Code::Plsd:
            case IR::Code::Plsi:
            case IR::Code::Plsr:
               CheckArgC(stmnt, 1);
               moveArgStk_W_src(stmnt->args[0]);
               break;

            case IR::Code::Plsf:
               break;

            case IR::Code::Plss:
               CheckArgC(stmnt, 1);
               moveArgStk_W_dst(stmnt->args[0]);
               break;

            case IR::Code::Pltn:
               CheckArgC(stmnt, 2);
               moveArgStk_W_dst(stmnt->args[0]);
               moveArgStk_W_src(stmnt->args[1]);
               break;

            case IR::Code::Retn:
               trStmnt_Retn();
               break;

            case IR::Code::Swap_W:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": cannot translate Code for ZDACS: "
                  << stmnt->code << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::trStmnt_Call
         //
         void Info::trStmnt_Call()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);
            for(auto n = stmnt->args.size(); --n != 1;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);

            auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               if(ret != 0 && ret != 1)
               {
                  std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
                  throw EXIT_FAILURE;
               }

               break;

            case IR::ArgBase::Stk:
               if(ret != 0 && ret != 1)
               {
                  std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
                  throw EXIT_FAILURE;
               }

               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Call\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::trStmnt_Cspe
         //
         void Info::trStmnt_Cspe()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);

            auto ret = ResolveValue(stmnt->args[1].aLit.value->getValue());

            if(ret > 1)
            {
               std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe ret\n";
               throw EXIT_FAILURE;
            }

            // Too many call args.
            if(stmnt->args.size() > 7)
            {
               std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe arg count\n";
               throw EXIT_FAILURE;
            }

            // No call args.
            if(stmnt->args.size() == 2)
               return;

            switch(stmnt->args[2].a)
            {
            case IR::ArgBase::Lit:
               for(auto n = stmnt->args.size(); n-- != 3;)
                  CheckArgB(stmnt, n, IR::ArgBase::Lit);
               break;

            case IR::ArgBase::Stk:
               for(auto n = stmnt->args.size(); n-- != 3;)
                  CheckArgB(stmnt, n, IR::ArgBase::Stk);
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::trStmnt_Move_W
         //
         void Info::trStmnt_Move_W()
         {
            CheckArgC(stmnt, 2);
            CheckArg(stmnt->args[0], stmnt->pos);
            CheckArg(stmnt->args[1], stmnt->pos);

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblArr:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  moveArgStk_W_src(*stmnt->args[0].aGblArr.idx);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::GblReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::Loc:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  moveArgStk_W_src(*stmnt->args[0].aLoc.idx);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::LocArs:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  moveArgStk_W_src(*stmnt->args[0].aLocArs.idx);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::LocReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::MapArr:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  moveArgStk_W_src(*stmnt->args[0].aMapArr.idx);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::MapReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::Nul:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::Stk:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::GblReg: break;
               case IR::ArgBase::LocReg: break;
               case IR::ArgBase::MapReg: break;
               case IR::ArgBase::WldReg: break;

               case IR::ArgBase::Lit:    break;

               case IR::ArgBase::Loc:
                  moveArgStk_W_src(*stmnt->args[1].aLoc.idx);
                  break;

               case IR::ArgBase::LocArs:
                  moveArgStk_W_src(*stmnt->args[1].aLocArs.idx);
                  break;

               case IR::ArgBase::GblArr:
                  moveArgStk_W_src(*stmnt->args[1].aGblArr.idx);
                  break;

               case IR::ArgBase::MapArr:
                  moveArgStk_W_src(*stmnt->args[1].aMapArr.idx);
                  break;

               case IR::ArgBase::WldArr:
                  moveArgStk_W_src(*stmnt->args[1].aWldArr.idx);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::WldArr:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  moveArgStk_W_src(*stmnt->args[0].aWldArr.idx);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::WldReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: break;
               default: goto badcase;
               }
               break;

            default:
            badcase:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Move_W("
                  << stmnt->args[0].a << ',' << stmnt->args[1].a << ")\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::trStmnt_Retn
         //
         void Info::trStmnt_Retn()
         {
            auto argc = stmnt->args.size();

            for(auto n = argc; n--;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);

            switch(func->ctype)
            {
            case IR::CallType::LangACS:
            case IR::CallType::LangC:
               if(argc != 0 && argc != 1)
               {
                  std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
                  throw EXIT_FAILURE;
               }
               break;

            case IR::CallType::Script:
            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               if(argc != 0 && argc != 1)
               {
                  std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
                  throw EXIT_FAILURE;
               }
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Retn\n";
               throw EXIT_FAILURE;
            }
         }
      }
   }
}

// EOF

