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
            switch(stmnt->op.code)
            {
            case IR::Code::Nop:
               putCode(Code::Nop);
               break;

            case IR::Code::AddF_W:  putStmntCall("___GDCC__AddF_W",  1); break;
            case IR::Code::AddF_W2: putStmntCall("___GDCC__AddF_W2", 2); break;
            case IR::Code::AddI_W:
            case IR::Code::AddU_W:  putStmnt_AddU_W(); break;
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
                  putWord(GetWord(arg.aLit));
               break;

            case IR::Code::CmpF_EQ_W:  putStmntCall("___GDCC__CmpF_EQ_W",  1); break;
            case IR::Code::CmpF_EQ_W2: putStmntCall("___GDCC__CmpF_EQ_W2", 1); break;

            case IR::Code::CmpF_GE_W:  putStmntCall("___GDCC__CmpF_GE_W",  1); break;
            case IR::Code::CmpF_GE_W2: putStmntCall("___GDCC__CmpF_GE_W2", 1); break;

            case IR::Code::CmpF_GT_W:  putStmntCall("___GDCC__CmpF_GT_W",  1); break;
            case IR::Code::CmpF_GT_W2: putStmntCall("___GDCC__CmpF_GT_W2", 1); break;

            case IR::Code::CmpF_LE_W:  putStmntCall("___GDCC__CmpF_LE_W",  1); break;
            case IR::Code::CmpF_LE_W2: putStmntCall("___GDCC__CmpF_LE_W2", 1); break;

            case IR::Code::CmpF_LT_W:  putStmntCall("___GDCC__CmpF_LT_W",  1); break;
            case IR::Code::CmpF_LT_W2: putStmntCall("___GDCC__CmpF_LT_W2", 1); break;

            case IR::Code::CmpF_NE_W:  putStmntCall("___GDCC__CmpF_NE_W",  1); break;
            case IR::Code::CmpF_NE_W2: putStmntCall("___GDCC__CmpF_NE_W2", 1); break;

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
               putWord(stmnt->args.size() - 1);
               putWord(GetWord(stmnt->args[0].aLit));
               break;

            case IR::Code::Copy_W: putCode(Code::Copy); break;

            case IR::Code::Cscr_IA: putStmnt_Cscr_IA(); break;
            case IR::Code::Cscr_IS: putStmnt_Cscr_IS(); break;
            case IR::Code::Cscr_SA: putStmnt_Cscr_SA(); break;
            case IR::Code::Cscr_SS: putStmnt_Cscr_SS(); break;

            case IR::Code::Cspe: putStmnt_Cspe(); break;

            case IR::Code::DiXI_W:  putStmnt_DiXI_W(); break;
            case IR::Code::DiXI_W2: putStmntCall("___GDCC__DiXI_W2", 4); break;
            case IR::Code::DiXI_W3: putStmntCall("___GDCC__DiXI_W3", 6); break;
            case IR::Code::DiXU_W:  putStmntCall("___GDCC__DiXU_W",  2); break;
            case IR::Code::DiXU_W2: putStmntCall("___GDCC__DiXU_W2", 4); break;
            case IR::Code::DiXU_W3: putStmntCall("___GDCC__DiXU_W3", 6); break;

            case IR::Code::DivF_W:  putStmntCall("___GDCC__DivF_W",  1); break;
            case IR::Code::DivF_W2: putStmntCall("___GDCC__DivF_W2", 2); break;
            case IR::Code::DivI_W:  putCode(Code::DivI); break;
            case IR::Code::DivI_W2: putStmntCall("___GDCC__DivI_W2", 2); break;
            case IR::Code::DivI_W3: putStmntCall("___GDCC__DivI_W3", 3); break;
            case IR::Code::DivK_W:  putStmntCall("___GDCC__DivK_W",  1); break;
            case IR::Code::DivK_W2: putStmntCall("___GDCC__DivK_W2", 2); break;
            case IR::Code::DivU_W:  putStmntCall("___GDCC__DivU_W",  1); break;
            case IR::Code::DivU_W2: putStmntCall("___GDCC__DivU_W2", 2); break;
            case IR::Code::DivU_W3: putStmntCall("___GDCC__DivU_W3", 3); break;
            case IR::Code::DivX_W:  putCode(Code::DivX); break;
            case IR::Code::DivX_W2: putStmntCall("___GDCC__DivX_W2", 2); break;

            case IR::Code::InvU_W:  putCode(Code::InvU); break;
            case IR::Code::InvU_W2: putStmnt_InvU_W2(); break;
            case IR::Code::InvU_W3: putStmnt_InvU_W3(); break;

            case IR::Code::Jcnd_Nil:
               putCode(Code::Jcnd_Nil);
               putWord(GetWord(stmnt->args[1].aLit));
               break;

            case IR::Code::Jcnd_Tru:
               putCode(Code::Jcnd_Tru);
               putWord(GetWord(stmnt->args[1].aLit));
               break;

            case IR::Code::Jump:
               putCode(Code::Jump_Lit);
               putWord(GetWord(stmnt->args[0].aLit));
               break;

            case IR::Code::ModI_W:  putCode(Code::ModI); break;
            case IR::Code::ModI_W2: putStmntCall("___GDCC__ModI_W2", 2); break;
            case IR::Code::ModI_W3: putStmntCall("___GDCC__ModI_W3", 3); break;
            case IR::Code::ModU_W:  putStmntCall("___GDCC__ModU_W",  1); break;
            case IR::Code::ModU_W2: putStmntCall("___GDCC__ModU_W2", 2); break;
            case IR::Code::ModU_W3: putStmntCall("___GDCC__ModU_W3", 3); break;

            case IR::Code::Move_W:  putStmnt_Move_W(); break;
            case IR::Code::Move_W2: putStmnt_Move_Wx(2); break;
            case IR::Code::Move_W3: putStmnt_Move_Wx(3); break;
            case IR::Code::Move_W4: putStmnt_Move_Wx(4); break;
            case IR::Code::Move_W5: putStmnt_Move_Wx(5); break;
            case IR::Code::Move_W6: putStmnt_Move_Wx(6); break;

            case IR::Code::MuXU_W:  putStmntCall("___GDCC__MuXU_W",  2); break;
            case IR::Code::MuXU_W2: putStmntCall("___GDCC__MuXU_W2", 4); break;

            case IR::Code::MulF_W:  putStmntCall("___GDCC__MulF_W",  1); break;
            case IR::Code::MulF_W2: putStmntCall("___GDCC__MulF_W2", 2); break;
            case IR::Code::MulK_W:  putStmntCall("___GDCC__MulK_W",  1); break;
            case IR::Code::MulK_W2: putStmntCall("___GDCC__MulK_W2", 2); break;
            case IR::Code::MulI_W:
            case IR::Code::MulU_W:  putCode(Code::MulU); break;
            case IR::Code::MulI_W2:
            case IR::Code::MulU_W2: putStmntCall("___GDCC__MulU_W2", 2); break;
            case IR::Code::MulI_W3:
            case IR::Code::MulU_W3: putStmntCall("___GDCC__MulU_W3", 3); break;
            case IR::Code::MulX_W:  putCode(Code::MulX); break;
            case IR::Code::MulX_W2: putStmntCall("___GDCC__MulX_W2", 2); break;

            case IR::Code::NegF_W:
            case IR::Code::NegF_W2: putStmnt_NegF_Wx(); break;
            case IR::Code::NegI_W:  putCode(Code::NegI); break;
            case IR::Code::NegI_W2: putStmnt_NegI_W2(); break;
            case IR::Code::NegI_W3: putStmnt_NegI_W3(); break;

            case IR::Code::NotU_W3: putCode(Code::OrIU);
            case IR::Code::NotU_W2: putCode(Code::OrIU);
            case IR::Code::NotU_W:  putCode(Code::NotU); break;

            case IR::Code::OrIU_W:  putCode(Code::OrIU); break;
            case IR::Code::OrIU_W2: putStmntBitwise2(Code::OrIU); break;
            case IR::Code::OrIU_W3: putStmntBitwise3(Code::OrIU); break;

            case IR::Code::OrXU_W:  putCode(Code::OrXU); break;
            case IR::Code::OrXU_W2: putStmntBitwise2(Code::OrXU); break;
            case IR::Code::OrXU_W3: putStmntBitwise3(Code::OrXU); break;

            case IR::Code::Plsa:
               putCode(Code::Call_Lit,    GetWord(resolveGlyph("___GDCC__Plsa")));
               putCode(Code::Drop_LocReg, getStkPtrIdx());
               break;

            case IR::Code::Plsf:
               putCode(Code::Push_LocReg, getStkPtrIdx());
               putCode(Code::Call_Nul,    GetWord(resolveGlyph("___GDCC__Plsf")));
               break;

            case IR::Code::Pltn:
               putCode(Code::Push_LocReg);
               putWord(getStkPtrIdx());
               putCode(Code::AddU);
               break;

            case IR::Code::Retn:
               putStmnt_Retn();
               break;

            case IR::Code::ShLF_W:  putStmntCall("___GDCC__ShLF_W",  1); break;
            case IR::Code::ShLF_W2: putStmntCall("___GDCC__ShLF_W2", 2); break;

            case IR::Code::ShLU_W:  putCode(Code::ShLU); break;
            case IR::Code::ShLU_W2: putStmnt_ShLU_W2(); break;
            case IR::Code::ShLU_W3: putStmnt_ShLU_W3(); break;

            case IR::Code::ShRF_W:  putStmntCall("___GDCC__ShRF_W",  1); break;
            case IR::Code::ShRF_W2: putStmntCall("___GDCC__ShRF_W2", 2); break;

            case IR::Code::ShRI_W:  putCode(Code::ShRI); break;
            case IR::Code::ShRI_W2: putStmnt_ShRI_W2(); break;
            case IR::Code::ShRI_W3: putStmnt_ShRI_W3(); break;

            case IR::Code::ShRU_W:  putStmnt_ShRU_W(); break;
            case IR::Code::ShRU_W2: putStmnt_ShRU_W2(); break;
            case IR::Code::ShRU_W3: putStmnt_ShRU_W3(); break;

            case IR::Code::SubF_W:  putStmntCall("___GDCC__SubF_W",  1); break;
            case IR::Code::SubF_W2: putStmntCall("___GDCC__SubF_W2", 2); break;
            case IR::Code::SubI_W:
            case IR::Code::SubU_W:  putStmnt_SubU_W(); break;
            case IR::Code::SubI_W2:
            case IR::Code::SubU_W2: putStmntCall("___GDCC__SubU_W2", 2); break;
            case IR::Code::SubI_W3:
            case IR::Code::SubU_W3: putStmntCall("___GDCC__SubU_W3", 3); break;

            case IR::Code::Swap_W:  putCode(Code::Swap); break;
            case IR::Code::Swap_W2: putStmnt_Swap_Wx(2); break;
            case IR::Code::Swap_W3: putStmnt_Swap_Wx(3); break;

            case IR::Code::Xcod_SID:
               putStmnt_Xcod_SID();
               break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot put Code for ZDACS: " << stmnt->op << '\n';
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
            putCode(Code::Jcnd_Tru);
            putWord(putPos + 12);
            putCode(Code::Wait_Lit);
            putWord(1);
         }

         //
         // Info::putStmntCall
         //
         void Info::putStmntCall(Core::String name, Core::FastU ret)
         {
            putCode(Code::Call_Lit);
            putWord(GetWord(resolveGlyph(name)));

            if(ret) putStmntPushRetn(ret - 1);
         }

         //
         // Info::putStmntDropArg
         //
         void Info::putStmntDropArg(IR::Arg const &arg, Core::FastU w)
         {
            //
            // putArr
            //
            auto putArr = [&](IR::ArgPtr2 const &a, Code code)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit, GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Swap);
                  putCode(code,           GetWord(a.arr->aLit.value));
               }
               else
               {
                  putStmntPushArg(*a.idx, 0);

                  if(a.off + w)
                  {
                     putCode(Code::Push_Lit, a.off + w);
                     putCode(Code::AddU);
                  }

                  putCode(Code::Swap);
                  putCode(code, GetWord(a.arr->aLit.value));
               }
            };

            //
            // putLoc
            //
            auto putLoc = [&](IR::Arg_Loc const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Push_LocReg, getStkPtrIdx());
                  putCode(Code::AddU);
                  putCode(Code::Swap);
                  putCode(Code::Drop_GblArr, LocArsArray);
               }
               else
               {
                  putStmntPushArg(*a.idx, 0);

                  putCode(Code::Push_LocReg, getStkPtrIdx());
                  putCode(Code::AddU);

                  if(a.off + w)
                  {
                     putCode(Code::Push_Lit, a.off + w);
                     putCode(Code::AddU);
                  }

                  putCode(Code::Swap);
                  putCode(Code::Drop_GblArr, LocArsArray);
               }
            };

            //
            // putLocArs
            //
            auto putLocArs = [&](IR::Arg_LocArs const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Swap);
                  putCode(Code::Drop_GblArr, LocArsArray);
               }
               else
               {
                  putStmntPushArg(*a.idx, 0);

                  if(a.off + w)
                  {
                     putCode(Code::Push_Lit, a.off + w);
                     putCode(Code::AddU);
                  }

                  putCode(Code::Swap);
                  putCode(Code::Drop_GblArr, LocArsArray);
               }
            };

            //
            // putReg
            //
            auto putReg = [&](IR::ArgPtr1 const &a, Code code)
            {
               putCode(code);
               putWord(GetWord(a.idx->aLit) + a.off + w);
            };

            switch(arg.a)
            {
            case IR::ArgBase::GblArr: putArr(arg.aGblArr, Code::Drop_GblArr); break;
            case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::Drop_GblReg); break;
            case IR::ArgBase::Loc:    putLoc(arg.aLoc); break;
            case IR::ArgBase::LocArs: putLocArs(arg.aLocArs); break;
            case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Drop_LocReg); break;
            case IR::ArgBase::MapArr: putArr(arg.aMapArr, Code::Drop_MapArr); break;
            case IR::ArgBase::MapReg: putReg(arg.aMapReg, Code::Drop_MapReg); break;
            case IR::ArgBase::Nul:    putCode(Code::Drop_Nul); break;
            case IR::ArgBase::WldArr: putArr(arg.aWldArr, Code::Drop_WldArr); break;
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
            for(Core::FastU i = ret; i--;)
            {
               putCode(Code::Push_Lit);
               putWord(~i);
               putCode(Code::Swap);
               putCode(Code::Drop_GblArr);
               putWord(LocArsArray);
            }
         }

         //
         // Info::putStmntIncUArg
         //
         void Info::putStmntIncUArg(IR::Arg const &arg, Core::FastU w)
         {
            //
            // putReg
            //
            auto putReg = [&](IR::ArgPtr1 const &a, Code code)
            {
               putCode(code);
               putWord(GetWord(a.idx->aLit) + a.off + w);
            };

            switch(arg.a)
            {
            case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::IncU_GblReg); break;
            case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::IncU_LocReg); break;
            case IR::ArgBase::MapReg: putReg(arg.aMapReg, Code::IncU_MapReg); break;
            case IR::ArgBase::WldReg: putReg(arg.aWldReg, Code::IncU_WldReg); break;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad putStmntIncUArg");
            }
         }

         //
         // Info::putStmntIncUArg
         //
         void Info::putStmntIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
         {
            for(; lo != hi; ++lo)
               putStmntIncUArg(arg, lo);
         }

         //
         // Info::putStmntPushArg
         //
         void Info::putStmntPushArg(IR::Arg const &arg, Core::FastU w)
         {
            //
            // putArr
            //
            auto putArr = [&](IR::ArgPtr2 const &a, Code code)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit, GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(code,           GetWord(a.arr->aLit.value));
               }
               else
               {
                  putStmntPushArg(*a.idx, 0);

                  if(a.off + w)
                  {
                     putCode(Code::Push_Lit, a.off + w);
                     putCode(Code::AddU);
                  }

                  putCode(code, GetWord(a.arr->aLit.value));
               }
            };

            //
            // putLit
            //
            auto putLit = [&](IR::Arg_Lit const &a)
            {
               auto val  = a.value->getValue();
               auto wLit = a.off + w;

               switch(val.v)
               {
               case IR::ValueBase::Funct:
                  if(wLit == 0)
                  {
                     if(IsScriptS(val.vFunct.vtype.callT))
                        putStmntPushStrEn(val.vFunct.value);
                     else
                        putStmntPushFunct(val.vFunct.value);
                  }
                  else
                     putCode(Code::Push_Lit, 0);
                  break;

               case IR::ValueBase::StrEn:
                  if(wLit == 0)
                     putStmntPushStrEn(val.vStrEn.value);
                  else
                     putCode(Code::Push_Lit, 0);
                  break;

               default:
                  putCode(Code::Push_Lit, GetWord(a, w));
                  break;
               }
            };

            //
            // putLoc
            //
            auto putLoc = [&](IR::Arg_Loc const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Push_LocReg, getStkPtrIdx());
                  putCode(Code::AddU);
                  putCode(Code::Push_GblArr, LocArsArray);
               }
               else
               {
                  putStmntPushArg(*a.idx, 0);

                  putCode(Code::Push_LocReg, getStkPtrIdx());
                  putCode(Code::AddU);

                  if(a.off + w)
                  {
                     putCode(Code::Push_Lit, a.off + w);
                     putCode(Code::AddU);
                  }

                  putCode(Code::Push_GblArr, LocArsArray);
               }
            };

            //
            // putLocArs
            //
            auto putLocArs = [&](IR::Arg_LocArs const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Push_GblArr, LocArsArray);
               }
               else
               {
                  putStmntPushArg(*a.idx, 0);

                  if(a.off + w)
                  {
                     putCode(Code::Push_Lit, a.off + w);
                     putCode(Code::AddU);
                  }

                  putCode(Code::Push_GblArr, LocArsArray);
               }
            };

            //
            // putReg
            //
            auto putReg = [&](IR::ArgPtr1 const &a, Code code)
            {
               putCode(code);
               putWord(GetWord(a.idx->aLit) + a.off + w);
            };

            switch(arg.a)
            {
            case IR::ArgBase::GblArr: putArr(arg.aGblArr, Code::Push_GblArr); break;
            case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::Push_GblReg); break;
            case IR::ArgBase::Lit:    putLit(arg.aLit); break;
            case IR::ArgBase::Loc:    putLoc(arg.aLoc); break;
            case IR::ArgBase::LocArs: putLocArs(arg.aLocArs); break;
            case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Push_LocReg); break;
            case IR::ArgBase::MapArr: putArr(arg.aMapArr, Code::Push_MapArr); break;
            case IR::ArgBase::MapReg: putReg(arg.aMapReg, Code::Push_MapReg); break;
            case IR::ArgBase::WldArr: putArr(arg.aWldArr, Code::Push_WldArr); break;
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
            for(Core::FastU i = 0; i != ret; ++i)
            {
               putCode(Code::Push_Lit);
               putWord(~i);
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

