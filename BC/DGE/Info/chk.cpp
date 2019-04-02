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

      switch(stmnt->code.base)
      {
      case IR::CodeBase::AddX: chkStmnt_AddX(); break;
      case IR::CodeBase::Call: chkStmnt_Call(); break;
      case IR::CodeBase::Cnat: chkStmnt_Cnat(); break;
      case IR::CodeBase::Copy: chkStmnt_Copy(); break;

      case IR::CodeBase::Jcnd_Nil: chkStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: chkStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: chkStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jfar_Pro: chkStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: chkStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: chkStmnt_Jfar_Sta(); break;

      case IR::CodeBase::Move: chkStmnt_Move(); break;
      case IR::CodeBase::SubX: chkStmnt_SubX(); break;
      case IR::CodeBase::Swap: chkStmnt_Swap(); break;

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

