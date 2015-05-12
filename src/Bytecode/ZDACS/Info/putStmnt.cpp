//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
            case IR::Code::Nop: putCode(Code::Nop); break;

            case IR::Code::AdXU_W: putStmnt_AdXU_W(); break;

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
               putStmntCall(stmnt->op.size);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W: putStmnt_AddU_W(); break;

            case IR::Code::AndU_W: putStmntBitwise(Code::AndU); break;

            case IR::Code::Bclo_W: putStmnt_Bclz_W(true); break;
            case IR::Code::Bclz_W: putStmnt_Bclz_W(false); break;

            case IR::Code::Call: putStmnt_Call(); break;

            case IR::Code::Casm: putStmnt_Casm(); break;

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
               putStmntCall(1);
               break;

            case IR::Code::CmpI_EQ_W: putStmnt_CmpU_EQ_W(); break;
            case IR::Code::CmpI_GE_W: putStmnt_CmpI_W(Code::CmpI_GE); break;
            case IR::Code::CmpI_GT_W: putStmnt_CmpI_W(Code::CmpI_GT); break;
            case IR::Code::CmpI_LE_W: putStmnt_CmpI_W(Code::CmpI_LE); break;
            case IR::Code::CmpI_LT_W: putStmnt_CmpI_W(Code::CmpI_LT); break;
            case IR::Code::CmpI_NE_W: putStmnt_CmpU_NE_W(); break;

            case IR::Code::CmpU_EQ_W: putStmnt_CmpU_EQ_W(); break;
            case IR::Code::CmpU_NE_W: putStmnt_CmpU_NE_W(); break;

            case IR::Code::Cnat: putStmnt_Cnat(); break;

            case IR::Code::Copy_W: putCode(Code::Copy); break;

            case IR::Code::Cscr_IA: putStmnt_Cscr_IA(); break;
            case IR::Code::Cscr_IS: putStmnt_Cscr_IS(); break;
            case IR::Code::Cscr_SA: putStmnt_Cscr_SA(); break;
            case IR::Code::Cscr_SS: putStmnt_Cscr_SS(); break;

            case IR::Code::Cspe: putStmnt_Cspe(); break;

            case IR::Code::DiXI_W: putStmnt_DiXI_W(); break;
            case IR::Code::DiXU_W: putStmntCall(stmnt->op.size * 2); break;

            case IR::Code::DivI_W: putStmnt_DivI_W(); break;
            case IR::Code::DivX_W: putStmnt_DivX_W(); break;

            case IR::Code::InvU_W: putStmnt_InvU_W(); break;

            case IR::Code::Jcnd_Nil: putStmnt_Jcnd_Nil(); break;
            case IR::Code::Jcnd_Tru: putStmnt_Jcnd_Tru(); break;

            case IR::Code::Jump: putStmnt_Jump(); break;

            case IR::Code::LAnd:
               if(stmnt->op.size == 1)
                  putCode(Code::LAnd);
               else
                  putStmntCall(stmnt->op.size);
               break;

            case IR::Code::LOrI:
               if(stmnt->op.size == 1)
                  putCode(Code::LOrI);
               else
                  putStmntCall(stmnt->op.size);
               break;

            case IR::Code::ModI_W: putStmnt_ModI_W(); break;

            case IR::Code::Move_W: putStmnt_Move_W(); break;

            case IR::Code::MuXU_W: putStmntCall(stmnt->op.size * 2); break;

            case IR::Code::MulI_W:
            case IR::Code::MulU_W:
               if(stmnt->op.size == 1)
                  putCode(Code::MulU);
               else
                  putStmntCall(stmnt->op.size);
               break;

            case IR::Code::MulX_W:
               if(stmnt->op.size == 1)
                  putCode(Code::MulX);
               else
                  putStmntCall(stmnt->op.size);
               break;

            case IR::Code::NegF_W: putStmnt_NegF_W(); break;
            case IR::Code::NegI_W: putStmnt_NegI_W(); break;

            case IR::Code::NotU_W: putStmnt_NotU_W(); break;

            case IR::Code::OrIU_W: putStmntBitwise(Code::OrIU); break;
            case IR::Code::OrXU_W: putStmntBitwise(Code::OrXU); break;

            case IR::Code::Plsa: putStmnt_Plsa(); break;
            case IR::Code::Plsf: putStmnt_Plsf(); break;
            case IR::Code::Pltn: putStmnt_Pltn(); break;

            case IR::Code::Retn: putStmnt_Retn(); break;

            case IR::Code::ShLU_W: putStmnt_ShLU_W(); break;
            case IR::Code::ShRI_W: putStmnt_ShRI_W(); break;
            case IR::Code::ShRU_W: putStmnt_ShRU_W(); break;

            case IR::Code::SuXU_W: putStmnt_SuXU_W(); break;

            case IR::Code::SubI_W:
            case IR::Code::SubU_W: putStmnt_SubU_W(); break;

            case IR::Code::Swap_W: putStmnt_Swap_W(); break;

            case IR::Code::Xcod_SID: putStmnt_Xcod_SID(); break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot put Code for ZDACS: " << stmnt->op << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::putStmnt_NotU_W
         //
         void Info::putStmnt_NotU_W()
         {
            if(auto i = stmnt->op.size) while(--i) putCode(Code::OrIU);
            putCode(Code::NotU);
         }

         //
         // Info::putStmnt_Plsa
         //
         void Info::putStmnt_Plsa()
         {
            putCode(Code::Call_Lit,    GetWord(resolveGlyph("___GDCC__Plsa")));
            putCode(Code::Drop_LocReg, getStkPtrIdx());
         }

         //
         // Info::putStmnt_Plsf
         //
         void Info::putStmnt_Plsf()
         {
            putCode(Code::Push_LocReg, getStkPtrIdx());
            putCode(Code::Call_Nul,    GetWord(resolveGlyph("___GDCC__Plsf")));
         }

         //
         // Info::putStmnt_Pltn
         //
         void Info::putStmnt_Pltn()
         {
            putCode(Code::Push_LocReg, getStkPtrIdx());
            putCode(Code::AddU);
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
         void Info::putStmntCall(Core::FastU ret)
         {
            putStmntCall(getCallName(), ret);
         }

         //
         // Info::putStmntCall
         //
         void Info::putStmntCall(Core::String name, Core::FastU ret)
         {
            putCode(ret ? Code::Call_Lit : Code::Call_Nul);
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
            // putAut
            //
            auto putAut = [&](IR::Arg_Aut const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Push_LocReg, getStkPtrIdx());
                  putCode(Code::AddU);
                  putCode(Code::Swap);
                  putCode(Code::Drop_GblArr, StaArray);
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
                  putCode(Code::Drop_GblArr, StaArray);
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

            //
            // putSta
            //
            auto putSta = [&](IR::Arg_Sta const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Swap);
                  putCode(Code::Drop_GblArr, StaArray);
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
                  putCode(Code::Drop_GblArr, StaArray);
               }
            };

            switch(arg.a)
            {
            case IR::ArgBase::Aut:    putAut(arg.aAut); break;
            case IR::ArgBase::GblArr: putArr(arg.aGblArr, Code::Drop_GblArr); break;
            case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::Drop_GblReg); break;
            case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Drop_LocReg); break;
            case IR::ArgBase::MapArr: putArr(arg.aMapArr, Code::Drop_MapArr); break;
            case IR::ArgBase::MapReg: putReg(arg.aMapReg, Code::Drop_MapReg); break;
            case IR::ArgBase::Nul:    putCode(Code::Drop_Nul); break;
            case IR::ArgBase::Sta:    putSta(arg.aSta); break;
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
               putWord(StaArray);
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
            // putAut
            //
            auto putAut = [&](IR::Arg_Aut const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Push_LocReg, getStkPtrIdx());
                  putCode(Code::AddU);
                  putCode(Code::Push_GblArr, StaArray);
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

                  putCode(Code::Push_GblArr, StaArray);
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
            // putReg
            //
            auto putReg = [&](IR::ArgPtr1 const &a, Code code)
            {
               putCode(code);
               putWord(GetWord(a.idx->aLit) + a.off + w);
            };

            //
            // putSta
            //
            auto putSta = [&](IR::Arg_Sta const &a)
            {
               if(a.idx->a == IR::ArgBase::Lit)
               {
                  putCode(Code::Push_Lit,    GetWord(a.idx->aLit.value) + a.off + w);
                  putCode(Code::Push_GblArr, StaArray);
               }
               else
               {
                  putStmntPushArg(*a.idx, 0);

                  if(a.off + w)
                  {
                     putCode(Code::Push_Lit, a.off + w);
                     putCode(Code::AddU);
                  }

                  putCode(Code::Push_GblArr, StaArray);
               }
            };

            switch(arg.a)
            {
            case IR::ArgBase::Aut:    putAut(arg.aAut); break;
            case IR::ArgBase::GblArr: putArr(arg.aGblArr, Code::Push_GblArr); break;
            case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::Push_GblReg); break;
            case IR::ArgBase::Lit:    putLit(arg.aLit); break;
            case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Push_LocReg); break;
            case IR::ArgBase::MapArr: putArr(arg.aMapArr, Code::Push_MapArr); break;
            case IR::ArgBase::MapReg: putReg(arg.aMapReg, Code::Push_MapReg); break;
            case IR::ArgBase::Sta:    putSta(arg.aSta); break;
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
               putWord(StaArray);
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

