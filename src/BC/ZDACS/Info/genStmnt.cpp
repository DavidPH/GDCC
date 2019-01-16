//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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

      switch(stmnt->code)
      {
      case IR::Code::Nop: numChunkCODE += 4; break;

      case IR::Code::AdXU: genStmnt_AdXU(); break;
      case IR::Code::AddF: genStmnt_AddF(); break;
      case IR::Code::AddI: genStmnt_AddI(); break;
      case IR::Code::AddU: genStmnt_AddU(); break;
      case IR::Code::BAnd: genStmnt_BAnd(); break;
      case IR::Code::BNot: genStmnt_BNot(); break;
      case IR::Code::BOrI: genStmnt_BOrI(); break;
      case IR::Code::BOrX: genStmnt_BOrX(); break;
      case IR::Code::Bclo: genStmnt_Bclo(); break;
      case IR::Code::Bclz: genStmnt_Bclz(); break;
      case IR::Code::Bges: genStmnt_Bges(); break;
      case IR::Code::Bget: genStmnt_Bget(); break;
      case IR::Code::Bset: genStmnt_Bset(); break;
      case IR::Code::Call: genStmnt_Call(); break;
      case IR::Code::Casm: genStmnt_Casm(); break;

      case IR::Code::CmpF_EQ: genStmnt_CmpF_EQ(); break;
      case IR::Code::CmpF_GE: genStmnt_CmpF_GE(); break;
      case IR::Code::CmpF_GT: genStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_LE: genStmnt_CmpF_LE(); break;
      case IR::Code::CmpF_LT: genStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_NE: genStmnt_CmpF_NE(); break;
      case IR::Code::CmpI_EQ: genStmnt_CmpI_EQ(); break;
      case IR::Code::CmpI_GE: genStmnt_CmpI_GE(); break;
      case IR::Code::CmpI_GT: genStmnt_CmpI_GT(); break;
      case IR::Code::CmpI_LE: genStmnt_CmpI_LE(); break;
      case IR::Code::CmpI_LT: genStmnt_CmpI_LT(); break;
      case IR::Code::CmpI_NE: genStmnt_CmpI_NE(); break;
      case IR::Code::CmpU_EQ: genStmnt_CmpU_EQ(); break;
      case IR::Code::CmpU_GE: genStmnt_CmpU_GE(); break;
      case IR::Code::CmpU_GT: genStmnt_CmpU_GT(); break;
      case IR::Code::CmpU_LE: genStmnt_CmpU_LE(); break;
      case IR::Code::CmpU_LT: genStmnt_CmpU_LT(); break;
      case IR::Code::CmpU_NE: genStmnt_CmpU_NE(); break;

      case IR::Code::Copy: genStmnt_Copy(); break;
      case IR::Code::Cnat: genStmnt_Cnat(); break;

      case IR::Code::Cscr_IA: genStmnt_Cscr_IA(); break;
      case IR::Code::Cscr_IS: genStmnt_Cscr_IS(); break;
      case IR::Code::Cscr_SA: genStmnt_Cscr_SA(); break;
      case IR::Code::Cscr_SS: genStmnt_Cscr_SS(); break;

      case IR::Code::Cspe: genStmnt_Cspe(); break;
      case IR::Code::DiXI: genStmnt_DiXI(); break;
      case IR::Code::DiXU: genStmnt_DiXU(); break;
      case IR::Code::DivF: genStmnt_DivF(); break;
      case IR::Code::DivK: genStmnt_DivK(); break;
      case IR::Code::DivI: genStmnt_DivI(); break;
      case IR::Code::DivU: genStmnt_DivU(); break;
      case IR::Code::DivX: genStmnt_DivX(); break;

      case IR::Code::Jcnd_Nil: genStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: genStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: genStmnt_Jcnd_Tru(); break;

      case IR::Code::Jdyn: genStmnt_Jdyn(); break;

      case IR::Code::Jfar_Pro: genStmnt_Jfar_Pro(); break;
      case IR::Code::Jfar_Set: genStmnt_Jfar_Set(); break;
      case IR::Code::Jfar_Sta: genStmnt_Jfar_Sta(); break;

      case IR::Code::Jump: genStmnt_Jump(); break;
      case IR::Code::LAnd: genStmnt_LAnd(); break;
      case IR::Code::LNot: genStmnt_LNot(); break;
      case IR::Code::LOrI: genStmnt_LOrI(); break;
      case IR::Code::ModI: genStmnt_ModI(); break;
      case IR::Code::ModU: genStmnt_ModU(); break;
      case IR::Code::Move: genStmnt_Move(); break;
      case IR::Code::MuXU: genStmnt_MuXU(); break;
      case IR::Code::MulF: genStmnt_MulF(); break;
      case IR::Code::MulI: genStmnt_MulI(); break;
      case IR::Code::MulK: genStmnt_MulK(); break;
      case IR::Code::MulU: genStmnt_MulU(); break;
      case IR::Code::MulX: genStmnt_MulX(); break;
      case IR::Code::NegF: genStmnt_NegF(); break;
      case IR::Code::NegI: genStmnt_NegI(); break;
      case IR::Code::Pltn: genStmnt_Pltn(); break;
      case IR::Code::Retn: genStmnt_Retn(); break;
      case IR::Code::ShLF: genStmnt_ShLF(); break;
      case IR::Code::ShLU: genStmnt_ShLU(); break;
      case IR::Code::ShRF: genStmnt_ShLF(); break;
      case IR::Code::ShRI: genStmnt_ShRI(); break;
      case IR::Code::ShRU: genStmnt_ShRU(); break;
      case IR::Code::SuXU: genStmnt_SuXU(); break;
      case IR::Code::SubF: genStmnt_SubF(); break;
      case IR::Code::SubI: genStmnt_SubI(); break;
      case IR::Code::SubU: genStmnt_SubU(); break;
      case IR::Code::Swap: genStmnt_Swap(); break;

      case IR::Code::Xcod_SID: genStmnt_Xcod_SID(); break;

      default:
         Core::Error(stmnt->pos, "ZDACS cannot gen Code: ", stmnt->code);
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
     numChunkCODE += 32;
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

