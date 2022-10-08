//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2022 David Hill
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
            if(ScriptFlags.find(st) || ScriptTypes.find(st))
               break;

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
      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop: break;

      case IR::CodeBase::Add:      trStmnt_Add(); break;
      case IR::CodeBase::AddX:     trStmnt_AddX(); break;
      case IR::CodeBase::BAnd:     trStmnt_BAnd(); break;
      case IR::CodeBase::BNot:     trStmnt_BNot(); break;
      case IR::CodeBase::BOrI:     trStmnt_BOrI(); break;
      case IR::CodeBase::BOrX:     trStmnt_BOrX(); break;
      case IR::CodeBase::Bclo:     trStmnt_Bclz(); break;
      case IR::CodeBase::Bclz:     trStmnt_Bclz(); break;
      case IR::CodeBase::Bges:     trStmnt_Bges(); break;
      case IR::CodeBase::Bget:     trStmnt_Bget(); break;
      case IR::CodeBase::Bset:     trStmnt_Bset(); break;
      case IR::CodeBase::Call:     trStmnt_Call(); break;
      case IR::CodeBase::Casm:     trStmnt_Casm(); break;
      case IR::CodeBase::CmpEQ:    trStmnt_CmpEQ(); break;
      case IR::CodeBase::CmpGE:    trStmnt_CmpGE(); break;
      case IR::CodeBase::CmpGT:    trStmnt_CmpGT(); break;
      case IR::CodeBase::CmpLE:    trStmnt_CmpLE(); break;
      case IR::CodeBase::CmpLT:    trStmnt_CmpLT(); break;
      case IR::CodeBase::CmpNE:    trStmnt_CmpNE(); break;
      case IR::CodeBase::Cnat:     trStmnt_Cnat(); break;
      case IR::CodeBase::Copy:     trStmnt_Copy(); break;
      case IR::CodeBase::Cscr_IA:  trStmnt_Cscr_IA(); break;
      case IR::CodeBase::Cscr_IS:  trStmnt_Cscr_IS(); break;
      case IR::CodeBase::Cscr_SA:  trStmnt_Cscr_SA(); break;
      case IR::CodeBase::Cscr_SS:  trStmnt_Cscr_SS(); break;
      case IR::CodeBase::Cspe:     trStmnt_Cspe(); break;
      case IR::CodeBase::Div:      trStmnt_Div(); break;
      case IR::CodeBase::DivX:     trStmnt_DivX(); break;
      case IR::CodeBase::Jcnd_Nil: trStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: trStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: trStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jdyn:     trStmnt_Jdyn(); break;
      case IR::CodeBase::Jfar_Pro: trStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: trStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: trStmnt_Jfar_Sta(); break;
      case IR::CodeBase::Jump:     trStmnt_Jump(); break;
      case IR::CodeBase::LAnd:     trStmnt_LAnd(); break;
      case IR::CodeBase::LNot:     trStmnt_LNot(); break;
      case IR::CodeBase::LOrI:     trStmnt_LOrI(); break;
      case IR::CodeBase::Mod:      trStmnt_Mod(); break;
      case IR::CodeBase::Move:     trStmnt_Move(); break;
      case IR::CodeBase::Mul:      trStmnt_Mul(); break;
      case IR::CodeBase::MulX:     trStmnt_MulX(); break;
      case IR::CodeBase::Neg:      trStmnt_Neg(); break;
      case IR::CodeBase::Pltn:     trStmnt_Pltn(); break;
      case IR::CodeBase::Retn:     trStmnt_Retn(); break;
      case IR::CodeBase::Rjnk:     trStmnt_Rjnk(); break;
      case IR::CodeBase::ShL:      trStmnt_ShL(); break;
      case IR::CodeBase::ShR:      trStmnt_ShR(); break;
      case IR::CodeBase::Sub:      trStmnt_Sub(); break;
      case IR::CodeBase::SubX:     trStmnt_SubX(); break;
      case IR::CodeBase::Swap:     trStmnt_Swap(); break;
      case IR::CodeBase::Tr:       trStmnt_Tr(); break;
      case IR::CodeBase::Xcod_SID: trStmnt_Xcod_SID(); break;

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

