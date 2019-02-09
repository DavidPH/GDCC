//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDACS check functions.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::chkStmnt
   //
   void Info::chkStmnt()
   {
      InfoBase::chkStmnt();

      switch(stmnt->code.base)
      {
      case IR::CodeBase::AddX:     chkStmnt_AddX(); break;
      case IR::CodeBase::Bges:     chkStmnt_Bges(); break;
      case IR::CodeBase::Bget:     chkStmnt_Bget(); break;
      case IR::CodeBase::Bset:     chkStmnt_Bset(); break;
      case IR::CodeBase::Call:     chkStmnt_Call(); break;
      case IR::CodeBase::Casm:     chkStmnt_Casm(); break;
      case IR::CodeBase::Cnat:     chkStmnt_Cnat(); break;
      case IR::CodeBase::Copy:     chkStmnt_Copy(); break;
      case IR::CodeBase::Cscr_IA:  chkStmnt_Cscr_IA(); break;
      case IR::CodeBase::Cscr_IS:  chkStmnt_Cscr_IS(); break;
      case IR::CodeBase::Cscr_SA:  chkStmnt_Cscr_SA(); break;
      case IR::CodeBase::Cscr_SS:  chkStmnt_Cscr_SS(); break;
      case IR::CodeBase::Cspe:     chkStmnt_Cspe(); break;
      case IR::CodeBase::Jcnd_Nil: chkStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: chkStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: chkStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jfar_Pro: chkStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: chkStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: chkStmnt_Jfar_Sta(); break;
      case IR::CodeBase::Jump:     chkStmnt_Jump(); break;
      case IR::CodeBase::LAnd:     chkStmnt_LAnd(); break;
      case IR::CodeBase::LNot:     chkStmnt_LNot(); break;
      case IR::CodeBase::LOrI:     chkStmnt_LOrI(); break;
      case IR::CodeBase::Rjnk:     chkStmnt_Rjnk(); break;
      case IR::CodeBase::SubX:     chkStmnt_SubX(); break;
      case IR::CodeBase::Swap:     chkStmnt_Swap(); break;

      default:
         break;
      }
   }

   //
   // Info::chkStmntArg
   //
   void Info::chkStmntArg(IR::Arg const &arg)
   {
      switch(arg.a)
      {
      case IR::ArgBase::Aut:
         chkStmntArg(*arg.aAut.idx);
         break;

      case IR::ArgBase::GblArr:
         chkStmntArgA(arg.aGblArr, IR::ArgBase::Lit);
         chkStmntArg(*arg.aGblArr.idx);
         break;

      case IR::ArgBase::GblReg:
         chkStmntArgI(arg.aGblReg, IR::ArgBase::Lit);
         break;

      case IR::ArgBase::HubArr:
         chkStmntArgA(arg.aHubArr, IR::ArgBase::Lit);
         chkStmntArg(*arg.aHubArr.idx);
         break;

      case IR::ArgBase::HubReg:
         chkStmntArgI(arg.aHubReg, IR::ArgBase::Lit);
         break;

      case IR::ArgBase::Lit: break;
      case IR::ArgBase::Nul: break;
      case IR::ArgBase::Stk: break;

      case IR::ArgBase::LocArr:
         chkStmntArgA(arg.aLocArr, IR::ArgBase::Lit);
         chkStmntArg(*arg.aLocArr.idx);
         break;

      case IR::ArgBase::LocReg:
         chkStmntArgI(arg.aLocReg, IR::ArgBase::Lit);
         break;

      case IR::ArgBase::ModArr:
         chkStmntArgA(arg.aModArr, IR::ArgBase::Lit);
         chkStmntArg(*arg.aModArr.idx);
         break;

      case IR::ArgBase::ModReg:
         chkStmntArgI(arg.aModReg, IR::ArgBase::Lit);
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

