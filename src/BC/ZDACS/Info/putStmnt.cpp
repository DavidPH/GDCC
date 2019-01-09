//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE statement output.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::putStmnt
   //
   void Info::putStmnt()
   {
      switch(stmnt->code)
      {
      case IR::Code::Nop: putCode(Code::Nop); break;

      case IR::Code::AdXU: putStmnt_AdXU(); break;

      case IR::Code::AddF:
      case IR::Code::DivF:
      case IR::Code::DivK:
      case IR::Code::MulF:
      case IR::Code::MulK:
      case IR::Code::SubF:
         putStmntCall(getStmntSize());
         break;

      case IR::Code::AddI:
      case IR::Code::AddU: putStmnt_AddU(); break;

      case IR::Code::BAnd: putStmntBitwise(Code::BAnd); break;
      case IR::Code::BNot: putStmnt_BNot(); break;
      case IR::Code::BOrI: putStmntBitwise(Code::BOrI); break;
      case IR::Code::BOrX: putStmntBitwise(Code::BOrX); break;

      case IR::Code::Bclo: putStmnt_Bclz(true); break;
      case IR::Code::Bclz: putStmnt_Bclz(false); break;
      case IR::Code::Bges: putStmnt_Bges(); break;
      case IR::Code::Bget: putStmnt_Bget(); break;
      case IR::Code::Bset: putStmnt_Bset(); break;

      case IR::Code::Call: putStmnt_Call(); break;

      case IR::Code::Casm: putStmnt_Casm(); break;

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
         putStmntCall(1);
         break;

      case IR::Code::CmpI_EQ: putStmnt_CmpU_EQ(); break;
      case IR::Code::CmpI_GE: putStmnt_CmpI(Code::CmpI_GE); break;
      case IR::Code::CmpI_GT: putStmnt_CmpI(Code::CmpI_GT); break;
      case IR::Code::CmpI_LE: putStmnt_CmpI(Code::CmpI_LE); break;
      case IR::Code::CmpI_LT: putStmnt_CmpI(Code::CmpI_LT); break;
      case IR::Code::CmpI_NE: putStmnt_CmpU_NE(); break;

      case IR::Code::CmpU_EQ: putStmnt_CmpU_EQ(); break;
      case IR::Code::CmpU_NE: putStmnt_CmpU_NE(); break;

      case IR::Code::Cnat: putStmnt_Cnat(); break;

      case IR::Code::Copy: putCode(Code::Copy); break;

      case IR::Code::Cscr_IA: putStmnt_Cscr_IA(); break;
      case IR::Code::Cscr_IS: putStmnt_Cscr_IS(); break;
      case IR::Code::Cscr_SA: putStmnt_Cscr_SA(); break;
      case IR::Code::Cscr_SS: putStmnt_Cscr_SS(); break;

      case IR::Code::Cspe: putStmnt_Cspe(); break;

      case IR::Code::DiXI: putStmnt_DiXI(); break;
      case IR::Code::DiXU: putStmntCall(getStmntSize() * 2); break;

      case IR::Code::DivI: putStmnt_DivI(); break;
      case IR::Code::DivU: putStmnt_DivU(); break;
      case IR::Code::DivX: putStmnt_DivX(); break;

      case IR::Code::Jcnd_Nil: putStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: putStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: putStmnt_Jcnd_Tru(); break;

      case IR::Code::Jdyn: putCode(Code::Jdyn); break;

      case IR::Code::Jfar: putStmnt_Jfar(); break;
      case IR::Code::Jset: putStmnt_Jset(); break;
      case IR::Code::Jump: putStmnt_Jump(); break;
      case IR::Code::LAnd: putStmnt_LAnd(); break;
      case IR::Code::LNot: putStmnt_LNot(); break;
      case IR::Code::LOrI: putStmnt_LOrI(); break;
      case IR::Code::ModI: putStmnt_ModI(); break;
      case IR::Code::ModU: putStmnt_ModU(); break;
      case IR::Code::Move: putStmnt_Move(); break;

      case IR::Code::MuXU: putStmntCall(getStmntSize() * 2); break;

      case IR::Code::MulI:
      case IR::Code::MulU:
         if(auto n = getStmntSize(); n == 1)
            putCode(Code::MulU);
         else
            putStmntCall(n);
         break;

      case IR::Code::MulX:
         if(auto n = getStmntSize(); n == 1)
            putCode(Code::MulX);
         else
            putStmntCall(n);
         break;

      case IR::Code::NegF: putStmnt_NegF(); break;
      case IR::Code::NegI: putStmnt_NegI(); break;

      case IR::Code::Pltn: putStmnt_Pltn(); break;

      case IR::Code::Retn: putStmnt_Retn(); break;

      case IR::Code::ShLF: putStmnt_ShLF(); break;
      case IR::Code::ShLU: putStmnt_ShLU(); break;
      case IR::Code::ShRF: putStmnt_ShLF(); break;
      case IR::Code::ShRI: putStmnt_ShRI(); break;
      case IR::Code::ShRU: putStmnt_ShRU(); break;

      case IR::Code::SuXU: putStmnt_SuXU(); break;

      case IR::Code::SubI:
      case IR::Code::SubU: putStmnt_SubU(); break;

      case IR::Code::Swap: putStmnt_Swap(); break;

      case IR::Code::Xcod_SID: putStmnt_Xcod_SID(); break;

      default:
         Core::Error(stmnt->pos, "ZDACS cannot put Code: ", stmnt->code);
      }
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

      if(isInitiHubArr())
      {
         arr  = getInitHubArray();
         idx  = getInitHubIndex();
         code = Code::Push_HubArr;
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
      putStmntCall(getCallName(stmnt->code, getStmntSize()), ret);
   }

   //
   // Info::putStmntCall
   //
   void Info::putStmntCall(Core::String name, Core::FastU ret)
   {
      putCode(ret ? Code::Call_Lit : Code::Call_Nul);
      putWord(getWord(resolveGlyph(name)));

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
            putCode(Code::Push_Lit, getWord(a.idx->aLit.value) + a.off + w);
            putCode(Code::Swap);
            putCode(code,           getWord(a.arr->aLit.value));
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
            putCode(code, getWord(a.arr->aLit.value));
         }
      };

      //
      // putAut
      //
      auto putAut = [&](IR::Arg_Aut const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode(Code::Push_Lit,    getWord(a.idx->aLit.value) + a.off + w);
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
         putWord(getWord(a.idx->aLit) + a.off + w);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode(Code::Push_Lit,    getWord(a.idx->aLit.value) + a.off + w);
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
      case IR::ArgBase::HubArr: putArr(arg.aHubArr, Code::Drop_HubArr); break;
      case IR::ArgBase::HubReg: putReg(arg.aHubReg, Code::Drop_HubReg); break;
      case IR::ArgBase::LocArr: putArr(arg.aLocArr, Code::Drop_LocArr); break;
      case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Drop_LocReg); break;
      case IR::ArgBase::ModArr: putArr(arg.aModArr, Code::Drop_ModArr); break;
      case IR::ArgBase::ModReg: putReg(arg.aModReg, Code::Drop_ModReg); break;
      case IR::ArgBase::Nul:    putCode(Code::Drop_Nul); break;
      case IR::ArgBase::Sta:    putSta(arg.aSta); break;

      default:
         Core::Error(stmnt->pos, "bad putStmntDropArg");
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
   // Info::putStmntDropTmp
   //
   void Info::putStmntDropTmp(Core::FastU w)
   {
      putCode(Code::Drop_LocReg, func->localReg + w);
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
         putWord(getWord(a.idx->aLit) + a.off + w);
      };

      switch(arg.a)
      {
      case IR::ArgBase::GblReg: putReg(arg.aGblReg, Code::IncU_GblReg); break;
      case IR::ArgBase::HubReg: putReg(arg.aHubReg, Code::IncU_HubReg); break;
      case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::IncU_LocReg); break;
      case IR::ArgBase::ModReg: putReg(arg.aModReg, Code::IncU_ModReg); break;

      default:
         Core::Error(stmnt->pos, "bad putStmntIncUArg");
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
            putCode(Code::Push_Lit, getWord(a.idx->aLit.value) + a.off + w);
            putCode(code,           getWord(a.arr->aLit.value));
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode(Code::Push_Lit, a.off + w);
               putCode(Code::AddU);
            }

            putCode(code, getWord(a.arr->aLit.value));
         }
      };

      //
      // putAut
      //
      auto putAut = [&](IR::Arg_Aut const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode(Code::Push_Lit,    getWord(a.idx->aLit.value) + a.off + w);
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
            putCode(Code::Push_Lit, getWord(a, w));
            break;
         }
      };

      //
      // putReg
      //
      auto putReg = [&](IR::ArgPtr1 const &a, Code code)
      {
         putCode(code);
         putWord(getWord(a.idx->aLit) + a.off + w);
      };

      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode(Code::Push_Lit,    getWord(a.idx->aLit.value) + a.off + w);
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
      case IR::ArgBase::HubArr: putArr(arg.aHubArr, Code::Push_HubArr); break;
      case IR::ArgBase::HubReg: putReg(arg.aHubReg, Code::Push_HubReg); break;
      case IR::ArgBase::Lit:    putLit(arg.aLit); break;
      case IR::ArgBase::LocArr: putArr(arg.aLocArr, Code::Push_LocArr); break;
      case IR::ArgBase::LocReg: putReg(arg.aLocReg, Code::Push_LocReg); break;
      case IR::ArgBase::ModArr: putArr(arg.aModArr, Code::Push_ModArr); break;
      case IR::ArgBase::ModReg: putReg(arg.aModReg, Code::Push_ModReg); break;
      case IR::ArgBase::Sta:    putSta(arg.aSta); break;

      default:
         Core::Error(stmnt->pos, "bad putStmntPushArg");
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
      putCode(IsNull_Funct(value) ? Code::Push_Lit : Code::Pfun_Lit, value);
   }

   //
   // Info::putStmntPushIdx
   //
   void Info::putStmntPushIdx(IR::Arg const &arg, Core::FastU w)
   {
      //
      // putSta
      //
      auto putSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            putCode(Code::Push_Lit,    getWord(a.idx->aLit.value) + a.off + w);
         }
         else
         {
            putStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               putCode(Code::Push_Lit, a.off + w);
               putCode(Code::AddU);
            }
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Sta:    putSta(arg.aSta); break;

      default:
         Core::Error(stmnt->pos, "bad putStmntPushIdx");
      }
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
      putCode(Code::Push_Lit, value);
      putCode(IsNull_StrEn(value) ? Code::Nop : Code::Pstr_Stk);
   }

   //
   // Info::putStmntPushTmp
   //
   void Info::putStmntPushTmp(Core::FastU w)
   {
      putCode(Code::Push_LocReg, func->localReg + w);
   }
}

// EOF

