//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic check functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "IR/Exception.hpp"
#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::chkStmnt
   //
   void Info::chkStmnt()
   {
      for(auto &arg : stmnt->args)
         chkStmntArg(arg);

      switch(stmnt->code)
      {
      case IR::Code::Nop:
      case IR::Code::Xcod_SID:
         chkStmntArgC(0);
         break;

      case IR::Code::Retn:
         chkStmntArgC(0, 1);
         break;

      case IR::Code::Jdyn:
      case IR::Code::Jump:
         chkStmntArgC(1);
         break;

      case IR::Code::BNot:
      case IR::Code::Bclo:
      case IR::Code::Bclz:
      case IR::Code::Copy:
      case IR::Code::Jcnd_Nil:
      case IR::Code::Jcnd_Tru:
      case IR::Code::Jfar_Pro:
      case IR::Code::Jfar_Set:
      case IR::Code::LNot:
      case IR::Code::Move:
      case IR::Code::NegF:
      case IR::Code::NegI:
      case IR::Code::Pltn:
      case IR::Code::Swap:
         chkStmntArgC(2);
         break;

      case IR::Code::Call:
      case IR::Code::Casm:
      case IR::Code::Cnat:
      case IR::Code::Cscr_IA:
      case IR::Code::Cscr_SA:
      case IR::Code::Cspe:
         chkStmntArgC(2, -1);
         break;

      case IR::Code::AddF:
      case IR::Code::AddI:
      case IR::Code::AddU:
      case IR::Code::BAnd:
      case IR::Code::BOrI:
      case IR::Code::BOrX:
      case IR::Code::CmpF_EQ:
      case IR::Code::CmpF_GE:
      case IR::Code::CmpF_GT:
      case IR::Code::CmpF_LE:
      case IR::Code::CmpF_LT:
      case IR::Code::CmpF_NE:
      case IR::Code::CmpI_EQ:
      case IR::Code::CmpI_GE:
      case IR::Code::CmpI_GT:
      case IR::Code::CmpI_LE:
      case IR::Code::CmpI_LT:
      case IR::Code::CmpI_NE:
      case IR::Code::CmpU_EQ:
      case IR::Code::CmpU_GE:
      case IR::Code::CmpU_GT:
      case IR::Code::CmpU_LE:
      case IR::Code::CmpU_LT:
      case IR::Code::CmpU_NE:
      case IR::Code::DiXI:
      case IR::Code::DiXU:
      case IR::Code::DivA:
      case IR::Code::DivF:
      case IR::Code::DivI:
      case IR::Code::DivK:
      case IR::Code::DivR:
      case IR::Code::DivU:
      case IR::Code::DivX:
      case IR::Code::Jfar_Sta:
      case IR::Code::LAnd:
      case IR::Code::LOrI:
      case IR::Code::ModI:
      case IR::Code::ModU:
      case IR::Code::MuXU:
      case IR::Code::MulA:
      case IR::Code::MulF:
      case IR::Code::MulI:
      case IR::Code::MulK:
      case IR::Code::MulR:
      case IR::Code::MulU:
      case IR::Code::MulX:
      case IR::Code::ShLF:
      case IR::Code::ShLU:
      case IR::Code::ShRF:
      case IR::Code::ShRI:
      case IR::Code::ShRU:
      case IR::Code::SubF:
      case IR::Code::SubI:
      case IR::Code::SubU:
         chkStmntArgC(3);
         break;

      case IR::Code::Cscr_IS:
      case IR::Code::Cscr_SS:
         chkStmntArgC(3, -1);
         break;

      case IR::Code::AdXU:
      case IR::Code::SuXU:
         chkStmntArgC(3, 4);
         break;

      case IR::Code::Bges:
      case IR::Code::Bget:
      case IR::Code::Bset:
         chkStmntArgC(4);
         break;

      case IR::Code::Jcnd_Tab:
         if(stmnt->args.size() % 2 == 0)
            IR::ErrorCode(stmnt, "bad arg count");
         break;

      case IR::Code::None:
         IR::ErrorCode(stmnt, "bad code");
      }
   }

   //
   // Info::chkStmntArg
   //
   void Info::chkStmntArg(IR::Arg const &arg)
   {
      if(arg.a == IR::ArgBase::Gen || arg.a == IR::ArgBase::Cpy)
         IR::ErrorCode(stmnt, "bad arg type");
   }

   //
   // Info::chkStmntArgA
   //
   void Info::chkStmntArgA(IR::ArgPtr2 const &arg, IR::ArgBase a)
   {
      if(arg.arr->a != a)
         IR::ErrorCode(stmnt, ".arr must be ", a);
   }

   //
   // Info::chkStmntArgB
   //
   void Info::chkStmntArgB(Core::FastU n, IR::ArgBase a)
   {
      if(stmnt->args[n].a != a)
         IR::ErrorCode(stmnt, "arg[", n, "] must be ", a);
   }

   //
   // Info::chkStmntArgC
   //
   void Info::chkStmntArgC(Core::FastU min, Core::FastU max)
   {
      if(stmnt->args.size() < min || stmnt->args.size() > max)
         IR::ErrorCode(stmnt, "bad arg count");
   }

   //
   // Info::chkStmntArgI
   //
   void Info::chkStmntArgI(IR::ArgPtr1 const &arg, IR::ArgBase a)
   {
      if(arg.idx->a != a)
         IR::ErrorCode(stmnt, ".idx must be ", a);
   }

   //
   // Info::chkStmntArgI
   //
   void Info::chkStmntArgI(IR::ArgPtr2 const &arg, IR::ArgBase a)
   {
      if(arg.idx->a != a)
         IR::ErrorCode(stmnt, ".idx must be ", a);
   }
}

// EOF

