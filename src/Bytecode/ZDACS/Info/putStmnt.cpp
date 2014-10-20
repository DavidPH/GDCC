//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE statement output.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"
#include "Core/Range.hpp"

#include "IR/CallType.hpp"
#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

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
         // Info::putStmnt
         //
         void Info::putStmnt()
         {
            switch(stmnt->code)
            {
            case IR::Code::Nop:
               putCode(Code::Nop);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W:  putCode(Code::AddU); break;
            case IR::Code::AddI_W2:
            case IR::Code::AddU_W2: putStmntCall("___GDCC__AddU_W2", 2); break;
            case IR::Code::AddI_W3:
            case IR::Code::AddU_W3: putStmntCall("___GDCC__AddU_W3", 3); break;

            case IR::Code::AndU_W:  putCode(Code::AndU); break;
            case IR::Code::AndU_W2: putStmntBitwise2(Code::AndU); break;
            case IR::Code::AndU_W3: putStmntBitwise3(Code::AndU); break;

            case IR::Code::Call:
               putStmnt_Call();
               break;

            case IR::Code::Casm:
               for(auto const &arg : stmnt->args)
                  putWord(GetWord(arg.aLit.value));
               break;

            case IR::Code::Cjmp_Nil:
               putCode(Code::Cjmp_Nil);
               putWord(GetWord(stmnt->args[1].aLit.value));
               break;

            case IR::Code::Cjmp_Tru:
               putCode(Code::Cjmp_Tru);
               putWord(GetWord(stmnt->args[1].aLit.value));
               break;

            case IR::Code::CmpI_EQ_W:  putCode(Code::CmpU_EQ); break;
            case IR::Code::CmpI_EQ_W2: putStmnt_CmpU_EQ_W2(); break;
            case IR::Code::CmpI_EQ_W3: putStmnt_CmpU_EQ_W3(); break;

            case IR::Code::CmpI_GE_W:  putCode(Code::CmpI_GE); break;
            case IR::Code::CmpI_GE_W2: putStmntCall("___GDCC__CmpI_GE_W2", 1); break;
            case IR::Code::CmpI_GE_W3: putStmntCall("___GDCC__CmpI_GE_W3", 1); break;

            case IR::Code::CmpI_GT_W:  putCode(Code::CmpI_GT); break;
            case IR::Code::CmpI_GT_W2: putStmntCall("___GDCC__CmpI_GT_W2", 1); break;
            case IR::Code::CmpI_GT_W3: putStmntCall("___GDCC__CmpI_GT_W3", 1); break;

            case IR::Code::CmpI_LE_W:  putCode(Code::CmpI_LE); break;
            case IR::Code::CmpI_LE_W2: putStmntCall("___GDCC__CmpI_LE_W2", 1); break;
            case IR::Code::CmpI_LE_W3: putStmntCall("___GDCC__CmpI_LE_W3", 1); break;

            case IR::Code::CmpI_LT_W:  putCode(Code::CmpI_LT); break;
            case IR::Code::CmpI_LT_W2: putStmntCall("___GDCC__CmpI_LT_W2", 1); break;
            case IR::Code::CmpI_LT_W3: putStmntCall("___GDCC__CmpI_LT_W3", 1); break;

            case IR::Code::CmpI_NE_W:  putCode(Code::CmpU_NE); break;
            case IR::Code::CmpI_NE_W2: putStmnt_CmpU_NE_W2(); break;
            case IR::Code::CmpI_NE_W3: putStmnt_CmpU_NE_W3(); break;

            case IR::Code::CmpU_EQ_W:  putCode(Code::CmpU_EQ); break;
            case IR::Code::CmpU_EQ_W2: putStmnt_CmpU_EQ_W2(); break;
            case IR::Code::CmpU_EQ_W3: putStmnt_CmpU_EQ_W3(); break;

            case IR::Code::CmpU_GE_W:  putStmntCall("___GDCC__CmpU_GE_W",  1); break;
            case IR::Code::CmpU_GE_W2: putStmntCall("___GDCC__CmpU_GE_W2", 1); break;
            case IR::Code::CmpU_GE_W3: putStmntCall("___GDCC__CmpU_GE_W3", 1); break;

            case IR::Code::CmpU_GT_W:  putStmntCall("___GDCC__CmpU_GT_W",  1); break;
            case IR::Code::CmpU_GT_W2: putStmntCall("___GDCC__CmpU_GT_W2", 1); break;
            case IR::Code::CmpU_GT_W3: putStmntCall("___GDCC__CmpU_GT_W3", 1); break;

            case IR::Code::CmpU_LE_W:  putStmntCall("___GDCC__CmpU_LE_W",  1); break;
            case IR::Code::CmpU_LE_W2: putStmntCall("___GDCC__CmpU_LE_W2", 1); break;
            case IR::Code::CmpU_LE_W3: putStmntCall("___GDCC__CmpU_LE_W3", 1); break;

            case IR::Code::CmpU_LT_W:  putStmntCall("___GDCC__CmpU_LT_W",  1); break;
            case IR::Code::CmpU_LT_W2: putStmntCall("___GDCC__CmpU_LT_W2", 1); break;
            case IR::Code::CmpU_LT_W3: putStmntCall("___GDCC__CmpU_LT_W3", 1); break;

            case IR::Code::CmpU_NE_W:  putCode(Code::CmpU_NE); break;
            case IR::Code::CmpU_NE_W2: putStmnt_CmpU_NE_W2(); break;
            case IR::Code::CmpU_NE_W3: putStmnt_CmpU_NE_W3(); break;

            case IR::Code::Cnat:
               putCode(Code::Cnat);
               putWord(GetWord(stmnt->args[0].aLit.value));
               putWord(stmnt->args.size() - 2);
               break;

            case IR::Code::Cspe:
               putStmnt_Cspe();
               break;

            case IR::Code::DivI_W:
               putCode(Code::DivI);
               break;

            case IR::Code::DivU_W:
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__DivU_W")));
               break;

            case IR::Code::DivX_W:
               putCode(Code::DivX);
               break;

            case IR::Code::InvU_W:  putCode(Code::InvU); break;
            case IR::Code::InvU_W2: putStmnt_InvU_W2(); break;
            case IR::Code::InvU_W3: putStmnt_InvU_W3(); break;

            case IR::Code::Jump:
               putCode(Code::Jump_Lit);
               putWord(GetWord(stmnt->args[0].aLit.value));
               break;

            case IR::Code::ModI_W:
               putCode(Code::ModI);
               break;

            case IR::Code::ModU_W:
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__ModU_W")));
               break;

            case IR::Code::Move_W:  putStmnt_Move_W(); break;
            case IR::Code::Move_W2: putStmnt_Move_Wx(2); break;
            case IR::Code::Move_W3: putStmnt_Move_Wx(3); break;

            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
               putCode(Code::MulU);
               break;

            case IR::Code::MulX_W:
               putCode(Code::MulX);
               break;

            case IR::Code::NegI_W:
               putCode(Code::NegI);
               break;

            case IR::Code::NotU_W:
               putCode(Code::NotU);
               break;

            case IR::Code::OrIU_W:  putCode(Code::OrIU); break;
            case IR::Code::OrIU_W2: putStmntBitwise2(Code::OrIU); break;
            case IR::Code::OrIU_W3: putStmntBitwise3(Code::OrIU); break;

            case IR::Code::OrXU_W:  putCode(Code::OrXU); break;
            case IR::Code::OrXU_W2: putStmntBitwise2(Code::OrXU); break;
            case IR::Code::OrXU_W3: putStmntBitwise3(Code::OrXU); break;

            case IR::Code::Plsa:
               putCode(Code::Push_Lit);
               putWord(0);
               putCode(Code::Push_Lit);
               putWord(GetWord(stmnt->args[0].aLit.value));
               putCode(Code::Call_Lit);
               putWord(GetWord(resolveGlyph("___GDCC__alloc")));
               putCode(Code::Drop_LocReg);
               putWord(getStkPtrIdx());
               break;

            case IR::Code::Plsf:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::Push_Lit);
               putWord(0);
               putCode(Code::Call_Nul);
               putWord(GetWord(resolveGlyph("___GDCC__alloc")));
               break;

            case IR::Code::Pltn:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::AddU);
               break;

            case IR::Code::Retn:
               putStmnt_Retn();
               break;

            case IR::Code::ShLU_W:  putCode(Code::ShLU); break;
            case IR::Code::ShLU_W2: putStmnt_ShLU_W2(); break;
            case IR::Code::ShLU_W3: putStmnt_ShLU_W3(); break;

            case IR::Code::ShRI_W:  putCode(Code::ShRI); break;
            case IR::Code::ShRI_W2: putStmnt_ShRI_W2(); break;
            case IR::Code::ShRI_W3: putStmnt_ShRI_W3(); break;

            case IR::Code::ShRU_W:  putStmnt_ShRU_W(); break;
            case IR::Code::ShRU_W2: putStmnt_ShRU_W2(); break;
            case IR::Code::ShRU_W3: putStmnt_ShRU_W3(); break;

            case IR::Code::SubI_W:
            case IR::Code::SubU_W:  putCode(Code::SubU); break;
            case IR::Code::SubI_W2:
            case IR::Code::SubU_W2: putStmntCall("___GDCC__SubU_W2", 2); break;
            case IR::Code::SubI_W3:
            case IR::Code::SubU_W3: putStmntCall("___GDCC__SubU_W3", 3); break;

            case IR::Code::Swap_W:
               putCode(Code::Swap);
               break;

            case IR::Code::Xcod_SID:
               putStmnt_Xcod_SID();
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot put Code for ZDACS: " << stmnt->code << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Call
         //
         void Info::putStmnt_Call()
         {
            auto ret = stmnt->args[1].aLit.value->getValue().getFastU();

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::Lit:
               if(ret == 0)
                  putCode(Code::Call_Nul);
               else
                  putCode(Code::Call_Lit);

               putWord(GetWord(stmnt->args[0].aLit.value));

               if(ret)
                  putStmntPushRetn(ret);

               break;

            case IR::ArgBase::Stk:
               putCode(Code::Call_Stk);

               if(ret == 0)
                  putCode(Code::Drop_Nul);
               else
                  putStmntPushRetn(ret);

               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Call\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Cspe
         //
         void Info::putStmnt_Cspe()
         {
            auto ret = stmnt->args[1].aLit.value->getValue().getFastU();

            IR::ArgBase a;
            if(stmnt->args.size() == 2)
               a = ret ? IR::ArgBase::Stk : IR::ArgBase::Lit;
            else
               a = stmnt->args[2].a;

            switch(a)
            {
            case IR::ArgBase::Lit:
               if(ret)
               {
                  for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
                  {
                     putCode(Code::Push_Lit);
                     putWord(GetWord(arg.aLit.value));
                  }

                  putCode(Code::Cspe_5R1); break;
                  putWord(GetWord(stmnt->args[0].aLit.value));

                  break;
               }

               switch(stmnt->args.size() - 2)
               {
               case 0: putCode(Code::Cspe_1L); break;
               case 1: putCode(Code::Cspe_1L); break;
               case 2: putCode(Code::Cspe_2L); break;
               case 3: putCode(Code::Cspe_3L); break;
               case 4: putCode(Code::Cspe_4L); break;
               case 5: putCode(Code::Cspe_5L); break;
               }

               putWord(GetWord(stmnt->args[0].aLit.value));

               // Dummy arg.
               if(stmnt->args.size() == 2)
                  putWord(0);

               for(auto const &arg : Core::MakeRange(stmnt->args.begin() + 2, stmnt->args.end()))
                  putWord(GetWord(arg.aLit.value));

               break;

            case IR::ArgBase::Stk:
               if(ret)
               {
                  switch(stmnt->args.size() - 2)
                  {
                  case 0: putCode(Code::Push_Lit); putWord(0);
                  case 1: putCode(Code::Push_Lit); putWord(0);
                  case 2: putCode(Code::Push_Lit); putWord(0);
                  case 3: putCode(Code::Push_Lit); putWord(0);
                  case 4: putCode(Code::Push_Lit); putWord(0);
                  case 5: putCode(Code::Cspe_5R1); break;
                  }
               }
               else
               {
                  switch(stmnt->args.size() - 2)
                  {
                  case 0: putCode(Code::Push_Lit); putWord(0);
                  case 1: putCode(Code::Cspe_1); break;
                  case 2: putCode(Code::Cspe_2); break;
                  case 3: putCode(Code::Cspe_3); break;
                  case 4: putCode(Code::Cspe_4); break;
                  case 5: putCode(Code::Cspe_5); break;
                  }
               }

               putWord(GetWord(stmnt->args[0].aLit.value));

               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad Cspe\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_Retn
         //
         void Info::putStmnt_Retn()
         {
            switch(func->ctype)
            {
            case IR::CallType::StdCall:
            case IR::CallType::StkCall:
               if(stmnt->args.size() == 0)
                  putCode(Code::Retn_Nul);
               else
               {
                  putStmntDropRetn(stmnt->args.size());
                  putCode(Code::Retn_Stk);
               }
               break;

            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               if(stmnt->args.size() == 0)
               {
                  putCode(Code::Rscr);
               }
               else if(stmnt->args.size() == 1)
               {
                  putCode(Code::Drop_ScrRet);
                  putCode(Code::Rscr);
               }
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
         // Info::putStmnt_Xcod_SID
         //
         void Info::putStmnt_Xcod_SID()
         {
            Core::FastU arr, idx;
            Code        code;

            if(isInitiWldArr())
            {
               arr  = getInitWldArray();
               idx  = getInitWldIndex();
               code = Code::Push_WldArr;
            }
            else if(isInitiGblArr())
            {
               arr  = getInitGblArray();
               idx  = getInitGblIndex();
               code = Code::Push_GblArr;
            }
            else
            {
               putCode(Code::Jump_Lit);
               putWord(putPos + 28);
               for(int i = 6; i--;) putCode(Code::Nop);
               return;
            }

            putCode(Code::Push_Lit);
            putWord(idx);
            putCode(code);
            putWord(arr);
            putCode(Code::Cjmp_Tru);
            putWord(putPos + 12);
            putCode(Code::Wait_Lit);
            putWord(1);
         }

         //
         // Info::putStmntBitwise2
         //
         void Info::putStmntBitwise2(Code code)
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 1);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 2);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 1);
               putCode(code);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 2);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 0);
               putCode(code);
            }
            else
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(code);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(code);
            }
         }

         //
         // Info::putStmntBitwise3
         //
         void Info::putStmntBitwise3(Code code)
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 1);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 2);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 3);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 4);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 2);
               putCode(code);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 4);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 1);
               putCode(code);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 3);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 0);
               putCode(code);
            }
            else
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(code);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(code);
               putStmntPushArg(stmnt->args[1], 2);
               putStmntPushArg(stmnt->args[2], 2);
               putCode(code);
            }
         }

         //
         // Info::putStmntCall
         //
         void Info::putStmntCall(Core::String name, Core::FastU ret)
         {
            putCode(Code::Call_Lit);
            putWord(GetWord(resolveGlyph(name)));

            putStmntPushRetn(ret);
         }

         //
         // Info::putStmntDropArg
         //
         void Info::putStmntDropArg(IR::Arg const &arg, Core::FastU w)
         {
            //
            // putReg
            //
            auto putReg = [&](IR::ArgPtr1 const &a, Code code)
            {
               putCode(code);
               putWord(GetWord(a.idx->aLit.value) + GetWord(a.off) + w);
            };

            switch(arg.a)
            {
            case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::Drop_GblReg); break;
            case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Drop_LocReg); break;
            case IR::ArgBase::MapReg: putReg(arg.aMapReg, Code::Drop_MapReg); break;
            case IR::ArgBase::Nul:    putCode(Code::Drop_Nul); break;
            case IR::ArgBase::WldReg: putReg(arg.aWldReg, Code::Drop_WldReg); break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad putStmntDropArg");
            }
         }

         //
         // Info::putStmntDropArg
         //
         void Info::putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
         {
            while(hi-- != lo)
               putStmntDropArg(arg, hi);
         }

         //
         // Info::putStmntDropRetn
         //
         void Info::putStmntDropRetn(Core::FastU ret)
         {
            if(ret) for(Core::FastU i = 0; ++i != ret;)
            {
               putCode(Code::Push_Lit);
               putWord(-i);
               putCode(Code::Swap);
               putCode(Code::Drop_GblArr);
               putWord(LocArsArray);
            }
         }

         //
         // Info::putStmntPushArg
         //
         void Info::putStmntPushArg(IR::Arg const &arg, Core::FastU w)
         {
            //
            // putLit
            //
            auto putLit = [&](IR::Arg_Lit const &a)
            {
               auto val = a.value->getValue();

               switch(val.v)
               {
               case IR::ValueBase::Funct:
                  if(w == 0)
                  {
                     if(val.vFunct.vtype.callT == IR::CallType::ScriptS)
                        putStmntPushStrEn(val.vFunct.value);
                     else
                        putStmntPushFunct(val.vFunct.value);
                  }
                  else
                     putCode(Code::Push_Lit, 0);
                  break;

               case IR::ValueBase::StrEn:
                  if(w == 0)
                     putStmntPushStrEn(val.vStrEn.value);
                  else
                     putCode(Code::Push_Lit, 0);
                  break;

               default:
                  putCode(Code::Push_Lit, GetWord(a.value, w));
                  break;
               }
            };

            //
            // putReg
            //
            auto putReg = [&](IR::ArgPtr1 const &a, Code code)
            {
               putCode(code);
               putWord(GetWord(a.idx->aLit.value) + GetWord(a.off) + w);
            };

            switch(arg.a)
            {
            case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::Push_GblReg); break;
            case IR::ArgBase::Lit:    putLit(arg.aLit); break;
            case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Push_LocReg); break;
            case IR::ArgBase::MapReg: putReg(arg.aMapReg, Code::Push_MapReg); break;
            case IR::ArgBase::WldReg: putReg(arg.aWldReg, Code::Push_WldReg); break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad putStmntPushArg");
            }
         }

         //
         // Info::putStmntPushArg
         //
         void Info::putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
         {
            for(; lo != hi; ++lo)
               putStmntPushArg(arg, lo);
         }

         //
         // Info::putStmntPushFunct
         //
         void Info::putStmntPushFunct(Core::FastU value)
         {
            if((value & 0xFFFFFFFF) == 0)
               putCode(Code::Push_Lit);
            else
               putCode(Code::Pfun_Lit);

            putWord(value);
         }

         //
         // Info::putStmntPushRetn
         //
         void Info::putStmntPushRetn(Core::FastU ret)
         {
            if(ret) while(ret-- != 1)
            {
               putCode(Code::Push_Lit);
               putWord(-ret);
               putCode(Code::Push_GblArr);
               putWord(LocArsArray);
            }
         }

         //
         // Info::putStmntPushStrEn
         //
         void Info::putStmntPushStrEn(Core::FastU value)
         {
            putCode(Code::Push_Lit);
            putWord(value);

            if((value & 0xFFFFFFFF) == 0xFFFFFFFF)
               putCode(Code::Nop);
            else
               putCode(Code::Pstr_Stk);
         }
      }
   }
}

// EOF

