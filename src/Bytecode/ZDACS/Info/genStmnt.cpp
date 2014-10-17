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

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:
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
            case IR::Code::SubI_W:
            case IR::Code::SubU_W:
               numChunkCODE += 4;
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

            case IR::Code::Cnat:
               numChunkCODE += 12;
               break;

            case IR::Code::Cspe:
               genStmnt_Cspe();
               break;

            case IR::Code::DivU_W:
            case IR::Code::ModU_W:
               numChunkCODE += 8;
               break;

            case IR::Code::InvU_W:
            case IR::Code::NegI_W:
            case IR::Code::NotU_W:
               numChunkCODE += 4;
               break;

            case IR::Code::Jump:
               numChunkCODE += 8;
               break;

            case IR::Code::Move_W:
               genStmnt_Move_W();
               break;

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

            case IR::Code::Swap_W:
               numChunkCODE += 4;
               break;

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
         // Info::genStmnt_Move_W
         //
         void Info::genStmnt_Move_W()
         {
            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblArr:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  genStmnt_Move_W__Arr_Stk(stmnt->args[0].aGblArr);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::GblReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: numChunkCODE += 8; break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::Loc:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  numChunkCODE += IsExp0(stmnt->args[0].aLoc.off) ? 24 : 36;
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::LocArs:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  numChunkCODE += IsExp0(stmnt->args[0].aLocArs.off) ? 12 : 24;
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::LocReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: numChunkCODE += 8; break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::MapArr:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  genStmnt_Move_W__Arr_Stk(stmnt->args[0].aMapArr);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::MapReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: numChunkCODE += 8; break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::Nul:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: numChunkCODE += 4; break;
               default: goto badcase;
               }
               break;

            case IR::ArgBase::Stk:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::GblReg: numChunkCODE += 8; break;
               case IR::ArgBase::LocReg: numChunkCODE += 8; break;
               case IR::ArgBase::MapReg: numChunkCODE += 8; break;
               case IR::ArgBase::WldReg: numChunkCODE += 8; break;

               case IR::ArgBase::Lit:
                  genStmnt_Move_W__Stk_Lit(stmnt->args[1].aLit.value);
                  break;

               case IR::ArgBase::Loc:
                  numChunkCODE += IsExp0(stmnt->args[1].aLoc.off) ? 20 : 32;
                  break;

               case IR::ArgBase::LocArs:
                  numChunkCODE += IsExp0(stmnt->args[1].aLocArs.off) ? 8 : 20;
                  break;

               case IR::ArgBase::GblArr:
                  genStmnt_Move_W__Stk_Arr(stmnt->args[1].aGblArr);
                  break;

               case IR::ArgBase::MapArr:
                  genStmnt_Move_W__Stk_Arr(stmnt->args[1].aMapArr);
                  break;

               case IR::ArgBase::WldArr:
                  genStmnt_Move_W__Stk_Arr(stmnt->args[1].aWldArr);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::WldArr:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk:
                  genStmnt_Move_W__Arr_Stk(stmnt->args[0].aWldArr);
                  break;

               default: goto badcase;
               }
               break;

            case IR::ArgBase::WldReg:
               switch(stmnt->args[1].a)
               {
               case IR::ArgBase::Stk: numChunkCODE += 8; break;
               default: goto badcase;
               }
               break;

            default:
            badcase:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Code::Move_W("
                  << stmnt->args[0].a << ',' << stmnt->args[1].a << ")\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::genStmnt_Move_W__Arr_Stk
         //
         void Info::genStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr)
         {
            numChunkCODE += IsExp0(arr.off) ? 12 : 24;
         }

         //
         // Info::genStmnt_Move_W__Stk_Arr
         //
         void Info::genStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr)
         {
            numChunkCODE += IsExp0(arr.off) ? 8 : 20;
         }

         //
         // Info::genStmnt_Move_W__Stk_Lit
         //
         void Info::genStmnt_Move_W__Stk_Lit(IR::Exp const *exp)
         {
            auto type = exp->getType();

            switch(type.t)
            {
            case IR::TypeBase::Funct:
               if(type.tFunct.callT == IR::CallType::ScriptS)
                  numChunkCODE += 12;
               else
                  numChunkCODE +=  8;
               break;

            case IR::TypeBase::StrEn: numChunkCODE += 12; break;

            default: numChunkCODE += 8; break;
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
         // Info::genStmntBitwise2
         //
         void Info::genStmntBitwise2()
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               numChunkCODE += 56;
            }
            else
            {
               genStmntPushArg(stmnt->args[1], 0, 2);
               genStmntPushArg(stmnt->args[2], 0, 2);
               numChunkCODE += 8;
            }
         }

         //
         // Info::genStmntBitwise3
         //
         void Info::genStmntBitwise3()
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               numChunkCODE += 92;
            }
            else
            {
               genStmntPushArg(stmnt->args[1], 0, 3);
               genStmntPushArg(stmnt->args[2], 0, 3);
               numChunkCODE += 12;
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
         // Info::genStmntPushArg
         //
         void Info::genStmntPushArg(IR::Arg const &arg, Core::FastU)
         {
            switch(arg.a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::Lit:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::MapReg:
            case IR::ArgBase::WldReg:
               numChunkCODE += 8;
               break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad genStmntPushArg");
            }
         }

         //
         // Info::genStmntPushArg
         //
         void Info::genStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
         {
            for(; lo != hi; ++lo)
               genStmntPushArg(arg, lo);
         }
      }
   }
}

// EOF

