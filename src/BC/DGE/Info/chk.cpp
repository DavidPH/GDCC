//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati check functions.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::chkStmnt
   //
   void Info::chkStmnt()
   {
      InfoBase::chkStmnt();

      switch(stmnt->code)
      {
      case IR::Code::AdXU: chkStmnt_AdXU(); break;
      case IR::Code::Call: chkStmnt_Call(); break;
      case IR::Code::Cnat: chkStmnt_Cnat(); break;
      case IR::Code::Copy: chkStmnt_Copy(); break;

      case IR::Code::Jcnd_Nil: chkStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: chkStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: chkStmnt_Jcnd_Tru(); break;
      case IR::Code::Jfar_Pro: chkStmnt_Jfar_Pro(); break;
      case IR::Code::Jfar_Set: chkStmnt_Jfar_Set(); break;
      case IR::Code::Jfar_Sta: chkStmnt_Jfar_Sta(); break;

      case IR::Code::Move: chkStmnt_Move(); break;
      case IR::Code::SuXU: chkStmnt_SuXU(); break;
      case IR::Code::Swap: chkStmnt_Swap(); break;

      default:
         break;
      }
   }

   //
   // Info::chkStmntArg
   //
   void Info::chkStmntArg(IR::Arg const &arg)
   {
      InfoBase::chkStmntArg(arg);

      switch(arg.a)
      {
      case IR::ArgBase::Aut:
         chkStmntArg(*arg.aAut.idx);
         break;

      case IR::ArgBase::Lit: break;
      case IR::ArgBase::Nul: break;
      case IR::ArgBase::Stk: break;

      case IR::ArgBase::LocReg:
         chkStmntArgI(arg.aLocReg, IR::ArgBase::Lit);
         break;

      case IR::ArgBase::Sta:
         chkStmntArg(*arg.aSta.idx);
         break;

      case IR::ArgBase::StrArs:
         chkStmntArg(*arg.aStrArs.idx);
         break;

      default:
         IR::ErrorCode(stmnt, "unsupported Arg: ", arg.a);
      }
   }
}

// EOF

