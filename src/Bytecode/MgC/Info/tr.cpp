//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageCraft IR code translation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/MgC/Info.hpp"

#include "IR/Block.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace MgC
      {
         //
         // Info::trStmnt
         //
         void Info::trStmnt()
         {
            switch(stmnt->op.code)
            {
            case IR::Code::Nop:
               break;

            case IR::Code::AddU_W:
            case IR::Code::AndU_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_GE_W:
            case IR::Code::CmpU_GT_W:
            case IR::Code::CmpU_LE_W:
            case IR::Code::CmpU_LT_W:
            case IR::Code::CmpU_NE_W:
            case IR::Code::MulU_W:
            case IR::Code::ShRU_W:
            case IR::Code::SubU_W:
               trStmntMath();
               break;

            case IR::Code::Call:
               trStmnt_Call();
               break;

            case IR::Code::Jcnd_Nil:
            case IR::Code::Jcnd_Tru:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Lit);
               break;

            case IR::Code::Jump:
               trStmnt_Jump();
               break;

            case IR::Code::Move_W:
               trStmnt_Move_W();
               break;

            case IR::Code::NotU_W:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               break;

            case IR::Code::Retn:
               for(auto i = stmnt->args.size(); i--;)
                  CheckArgB(stmnt, i, IR::ArgBase::Stk);
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": cannot translate Code for MgC: "
                  << stmnt->op << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::trStmntMath
         //
         // Translates a Code(dst, src, src) instruction to be stack-based.
         //
         void Info::trStmntMath()
         {
            auto newStmnt = stmnt;

            CheckArgC(stmnt, 3);

            if(stmnt->args[0].a != IR::ArgBase::Stk)
            {
               block->addStatementArgs(stmnt->next, {IR::Code::Move_W, 1},
                  std::move(stmnt->args[0]), IR::Arg_Stk());
               stmnt->args[0] = IR::Arg_Stk();
            }

            // This is actually a little bad because it can break partially-stack
            // operations that are not reversible. Such as:
            //   SubU_W, Stk(), Lit(1), Stk()
            // Which will end up turning 1-stk into stk-1. As always, care must be
            // taken when writing assembly.
            for(unsigned i = 3; --i != 0;)
               if(stmnt->args[i].a != IR::ArgBase::Stk)
            {
               block->addStatementArgs(newStmnt, {IR::Code::Move_W, 1},
                  IR::Arg_Stk(), std::move(stmnt->args[i]));
               stmnt->args[i] = IR::Arg_Stk();
               newStmnt = newStmnt->prev;
            }

            if(stmnt != newStmnt)
            {
               // Transfer labels.
               newStmnt->labs = std::move(stmnt->labs);
               stmnt->labs = Core::Array<Core::String>();

               // Reset iterator.
               stmnt = newStmnt->prev;
            }
         }

         //
         // Info::trStmnt_Call
         //
         void Info::trStmnt_Call()
         {
            CheckArgC(stmnt, 1);

            for(auto i = stmnt->args.size(); i-- != 1;)
               CheckArgB(stmnt, i, IR::ArgBase::Stk);

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
               throw EXIT_FAILURE;

            case IR::ArgBase::Stk:
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Arg for Code::Call[0]: "
                  << stmnt->args[0].a << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::trStmnt_Jump
         //
         void Info::trStmnt_Jump()
         {
            CheckArgC(stmnt, 1);

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit: break;
            case IR::ArgBase::Stk: break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Arg for Code::Jump[0]: "
                  << stmnt->args[0].a << '\n';
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

            // MageCraft has no direct memory<->memory move ops, so if this
            // instruction is one, change it to a stack intermediary.
            if(stmnt->args[0].a != IR::ArgBase::Stk &&
               stmnt->args[1].a != IR::ArgBase::Stk)
            {
               block->addStatementArgs(stmnt->next, {IR::Code::Move_W, 1},
                  std::move(stmnt->args[0]), IR::Arg_Stk());
               stmnt->args[0] = IR::Arg_Stk();
            }

            switch(stmnt->args[0].a)
            {
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
               case IR::ArgBase::Lit:    break;
               case IR::ArgBase::LocReg: break;
               case IR::ArgBase::Sta:    break;
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

            case IR::ArgBase::Sta:
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
      }
   }
}

// EOF

