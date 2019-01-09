//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code translation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Exception.hpp"
#include "IR/Function.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::trFunc
   //
   void Info::trFunc()
   {
      trBlock(func->block);

      switch(func->ctype)
      {
      case IR::CallType::SScriptS:
      case IR::CallType::ScriptS:
         if(!func->valueStr)
            func->valueStr = func->glyph;

      case IR::CallType::SScriptI:
      case IR::CallType::ScriptI:
         // Check script types.
         for(auto const &st : func->stype) switch(st)
         {
         // Flags.
         case Core::STR_clientside:
         case Core::STR_net:

         // Types.
         case Core::STR_bluereturn:
         case Core::STR_death:
         case Core::STR_disconnect:
         case Core::STR_enter:
         case Core::STR_event:
         case Core::STR_kill:
         case Core::STR_lightning:
         case Core::STR_open:
         case Core::STR_pickup:
         case Core::STR_redreturn:
         case Core::STR_reopen:
         case Core::STR_respawn:
         case Core::STR_return:
         case Core::STR_unloading:
         case Core::STR_whitereturn:
            break;

         default:
            Core::ErrorExpect(func->getOrigin(), "ZDACS script type", st);
         }

         break;

      default: break;
      }
   }

   //
   // Info::trStmnt
   //
   void Info::trStmnt()
   {
      switch(stmnt->code)
      {
      case IR::Code::Nop: break;

      case IR::Code::AdXU: trStmnt_AdXU(); break;

      case IR::Code::AddF:
      case IR::Code::MulF:
      case IR::Code::MulI:
      case IR::Code::MulK:
      case IR::Code::MulU:
      case IR::Code::MulX:
         trStmntStk3(false);
         break;

      case IR::Code::AddI:
      case IR::Code::AddU: trStmnt_AddU(); break;

      case IR::Code::BAnd: trStmntBitwise(); break;
      case IR::Code::BNot: trStmnt_BNot(); break;
      case IR::Code::BOrI: trStmntBitwise(); break;
      case IR::Code::BOrX: trStmntBitwise(); break;

      case IR::Code::Bclo: trStmnt_Bclz(); break;
      case IR::Code::Bclz: trStmnt_Bclz(); break;
      case IR::Code::Bges: trStmnt_Bget(); break;
      case IR::Code::Bget: trStmnt_Bget(); break;
      case IR::Code::Bset: trStmnt_Bset(); break;

      case IR::Code::Call: trStmnt_Call(); break;

      case IR::Code::Casm: trStmnt_Casm(); break;

      case IR::Code::CmpF_EQ:
      case IR::Code::CmpF_NE:
         trStmntStk3(false);
         break;

      case IR::Code::CmpF_GE:
      case IR::Code::CmpF_GT:
      case IR::Code::CmpF_LE:
      case IR::Code::CmpF_LT:
         trStmntStk3(true);
         break;

      case IR::Code::CmpI_EQ:
      case IR::Code::CmpI_NE:
      case IR::Code::CmpU_EQ:
      case IR::Code::CmpU_NE:
         trStmnt_CmpU_EQ();
         break;

      case IR::Code::CmpI_GE:
      case IR::Code::CmpI_GT:
      case IR::Code::CmpI_LE:
      case IR::Code::CmpI_LT:
      case IR::Code::CmpU_GE:
      case IR::Code::CmpU_GT:
      case IR::Code::CmpU_LE:
      case IR::Code::CmpU_LT:
         trStmntStk3(true);
         break;

      case IR::Code::Cnat: trStmnt_Cnat(); break;
      case IR::Code::Copy: trStmnt_Copy(); break;

      case IR::Code::Cscr_IA: trStmnt_Cscr_IA(); break;
      case IR::Code::Cscr_IS: trStmnt_Cscr_IS(); break;
      case IR::Code::Cscr_SA: trStmnt_Cscr_SA(); break;
      case IR::Code::Cscr_SS: trStmnt_Cscr_SS(); break;

      case IR::Code::Cspe: trStmnt_Cspe(); break;

      case IR::Code::DiXI: trStmnt_DiXI(); break;
      case IR::Code::DiXU: trStmnt_DiXU(); break;

      case IR::Code::DivF:
      case IR::Code::DivI:
      case IR::Code::DivK:
      case IR::Code::DivU:
      case IR::Code::DivX:
      case IR::Code::SubF:
         trStmntStk3(true);
         break;

      case IR::Code::Jcnd_Nil: trStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: trStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: trStmnt_Jcnd_Tru(); break;

      case IR::Code::Jdyn:
         moveArgStk_src(stmnt->args[0]);
         break;

      case IR::Code::Jfar: trStmnt_Jfar(); break;
      case IR::Code::Jset: trStmnt_Jset(); break;
      case IR::Code::Jump: trStmnt_Jump(); break;
      case IR::Code::LAnd: trStmnt_LAnd(); break;
      case IR::Code::LNot: trStmnt_LNot(); break;
      case IR::Code::LOrI: trStmnt_LOrI(); break;
      case IR::Code::ModI: trStmnt_ModI(); break;
      case IR::Code::ModU: trStmnt_ModU(); break;
      case IR::Code::Move: trStmnt_Move(); break;

      case IR::Code::MuXU:
         trStmntStk3(false);
         break;

      case IR::Code::NegF: trStmntStk2(); break;
      case IR::Code::NegI: trStmnt_NegI(); break;

      case IR::Code::Pltn: trStmntStk2(); break;

      case IR::Code::Retn: trStmnt_Retn(); break;

      case IR::Code::ShLF: trStmnt_ShLF(); break;
      case IR::Code::ShLU: trStmnt_ShLU(); break;
      case IR::Code::ShRF: trStmnt_ShLF(); break;
      case IR::Code::ShRI: trStmnt_ShRI(); break;
      case IR::Code::ShRU: trStmnt_ShRU(); break;

      case IR::Code::SuXU: trStmnt_SuXU(); break;

      case IR::Code::SubI:
      case IR::Code::SubU: trStmnt_SubU(); break;

      case IR::Code::Swap: trStmnt_Swap(); break;

      case IR::Code::Xcod_SID: break;

      default:
         IR::ErrorCode(stmnt, "unsupported tr");
      }
   }

   //
   // Info::trStmntTmp
   //
   void Info::trStmntTmp(Core::FastU n)
   {
      func->setLocalTmp(n);
   }
}

// EOF

