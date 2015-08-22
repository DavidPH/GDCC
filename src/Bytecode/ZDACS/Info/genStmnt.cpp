//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement glyph generation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
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

            switch(stmnt->op.code)
            {
            case IR::Code::Nop: numChunkCODE += 4; break;

            case IR::Code::AdXU_W: genStmnt_AdXU_W(); break;

            case IR::Code::AddF_W:
            case IR::Code::DivF_W:
            case IR::Code::DivK_W:
            case IR::Code::DivU_W:
            case IR::Code::ModU_W:
            case IR::Code::MulF_W:
            case IR::Code::MulK_W:
            case IR::Code::ShLF_W:
            case IR::Code::ShRF_W:
            case IR::Code::SubF_W:
               genStmntCall(stmnt->op.size);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:
            case IR::Code::SubI_W:
            case IR::Code::SubU_W:
               genStmnt_AddU_W();
               break;

            case IR::Code::Bclo_W: genStmnt_Bclz_W(); break;
            case IR::Code::Bclz_W: genStmnt_Bclz_W(); break;

            case IR::Code::DivI_W:
            case IR::Code::DivX_W:
            case IR::Code::ModI_W:
               genStmnt_DivI_W();
               break;

            case IR::Code::AndU_W: genStmntBitwise(); break;

            case IR::Code::Call: genStmnt_Call(); break;

            case IR::Code::Casm: numChunkCODE += stmnt->args.size() * 4; break;

            case IR::Code::CmpF_EQ_W:
            case IR::Code::CmpF_GE_W:
            case IR::Code::CmpF_GT_W:
            case IR::Code::CmpF_LE_W:
            case IR::Code::CmpF_LT_W:
            case IR::Code::CmpF_NE_W:
            case IR::Code::CmpU_GE_W:
            case IR::Code::CmpU_GT_W:
            case IR::Code::CmpU_LE_W:
            case IR::Code::CmpU_LT_W:
               genStmntCall(1);
               break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_NE_W:
               genStmnt_CmpU_EQ_W();
               break;

            case IR::Code::CmpI_GE_W:
            case IR::Code::CmpI_GT_W:
            case IR::Code::CmpI_LE_W:
            case IR::Code::CmpI_LT_W:
               genStmnt_CmpI_W();
               break;

            case IR::Code::Copy_W: numChunkCODE += 4; break;

            case IR::Code::Cnat: numChunkCODE += 12; break;

            case IR::Code::Cscr_IA: genStmnt_Cscr_IA(); break;
            case IR::Code::Cscr_IS: genStmnt_Cscr_IS(); break;
            case IR::Code::Cscr_SA: genStmnt_Cscr_SA(); break;
            case IR::Code::Cscr_SS: genStmnt_Cscr_SS(); break;

            case IR::Code::Cspe: genStmnt_Cspe(); break;

            case IR::Code::DiXI_W: genStmnt_DiXI_W(); break;
            case IR::Code::DiXU_W: genStmntCall(stmnt->op.size * 2); break;

            case IR::Code::InvU_W: genStmnt_InvU_W(); break;

            case IR::Code::Jcnd_Nil: genStmnt_Jcnd_Nil(); break;
            case IR::Code::Jcnd_Tru: genStmnt_Jcnd_Tru(); break;

            case IR::Code::Jdyn: numChunkCODE += 4; break;

            case IR::Code::Jump: genStmnt_Jump(); break;

            case IR::Code::LAnd:
            case IR::Code::LOrI:
               if(stmnt->op.size == 1)
                  numChunkCODE += 4;
               else
                  genStmntCall(stmnt->op.size);
               break;

            case IR::Code::Move_W: genStmnt_Move_W(); break;

            case IR::Code::MuXU_W: genStmntCall(stmnt->op.size * 2); break;

            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
            case IR::Code::MulX_W:
               if(stmnt->op.size == 1)
                  numChunkCODE += 4;
               else
                  genStmntCall(stmnt->op.size);
               break;

            case IR::Code::NegF_W: genStmnt_NegF_W(); break;
            case IR::Code::NegI_W: genStmnt_NegI_W(); break;

            case IR::Code::NotU_W: numChunkCODE += stmnt->op.size * 4; break;

            case IR::Code::OrIU_W: genStmntBitwise(); break;
            case IR::Code::OrXU_W: genStmntBitwise(); break;

            case IR::Code::Plsa: numChunkCODE += 16; break;
            case IR::Code::Plsf: numChunkCODE += 16; break;
            case IR::Code::Pltn: numChunkCODE += 12; break;

            case IR::Code::Retn: genStmnt_Retn(); break;

            case IR::Code::ShLU_W: genStmnt_ShLU_W(); break;
            case IR::Code::ShRI_W: genStmnt_ShRI_W(); break;
            case IR::Code::ShRU_W: genStmnt_ShRU_W(); break;

            case IR::Code::SuXU_W: genStmnt_SuXU_W(); break;

            case IR::Code::Swap_W: genStmnt_Swap_W(); break;

            case IR::Code::Xcod_SID: numChunkCODE += 32; break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot gen Code for ZDACS: " << stmnt->op << '\n';
               throw EXIT_FAILURE;
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
   }
}

// EOF

