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

#include "Info.hpp"

#include "GDCC/IR/Block.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace MgC
   {
      //
      // Info::trStmnt
      //
      void Info::trStmnt(GDCC::IR::Statement &stmnt)
      {
         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            break;

         case GDCC::IR::Code::AddU_W:
         case GDCC::IR::Code::CmpU_EQ_W:
         case GDCC::IR::Code::CmpU_GE_W:
         case GDCC::IR::Code::CmpU_GT_W:
         case GDCC::IR::Code::CmpU_LE_W:
         case GDCC::IR::Code::CmpU_LT_W:
         case GDCC::IR::Code::CmpU_NE_W:
         case GDCC::IR::Code::SubU_W:
            CheckArgC(stmnt, 3);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 2, GDCC::IR::ArgBase::Stk);
            break;

         case GDCC::IR::Code::Call:
            trStmnt_Call(stmnt);
            break;

         case GDCC::IR::Code::Cjmp_Nil:
         case GDCC::IR::Code::Cjmp_Tru:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);
            break;

         case GDCC::IR::Code::Jump:
            trStmnt_Jump(stmnt);
            break;

         case GDCC::IR::Code::Move_W:
            trStmnt_Move_W(stmnt);
            break;

         case GDCC::IR::Code::NotU_W:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            break;

         case GDCC::IR::Code::Retn:
            CheckArgC(stmnt, 1);
            for(auto i = stmnt.args.size(); i--;)
               CheckArgB(stmnt, i, GDCC::IR::ArgBase::Stk);
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": cannot translate Code for MgC: "
               << stmnt.code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::trStmnt_Call
      //
      void Info::trStmnt_Call(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);

         for(auto i = stmnt.args.size(); i-- != 2;)
            CheckArgB(stmnt, i, GDCC::IR::ArgBase::Stk);

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Lit:
            std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
            throw EXIT_FAILURE;

         case GDCC::IR::ArgBase::Stk:
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Arg for Code::Call[0]: "
               << stmnt.args[0].a << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::trStmnt_Jump
      //
      void Info::trStmnt_Jump(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 1);

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Lit: break;
         case GDCC::IR::ArgBase::Stk: break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Arg for Code::Jump[0]: "
               << stmnt.args[0].a << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::trStmnt_Move_W
      //
      void Info::trStmnt_Move_W(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArg(stmnt.args[0], stmnt.pos);
         CheckArg(stmnt.args[1], stmnt.pos);

         switch(stmnt.args[0].a)
         {
         case GDCC::IR::ArgBase::Nul:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::Stk:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Lit:    break;
            case GDCC::IR::ArgBase::LocArs: break;
            case GDCC::IR::ArgBase::LocReg: break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::LocArs:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: break;
            default: goto badcase;
            }
            break;

         case GDCC::IR::ArgBase::LocReg:
            switch(stmnt.args[1].a)
            {
            case GDCC::IR::ArgBase::Stk: break;
            default: goto badcase;
            }
            break;

         default:
         badcase:
            std::cerr << "ERROR: " << stmnt.pos << ": bad Code::Move_W("
               << stmnt.args[0].a << ',' << stmnt.args[1].a << ")\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

