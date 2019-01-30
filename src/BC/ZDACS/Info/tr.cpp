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
      case IR::Code::AddF: trStmnt_AddF(); break;
      case IR::Code::AddI: trStmnt_AddI(); break;
      case IR::Code::AddU: trStmnt_AddU(); break;
      case IR::Code::BAnd: trStmnt_BAnd(); break;
      case IR::Code::BNot: trStmnt_BNot(); break;
      case IR::Code::BOrI: trStmnt_BOrI(); break;
      case IR::Code::BOrX: trStmnt_BOrX(); break;
      case IR::Code::Bclo: trStmnt_Bclz(); break;
      case IR::Code::Bclz: trStmnt_Bclz(); break;
      case IR::Code::Bges: trStmnt_Bges(); break;
      case IR::Code::Bget: trStmnt_Bget(); break;
      case IR::Code::Bset: trStmnt_Bset(); break;
      case IR::Code::Call: trStmnt_Call(); break;
      case IR::Code::Casm: trStmnt_Casm(); break;

      case IR::Code::CmpF_EQ: trStmnt_CmpF_EQ(); break;
      case IR::Code::CmpF_GE: trStmnt_CmpF_GE(); break;
      case IR::Code::CmpF_GT: trStmnt_CmpF_GT(); break;
      case IR::Code::CmpF_LE: trStmnt_CmpF_LE(); break;
      case IR::Code::CmpF_LT: trStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_NE: trStmnt_CmpF_NE(); break;
      case IR::Code::CmpI_EQ: trStmnt_CmpI_EQ(); break;
      case IR::Code::CmpI_GE: trStmnt_CmpI_GE(); break;
      case IR::Code::CmpI_GT: trStmnt_CmpI_GT(); break;
      case IR::Code::CmpI_LE: trStmnt_CmpI_LE(); break;
      case IR::Code::CmpI_LT: trStmnt_CmpI_LT(); break;
      case IR::Code::CmpI_NE: trStmnt_CmpI_NE(); break;
      case IR::Code::CmpU_EQ: trStmnt_CmpU_EQ(); break;
      case IR::Code::CmpU_GE: trStmnt_CmpU_GE(); break;
      case IR::Code::CmpU_GT: trStmnt_CmpU_GT(); break;
      case IR::Code::CmpU_LE: trStmnt_CmpU_LE(); break;
      case IR::Code::CmpU_LT: trStmnt_CmpU_LT(); break;
      case IR::Code::CmpU_NE: trStmnt_CmpU_NE(); break;

      case IR::Code::Cnat: trStmnt_Cnat(); break;
      case IR::Code::Copy: trStmnt_Copy(); break;

      case IR::Code::Cscr_IA: trStmnt_Cscr_IA(); break;
      case IR::Code::Cscr_IS: trStmnt_Cscr_IS(); break;
      case IR::Code::Cscr_SA: trStmnt_Cscr_SA(); break;
      case IR::Code::Cscr_SS: trStmnt_Cscr_SS(); break;

      case IR::Code::Cspe: trStmnt_Cspe(); break;
      case IR::Code::DiXI: trStmnt_DiXI(); break;
      case IR::Code::DiXU: trStmnt_DiXU(); break;
      case IR::Code::DivA: trStmnt_DivA(); break;
      case IR::Code::DivF: trStmnt_DivF(); break;
      case IR::Code::DivI: trStmnt_DivI(); break;
      case IR::Code::DivK: trStmnt_DivK(); break;
      case IR::Code::DivR: trStmnt_DivR(); break;
      case IR::Code::DivU: trStmnt_DivU(); break;
      case IR::Code::DivX: trStmnt_DivX(); break;

      case IR::Code::Jcnd_Nil: trStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: trStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: trStmnt_Jcnd_Tru(); break;

      case IR::Code::Jdyn: trStmnt_Jdyn(); break;

      case IR::Code::Jfar_Pro: trStmnt_Jfar_Pro(); break;
      case IR::Code::Jfar_Set: trStmnt_Jfar_Set(); break;
      case IR::Code::Jfar_Sta: trStmnt_Jfar_Sta(); break;

      case IR::Code::Jump: trStmnt_Jump(); break;
      case IR::Code::LAnd: trStmnt_LAnd(); break;
      case IR::Code::LNot: trStmnt_LNot(); break;
      case IR::Code::LOrI: trStmnt_LOrI(); break;
      case IR::Code::ModI: trStmnt_ModI(); break;
      case IR::Code::ModU: trStmnt_ModU(); break;
      case IR::Code::Move: trStmnt_Move(); break;
      case IR::Code::MuXU: trStmnt_MuXU(); break;
      case IR::Code::MulA: trStmnt_MulA(); break;
      case IR::Code::MulF: trStmnt_MulF(); break;
      case IR::Code::MulI: trStmnt_MulI(); break;
      case IR::Code::MulK: trStmnt_MulK(); break;
      case IR::Code::MulR: trStmnt_MulR(); break;
      case IR::Code::MulU: trStmnt_MulU(); break;
      case IR::Code::MulX: trStmnt_MulX(); break;
      case IR::Code::NegF: trStmnt_NegF(); break;
      case IR::Code::NegI: trStmnt_NegI(); break;
      case IR::Code::Pltn: trStmnt_Pltn(); break;
      case IR::Code::Retn: trStmnt_Retn(); break;
      case IR::Code::Rjnk: trStmnt_Rjnk(); break;
      case IR::Code::ShLF: trStmnt_ShLF(); break;
      case IR::Code::ShLU: trStmnt_ShLU(); break;
      case IR::Code::ShRF: trStmnt_ShRF(); break;
      case IR::Code::ShRI: trStmnt_ShRI(); break;
      case IR::Code::ShRU: trStmnt_ShRU(); break;
      case IR::Code::SuXU: trStmnt_SuXU(); break;
      case IR::Code::SubF: trStmnt_SubF(); break;
      case IR::Code::SubI: trStmnt_SubI(); break;
      case IR::Code::SubU: trStmnt_SubU(); break;
      case IR::Code::Swap: trStmnt_Swap(); break;

      case IR::Code::Xcod_SID: break;

      default:
         IR::ErrorCode(stmnt, "unsupported tr");
      }
   }

   //
   // Info::trStmntStkBin
   //
   void Info::trStmntStkBin(bool ordered)
   {
      trStmntStk3(ordered);
   }

   //
   // Info::trStmntStkCmp
   //
   void Info::trStmntStkCmp(bool ordered)
   {
      trStmntStk3(ordered);
   }

   //
   // Info::trStmntStkUna
   //
   void Info::trStmntStkUna()
   {
      trStmntStk2();
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

