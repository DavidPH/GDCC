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

//
// CheckArgB
//
static void CheckArgB(GDCC::IR::Statement &stmnt, std::size_t a, GDCC::IR::ArgBase b)
{
   if(stmnt.args[a].a != b)
   {
      std::cerr << "ERROR: " << stmnt.pos << ": " << stmnt.code
         << " must have " << b << " args[" << a << "]\n";
      throw EXIT_FAILURE;
   }
}

//
// CheckArgC
//
static void CheckArgC(GDCC::IR::Statement &stmnt, std::size_t c)
{
   if(stmnt.args.size() < 2)
   {
      std::cerr << "ERROR: " << stmnt.pos << ": bad argc for " << stmnt.code
         << ": " << stmnt.args.size() << " < " << c << '\n';
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
      // Info::translateBlock
      //
      void Info::translateBlock(GDCC::IR::Block &block)
      {
         for(auto &stmnt : block)
            translateStatement(stmnt);
      }

      //
      // Info::translateStatement
      //
      void Info::translateStatement(GDCC::IR::Statement &stmnt)
      {
         switch(stmnt.code)
         {
         case GDCC::IR::Code::Nop:
            break;

         case GDCC::IR::Code::Call:
            translateStatement_Call(stmnt);
            break;

         case GDCC::IR::Code::Move_W:
            translateStatement_Move_W(stmnt);
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
               << stmnt.args[0].a << ',' << stmnt.args[1].a << '\n';
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

