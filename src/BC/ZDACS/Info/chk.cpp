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

      switch(stmnt->code)
      {
      case IR::Code::AdXU: chkStmnt_AdXU(); break;
      case IR::Code::Bges: chkStmnt_Bges(); break;
      case IR::Code::Bget: chkStmnt_Bget(); break;
      case IR::Code::Bset: chkStmnt_Bset(); break;
      case IR::Code::Call: chkStmnt_Call(); break;
      case IR::Code::Casm: chkStmnt_Casm(); break;
      case IR::Code::Cnat: chkStmnt_Cnat(); break;
      case IR::Code::Copy: chkStmnt_Copy(); break;

      case IR::Code::Cscr_IA: chkStmnt_Cscr_IA(); break;
      case IR::Code::Cscr_IS: chkStmnt_Cscr_IS(); break;
      case IR::Code::Cscr_SA: chkStmnt_Cscr_SA(); break;
      case IR::Code::Cscr_SS: chkStmnt_Cscr_SS(); break;

      case IR::Code::Cspe: chkStmnt_Cspe(); break;

      case IR::Code::Jcnd_Nil: chkStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: chkStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: chkStmnt_Jcnd_Tru(); break;
      case IR::Code::Jfar_Pro: chkStmnt_Jfar_Pro(); break;
      case IR::Code::Jfar_Set: chkStmnt_Jfar_Set(); break;
      case IR::Code::Jfar_Sta: chkStmnt_Jfar_Sta(); break;

      case IR::Code::Jump: chkStmnt_Jump(); break;
      case IR::Code::LAnd: chkStmnt_LAnd(); break;
      case IR::Code::LNot: chkStmnt_LNot(); break;
      case IR::Code::LOrI: chkStmnt_LOrI(); break;
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

