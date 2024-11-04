//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement glyph generation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Exception.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::genStmnt
   //
   void Info::genStmnt()
   {
      // Generate label glyphs.
      if(!stmnt->labs.empty())
      {
         auto ip = Core::NumberCast<Core::Integ>(CodeBase() + numChunkCODE);
         auto val = IR::ExpCreate_Value(
            IR::Value_Fixed(std::move(ip), TypeWord), stmnt->pos);

         for(auto const &lab : stmnt->labs)
         {
            auto &data = prog->getGlyphData(lab);

            data.type  = TypeWord;
            data.value = val;
         }
      }

      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop: numChunkCODE += 4; break;

      case IR::CodeBase::Add:      genStmnt_Add(); break;
      case IR::CodeBase::AddX:     genStmnt_AddX(); break;
      case IR::CodeBase::BAnd:     genStmnt_BAnd(); break;
      case IR::CodeBase::BNot:     genStmnt_BNot(); break;
      case IR::CodeBase::BOrI:     genStmnt_BOrI(); break;
      case IR::CodeBase::BOrX:     genStmnt_BOrX(); break;
      case IR::CodeBase::Bclo:     genStmnt_Bclo(); break;
      case IR::CodeBase::Bclz:     genStmnt_Bclz(); break;
      case IR::CodeBase::Bges:     genStmnt_Bges(); break;
      case IR::CodeBase::Bget:     genStmnt_Bget(); break;
      case IR::CodeBase::Bset:     genStmnt_Bset(); break;
      case IR::CodeBase::Call:     genStmnt_Call(); break;
      case IR::CodeBase::Casm:     genStmnt_Casm(); break;
      case IR::CodeBase::CmpEQ:    genStmnt_CmpEQ(); break;
      case IR::CodeBase::CmpGE:    genStmnt_CmpGE(); break;
      case IR::CodeBase::CmpGT:    genStmnt_CmpLT(); break;
      case IR::CodeBase::CmpLE:    genStmnt_CmpLE(); break;
      case IR::CodeBase::CmpLT:    genStmnt_CmpLT(); break;
      case IR::CodeBase::CmpNE:    genStmnt_CmpNE(); break;
      case IR::CodeBase::Copy:     genStmnt_Copy(); break;
      case IR::CodeBase::Cnat:     genStmnt_Cnat(); break;
      case IR::CodeBase::Cscr_IA:  genStmnt_Cscr_IA(); break;
      case IR::CodeBase::Cscr_IS:  genStmnt_Cscr_IS(); break;
      case IR::CodeBase::Cscr_SA:  genStmnt_Cscr_SA(); break;
      case IR::CodeBase::Cscr_SS:  genStmnt_Cscr_SS(); break;
      case IR::CodeBase::Cspe:     genStmnt_Cspe(); break;
      case IR::CodeBase::Div:      genStmnt_Div(); break;
      case IR::CodeBase::DivX:     genStmnt_DivX(); break;
      case IR::CodeBase::Jcnd_Nil: genStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: genStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: genStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jdyn:     genStmnt_Jdyn(); break;
      case IR::CodeBase::Jfar_Pro: genStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: genStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: genStmnt_Jfar_Sta(); break;
      case IR::CodeBase::Jump:     genStmnt_Jump(); break;
      case IR::CodeBase::LAnd:     genStmnt_LAnd(); break;
      case IR::CodeBase::LNot:     genStmnt_LNot(); break;
      case IR::CodeBase::LOrI:     genStmnt_LOrI(); break;
      case IR::CodeBase::Mod:      genStmnt_Mod(); break;
      case IR::CodeBase::Move:     genStmnt_Move(); break;
      case IR::CodeBase::Mul:      genStmnt_Mul(); break;
      case IR::CodeBase::MulX:     genStmnt_MulX(); break;
      case IR::CodeBase::Neg:      genStmnt_Neg(); break;
      case IR::CodeBase::Pltn:     genStmnt_Pltn(); break;
      case IR::CodeBase::Retn:     genStmnt_Retn(); break;
      case IR::CodeBase::Rjnk:     genStmnt_Rjnk(); break;
      case IR::CodeBase::ShL:      genStmnt_ShL(); break;
      case IR::CodeBase::ShR:      genStmnt_ShR(); break;
      case IR::CodeBase::Sub:      genStmnt_Sub(); break;
      case IR::CodeBase::SubX:     genStmnt_SubX(); break;
      case IR::CodeBase::Swap:     genStmnt_Swap(); break;
      case IR::CodeBase::Tr:       genStmnt_Tr(); break;
      case IR::CodeBase::Xcod_SID: genStmnt_Xcod_SID(); break;

      default:
         IR::ErrorCode(stmnt, "unsupported gen");
      }
   }

   //
   // Info::genStmnt_Pltn
   //
   void Info::genStmnt_Pltn()
   {
      numChunkCODE += 12;
   }

   //
   // Info::genStmnt_Xcod_SID
   //
   void Info::genStmnt_Xcod_SID()
   {
      if(!isInitScriptEvent())
         numChunkCODE += 32;
   }

   //
   // Info::genStmntCall
   //
   void Info::genStmntCall(Core::FastU retn)
   {
      numChunkCODE += 8;
      genStmntPushRetn(retn, GetRetnMax(IR::CallType::StkCall));
   }

   //
   // Info::genStmntDropArg
   //
   void Info::genStmntDropArg(IR::Arg const &arg, Core::FastU w)
   {
      numChunkCODE += lenDropArg(arg, w);
   }

   //
   // Info::genStmntDropArg
   //
   void Info::genStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      numChunkCODE += lenDropArg(arg, lo, hi);
   }

   //
   // Info::genStmntDropRetn
   //
   void Info::genStmntDropRetn(Core::FastU retn, Core::FastU retnMax)
   {
      if(retn > retnMax)
         numChunkCODE += (retn - retnMax) * 20;
   }

   //
   // Info::genStmntPushArg
   //
   void Info::genStmntPushArg(IR::Arg const &arg, Core::FastU w)
   {
      numChunkCODE += lenPushArg(arg, w);
   }

   //
   // Info::genStmntPushArg
   //
   void Info::genStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      numChunkCODE += lenPushArg(arg, lo, hi);
   }

   //
   // Info::genStmntDropParam
   //
   void Info::genStmntDropParam(Core::FastU param, Core::FastU paramMax)
   {
      genStmntDropRetn(param, paramMax);
   }

   //
   // Info::genStmntPushRetn
   //
   void Info::genStmntPushRetn(Core::FastU retn, Core::FastU retnMax)
   {
      if(retn > retnMax)
         numChunkCODE += (retn - retnMax) * 16;
   }

   //
   // Info::genStmntStkBin
   //
   void Info::genStmntStkBin(Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1 && code != Code::Nop)
         return (void)(numChunkCODE += 4);

      genStmntCall(stmnt->args[0].getSize());
   }

   //
   // Info::genStmntStkCmp
   //
   void Info::genStmntStkCmp(Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return (void)(numChunkCODE += 8);

      if(n == 1 && code != Code::Nop)
         return (void)(numChunkCODE += 4);

      genStmntCall(stmnt->args[0].getSize());
   }
}

// EOF

