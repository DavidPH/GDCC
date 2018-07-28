//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement glyph generation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "Core/Exception.hpp"

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
         auto val = IR::ExpCreate_Value(
            IR::Value_Fixed(CodeBase() + numChunkCODE, TypeWord), stmnt->pos);

         for(auto const &lab : stmnt->labs)
         {
            auto &data = prog->getGlyphData(lab);

            data.type  = TypeWord;
            data.value = val;
         }
      }

      switch(stmnt->code)
      {
      case IR::Code::Nop: numChunkCODE += 4; break;

      case IR::Code::AdXU: genStmnt_AdXU(); break;

      case IR::Code::AddF:
      case IR::Code::DivF:
      case IR::Code::DivK:
      case IR::Code::MulF:
      case IR::Code::MulK:
      case IR::Code::SubF:
         genStmntCall(getStmntSize());
         break;

      case IR::Code::AddI:
      case IR::Code::AddU:
      case IR::Code::SubI:
      case IR::Code::SubU:
         genStmnt_AddU();
         break;

      case IR::Code::BAnd: genStmntBitwise(); break;
      case IR::Code::BNot: genStmnt_BNot(); break;
      case IR::Code::BOrI: genStmntBitwise(); break;
      case IR::Code::BOrX: genStmntBitwise(); break;

      case IR::Code::Bclo: genStmnt_Bclz(); break;
      case IR::Code::Bclz: genStmnt_Bclz(); break;
      case IR::Code::Bges: genStmnt_Bges(); break;
      case IR::Code::Bget: genStmnt_Bget(); break;
      case IR::Code::Bset: genStmnt_Bset(); break;

      case IR::Code::Call: genStmnt_Call(); break;
      case IR::Code::Casm: genStmnt_Casm(); break;

      case IR::Code::CmpF_EQ:
      case IR::Code::CmpF_GE:
      case IR::Code::CmpF_GT:
      case IR::Code::CmpF_LE:
      case IR::Code::CmpF_LT:
      case IR::Code::CmpF_NE:
      case IR::Code::CmpU_GE:
      case IR::Code::CmpU_GT:
      case IR::Code::CmpU_LE:
      case IR::Code::CmpU_LT:
         genStmntCall(1);
         break;

      case IR::Code::CmpI_EQ:
      case IR::Code::CmpI_NE:
      case IR::Code::CmpU_EQ:
      case IR::Code::CmpU_NE:
         genStmnt_CmpU_EQ();
         break;

      case IR::Code::CmpI_GE:
      case IR::Code::CmpI_GT:
      case IR::Code::CmpI_LE:
      case IR::Code::CmpI_LT:
         genStmnt_CmpI();
         break;

      case IR::Code::Copy: numChunkCODE += 4; break;

      case IR::Code::Cnat: numChunkCODE += 12; break;

      case IR::Code::Cscr_IA: genStmnt_Cscr_IA(); break;
      case IR::Code::Cscr_IS: genStmnt_Cscr_IS(); break;
      case IR::Code::Cscr_SA: genStmnt_Cscr_SA(); break;
      case IR::Code::Cscr_SS: genStmnt_Cscr_SS(); break;

      case IR::Code::Cspe: genStmnt_Cspe(); break;

      case IR::Code::DiXI: genStmnt_DiXI(); break;
      case IR::Code::DiXU: genStmntCall(getStmntSize() * 2); break;

      case IR::Code::DivI: genStmnt_DivI(); break;
      case IR::Code::DivU: genStmnt_DivU(); break;
      case IR::Code::DivX: genStmnt_DivX(); break;

      case IR::Code::Jcnd_Nil: genStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: genStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: genStmnt_Jcnd_Tru(); break;

      case IR::Code::Jdyn: numChunkCODE += 4; break;

      case IR::Code::Jfar: genStmnt_Jfar(); break;

      case IR::Code::Jset: genStmnt_Jset(); break;

      case IR::Code::Jump: genStmnt_Jump(); break;

      case IR::Code::LAnd:
      case IR::Code::LOrI:
         if(auto n = getStmntSize(); n == 1)
            numChunkCODE += 4;
         else
            genStmntCall(getStmntSize());
         break;

      case IR::Code::LNot: numChunkCODE += getStmntSize() * 4; break;

      case IR::Code::ModI: genStmnt_ModI(); break;
      case IR::Code::ModU: genStmnt_ModU(); break;

      case IR::Code::Move: genStmnt_Move(); break;

      case IR::Code::MuXU: genStmntCall(getStmntSize() * 2); break;

      case IR::Code::MulI:
      case IR::Code::MulU:
      case IR::Code::MulX:
         if(auto n = getStmntSize(); n == 1)
            numChunkCODE += 4;
         else
            genStmntCall(n);
         break;

      case IR::Code::NegF: genStmnt_NegF(); break;
      case IR::Code::NegI: genStmnt_NegI(); break;

      case IR::Code::Pltn: numChunkCODE += 12; break;

      case IR::Code::Retn: genStmnt_Retn(); break;

      case IR::Code::ShLF: genStmnt_ShLF(); break;
      case IR::Code::ShLU: genStmnt_ShLU(); break;
      case IR::Code::ShRF: genStmnt_ShLF(); break;
      case IR::Code::ShRI: genStmnt_ShRI(); break;
      case IR::Code::ShRU: genStmnt_ShRU(); break;

      case IR::Code::SuXU: genStmnt_SuXU(); break;

      case IR::Code::Swap: genStmnt_Swap(); break;

      case IR::Code::Xcod_SID: numChunkCODE += 32; break;

      default:
         Core::Error(stmnt->pos, "ZDACS cannot gen Code: ", stmnt->code);
      }
   }

   //
   // Info::genStmntCall
   //
   void Info::genStmntCall(Core::FastU ret)
   {
      numChunkCODE += (ret ? (ret - 1) * 16 : 0) + 8;
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
}

// EOF

