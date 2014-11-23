//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "IR/CallType.hpp"
#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
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

            switch(stmnt->code)
            {
            case IR::Code::Nop:
               numChunkCODE += 4;
               break;

            case IR::Code::AddF_W:
            case IR::Code::DivF_W:
            case IR::Code::DivK_W:
            case IR::Code::MulF_W:
            case IR::Code::MulK_W:
            case IR::Code::SubF_W:
               genStmntCall(1);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:
            case IR::Code::SubI_W:
            case IR::Code::SubU_W:
               genStmnt_AddU_W();
               break;

            case IR::Code::AndU_W:
            case IR::Code::DivI_W:
            case IR::Code::DivX_W:
            case IR::Code::ModI_W:
            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
            case IR::Code::MulX_W:
            case IR::Code::OrIU_W:
            case IR::Code::OrXU_W:
            case IR::Code::ShLU_W:
            case IR::Code::ShRI_W:
               numChunkCODE += 4;
               break;

            case IR::Code::AddF_W2:
            case IR::Code::AddI_W2:
            case IR::Code::AddU_W2:
            case IR::Code::DivF_W2:
            case IR::Code::DivI_W2:
            case IR::Code::DivK_W2:
            case IR::Code::DivU_W2:
            case IR::Code::DivX_W2:
            case IR::Code::ModI_W2:
            case IR::Code::ModU_W2:
            case IR::Code::MulF_W2:
            case IR::Code::MulI_W2:
            case IR::Code::MulK_W2:
            case IR::Code::MulU_W2:
            case IR::Code::MulX_W2:
            case IR::Code::SubF_W2:
            case IR::Code::SubI_W2:
            case IR::Code::SubU_W2:
               genStmntCall(2);
               break;

            case IR::Code::AddI_W3:
            case IR::Code::AddU_W3:
            case IR::Code::DivI_W3:
            case IR::Code::DivU_W3:
            case IR::Code::ModI_W3:
            case IR::Code::ModU_W3:
            case IR::Code::MulI_W3:
            case IR::Code::MulU_W3:
            case IR::Code::SubI_W3:
            case IR::Code::SubU_W3:
               genStmntCall(3);
               break;

            case IR::Code::AndU_W2:
            case IR::Code::OrIU_W2:
            case IR::Code::OrXU_W2:
               genStmntBitwise2();
               break;

            case IR::Code::AndU_W3:
            case IR::Code::OrIU_W3:
            case IR::Code::OrXU_W3:
               genStmntBitwise3();
               break;

            case IR::Code::Call:
               genStmnt_Call();
               break;

            case IR::Code::Casm:
               numChunkCODE += stmnt->args.size() * 4;
               break;

            case IR::Code::Cjmp_Nil:
            case IR::Code::Cjmp_Tru:
               numChunkCODE += 8;
               break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpI_GE_W:
            case IR::Code::CmpI_GT_W:
            case IR::Code::CmpI_LE_W:
            case IR::Code::CmpI_LT_W:
            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_NE_W:
               numChunkCODE += 4;
               break;

            case IR::Code::CmpI_EQ_W2:
            case IR::Code::CmpI_NE_W2:
            case IR::Code::CmpU_EQ_W2:
            case IR::Code::CmpU_NE_W2:
               genStmnt_CmpU_EQ_W2();
               break;

            case IR::Code::CmpI_EQ_W3:
            case IR::Code::CmpI_NE_W3:
            case IR::Code::CmpU_EQ_W3:
            case IR::Code::CmpU_NE_W3:
               genStmnt_CmpU_EQ_W3();
               break;

            case IR::Code::CmpF_EQ_W:
            case IR::Code::CmpF_EQ_W2:
            case IR::Code::CmpF_GE_W:
            case IR::Code::CmpF_GE_W2:
            case IR::Code::CmpF_GT_W:
            case IR::Code::CmpF_GT_W2:
            case IR::Code::CmpF_LE_W:
            case IR::Code::CmpF_LE_W2:
            case IR::Code::CmpF_LT_W:
            case IR::Code::CmpF_LT_W2:
            case IR::Code::CmpF_NE_W:
            case IR::Code::CmpF_NE_W2:
            case IR::Code::CmpI_GE_W2:
            case IR::Code::CmpI_GE_W3:
            case IR::Code::CmpI_GT_W2:
            case IR::Code::CmpI_GT_W3:
            case IR::Code::CmpI_LE_W2:
            case IR::Code::CmpI_LE_W3:
            case IR::Code::CmpI_LT_W2:
            case IR::Code::CmpI_LT_W3:
            case IR::Code::CmpU_GE_W:
            case IR::Code::CmpU_GE_W2:
            case IR::Code::CmpU_GE_W3:
            case IR::Code::CmpU_GT_W:
            case IR::Code::CmpU_GT_W2:
            case IR::Code::CmpU_GT_W3:
            case IR::Code::CmpU_LE_W:
            case IR::Code::CmpU_LE_W2:
            case IR::Code::CmpU_LE_W3:
            case IR::Code::CmpU_LT_W:
            case IR::Code::CmpU_LT_W2:
            case IR::Code::CmpU_LT_W3:
               genStmntCall(1);
               break;

            case IR::Code::Copy_W: numChunkCODE += 4; break;

            case IR::Code::Cnat:
               numChunkCODE += 12;
               break;

            case IR::Code::Cspe:
               genStmnt_Cspe();
               break;

            case IR::Code::DiXI_W:  genStmnt_DiXI_W(); break;
            case IR::Code::DiXI_W2: genStmntCall(4); break;
            case IR::Code::DiXI_W3: genStmntCall(6); break;
            case IR::Code::DiXU_W:  genStmntCall(2); break;
            case IR::Code::DiXU_W2: genStmntCall(4); break;
            case IR::Code::DiXU_W3: genStmntCall(6); break;

            case IR::Code::DivU_W:
            case IR::Code::ModU_W:
               numChunkCODE += 8;
               break;

            case IR::Code::InvU_W:  numChunkCODE += 4; break;
            case IR::Code::InvU_W2: genStmnt_InvU_W2(); break;
            case IR::Code::InvU_W3: genStmnt_InvU_W3(); break;

            case IR::Code::Jump:
               numChunkCODE += 8;
               break;

            case IR::Code::Move_W:  genStmnt_Move_W(); break;
            case IR::Code::Move_W2: genStmnt_Move_Wx(2); break;
            case IR::Code::Move_W3: genStmnt_Move_Wx(3); break;
            case IR::Code::Move_W4: genStmnt_Move_Wx(4); break;
            case IR::Code::Move_W5: genStmnt_Move_Wx(5); break;
            case IR::Code::Move_W6: genStmnt_Move_Wx(6); break;

            case IR::Code::MuXU_W:  genStmntCall(2); break;
            case IR::Code::MuXU_W2: genStmntCall(4); break;

            case IR::Code::NegF_W:
            case IR::Code::NegF_W2: genStmnt_NegF_Wx(); break;
            case IR::Code::NegI_W:  numChunkCODE += 4; break;
            case IR::Code::NegI_W2: genStmnt_NegI_W2(); break;
            case IR::Code::NegI_W3: genStmnt_NegI_W3(); break;

            case IR::Code::NotU_W:  numChunkCODE += 4; break;
            case IR::Code::NotU_W2: numChunkCODE += 8; break;
            case IR::Code::NotU_W3: numChunkCODE += 12; break;

            case IR::Code::Plsa:
               numChunkCODE += 32;
               break;

            case IR::Code::Plsd:
            case IR::Code::Plsi:
            case IR::Code::Plsr:
            case IR::Code::Plss:
               numChunkCODE += 8;
               break;

            case IR::Code::Plsf:
               numChunkCODE += 24;
               break;

            case IR::Code::Pltn:
               numChunkCODE += 12;
               break;

            case IR::Code::Retn:
               genStmnt_Retn();
               break;

            case IR::Code::ShLU_W2: genStmnt_ShLU_W2(); break;
            case IR::Code::ShLU_W3: genStmnt_ShLU_W3(); break;

            case IR::Code::ShRI_W2: genStmnt_ShRI_W2(); break;
            case IR::Code::ShRI_W3: genStmnt_ShRI_W3(); break;

            case IR::Code::ShRU_W:  genStmnt_ShRU_W(); break;
            case IR::Code::ShRU_W2: genStmnt_ShRU_W2(); break;
            case IR::Code::ShRU_W3: genStmnt_ShRU_W3(); break;

            case IR::Code::Swap_W:  numChunkCODE += 4; break;
            case IR::Code::Swap_W2: numChunkCODE += 64; break;
            case IR::Code::Swap_W3: numChunkCODE += 96; break;

            case IR::Code::Xcod_SID:
               numChunkCODE += 32;
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot gen Code for ZDACS: " << stmnt->code << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::genStmnt_Call
         //
         void Info::genStmnt_Call()
         {
            auto ret = stmnt->args[1].aLit.value->getValue().getFastU();

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               if(ret == 0)
                  numChunkCODE += 8;
               else
                  numChunkCODE += 8 + (ret - 1) * 16;

               break;

            case IR::ArgBase::Stk:
               if(ret == 0)
                  numChunkCODE += 8;
               else
                  numChunkCODE += 4 + (ret - 1) * 16;

               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Call\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::genStmnt_Cspe
         //
         void Info::genStmnt_Cspe()
         {
            auto ret = stmnt->args[1].aLit.value->getValue().getFastU();

            // No call args.
            if(stmnt->args.size() == 2)
            {
               numChunkCODE += ret ? 48 : 12;
               return;
            }

            switch(stmnt->args[2].a)
            {
            case IR::ArgBase::Lit:
               numChunkCODE += 8 + (stmnt->args.size() - 2) * (ret ? 8 : 4);
               break;

            case IR::ArgBase::Stk:
               numChunkCODE += 8;

               // Dummy args.
               if(ret) numChunkCODE += (7 - stmnt->args.size()) * 8;

               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::genStmnt_Retn
         //
         void Info::genStmnt_Retn()
         {
            auto argc = stmnt->args.size();

            switch(func->ctype)
            {
            case IR::CallType::StdCall:
            case IR::CallType::StkCall:
               if(argc == 0)
                  numChunkCODE += 4;
               else
                  numChunkCODE += 4 + (argc - 1) * 20;
               break;

            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               if(argc == 0)
                  numChunkCODE += 4;
               else if(argc == 1)
                  numChunkCODE += 8;
               else
               {
                  std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
                  throw EXIT_FAILURE;
               }
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Retn\n";
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

