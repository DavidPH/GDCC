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
// Static Functions                                                           |
//

//
// CheckArg
//
// Checks Arg properties common to all uses for the target.
//
static void CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin const &pos)
{
   switch(arg.a)
   {
   case GDCC::IR::ArgBase::Lit: break;
   case GDCC::IR::ArgBase::Nul: break;
   case GDCC::IR::ArgBase::Stk: break;

   case GDCC::IR::ArgBase::LocArs:
      CheckArg(*arg.aLocArs.idx, pos);
      break;

   case GDCC::IR::ArgBase::LocReg:
      if(arg.aLocReg.idx->a != GDCC::IR::ArgBase::Lit)
      {
         std::cerr << "ERROR: " << pos << ": LocReg.idx must be Lit\n";
         throw EXIT_FAILURE;
      }
      break;

   default:
      std::cerr << "ERROR: " << pos << ": MgC cannot use Arg: " << arg.a << '\n';
      throw EXIT_FAILURE;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace MgC
   {
      //
      // Info::translateStatement
      //
      void Info::translateStatement(GDCC::IR::Statement &stmnt)
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
            translateStatement_Call(stmnt);
            break;

         case GDCC::IR::Code::Cjmp_Nil:
         case GDCC::IR::Code::Cjmp_Tru:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);
            break;

         case GDCC::IR::Code::Jump:
            translateStatement_Jump(stmnt);
            break;

         case GDCC::IR::Code::Move_W:
            translateStatement_Move_W(stmnt);
            break;

         case GDCC::IR::Code::NotU_W:
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Stk);
            break;

         case GDCC::IR::Code::Retn:
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, GDCC::IR::ArgBase::Stk);
            break;

         default:
            std::cerr << "ERROR: " << stmnt.pos << ": cannot translate Code for MgC: "
               << stmnt.code << '\n';
            throw EXIT_FAILURE;
         }
      }

      //
      // Info::translateStatement_Call
      //
      void Info::translateStatement_Call(GDCC::IR::Statement &stmnt)
      {
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 1, GDCC::IR::ArgBase::Lit);

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
      // Info::translateStatement_Jump
      //
      void Info::translateStatement_Jump(GDCC::IR::Statement &stmnt)
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
      // Info::translateStatement_Move_W
      //
      void Info::translateStatement_Move_W(GDCC::IR::Statement &stmnt)
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

