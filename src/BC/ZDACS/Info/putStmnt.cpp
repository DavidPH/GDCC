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

      case IR::Code::AddF: putStmnt_AddF(); break;
      case IR::Code::AddI: putStmnt_AddI(); break;
      case IR::Code::AddU: putStmnt_AddU(); break;
      case IR::Code::BAnd: putStmnt_BAnd(); break;
      case IR::Code::BNot: putStmnt_BNot(); break;
      case IR::Code::BOrI: putStmnt_BOrI(); break;
      case IR::Code::BOrX: putStmnt_BOrX(); break;
      case IR::Code::Bclo: putStmnt_Bclo(); break;
      case IR::Code::Bclz: putStmnt_Bclz(); break;
      case IR::Code::Bges: putStmnt_Bges(); break;
      case IR::Code::Bget: putStmnt_Bget(); break;
      case IR::Code::Bset: putStmnt_Bset(); break;
      case IR::Code::Call: putStmnt_Call(); break;
      case IR::Code::Casm: putStmnt_Casm(); break;

      case IR::Code::CmpF_EQ: putStmnt_CmpF_EQ(); break;
      case IR::Code::CmpF_GE: putStmnt_CmpF_GE(); break;
      case IR::Code::CmpF_GT: putStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_LE: putStmnt_CmpF_LE(); break;
      case IR::Code::CmpF_LT: putStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_NE: putStmnt_CmpF_NE(); break;
      case IR::Code::CmpI_EQ: putStmnt_CmpI_EQ(); break;
      case IR::Code::CmpI_GE: putStmnt_CmpI_GE(); break;
      case IR::Code::CmpI_GT: putStmnt_CmpI_GT(); break;
      case IR::Code::CmpI_LE: putStmnt_CmpI_LE(); break;
      case IR::Code::CmpI_LT: putStmnt_CmpI_LT(); break;
      case IR::Code::CmpI_NE: putStmnt_CmpI_NE(); break;
      case IR::Code::CmpU_EQ: putStmnt_CmpU_EQ(); break;
      case IR::Code::CmpU_GE: putStmnt_CmpU_GE(); break;
      case IR::Code::CmpU_GT: putStmnt_CmpU_GT(); break;
      case IR::Code::CmpU_LE: putStmnt_CmpU_LE(); break;
      case IR::Code::CmpU_LT: putStmnt_CmpU_LT(); break;
      case IR::Code::CmpU_NE: putStmnt_CmpU_NE(); break;

      case IR::Code::Cnat: putStmnt_Cnat(); break;
      case IR::Code::Copy: putStmnt_Copy(); break;

      case IR::Code::Cscr_IA: putStmnt_Cscr_IA(); break;
      case IR::Code::Cscr_IS: putStmnt_Cscr_IS(); break;
      case IR::Code::Cscr_SA: putStmnt_Cscr_SA(); break;
      case IR::Code::Cscr_SS: putStmnt_Cscr_SS(); break;

      case IR::Code::Cspe: putStmnt_Cspe(); break;
      case IR::Code::DiXI: putStmnt_DiXI(); break;
      case IR::Code::DiXU: putStmnt_DiXU(); break;
      case IR::Code::DivA: putStmnt_DivA(); break;
      case IR::Code::DivF: putStmnt_DivF(); break;
      case IR::Code::DivK: putStmnt_DivK(); break;
      case IR::Code::DivR: putStmnt_DivR(); break;
      case IR::Code::DivI: putStmnt_DivI(); break;
      case IR::Code::DivU: putStmnt_DivU(); break;
      case IR::Code::DivX: putStmnt_DivX(); break;

      case IR::Code::Jcnd_Nil: putStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: putStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: putStmnt_Jcnd_Tru(); break;

      case IR::Code::Jdyn: putStmnt_Jdyn(); break;

      case IR::Code::Jfar_Pro: putStmnt_Jfar_Pro(); break;
      case IR::Code::Jfar_Set: putStmnt_Jfar_Set(); break;
      case IR::Code::Jfar_Sta: putStmnt_Jfar_Sta(); break;

      case IR::Code::Jump: putStmnt_Jump(); break;
      case IR::Code::LAnd: putStmnt_LAnd(); break;
      case IR::Code::LNot: putStmnt_LNot(); break;
      case IR::Code::LOrI: putStmnt_LOrI(); break;
      case IR::Code::ModI: putStmnt_ModI(); break;
      case IR::Code::ModU: putStmnt_ModU(); break;
      case IR::Code::Move: putStmnt_Move(); break;
      case IR::Code::MuXU: putStmnt_MuXU(); break;
      case IR::Code::MulA: putStmnt_MulA(); break;
      case IR::Code::MulF: putStmnt_MulF(); break;
      case IR::Code::MulI: putStmnt_MulI(); break;
      case IR::Code::MulK: putStmnt_MulK(); break;
      case IR::Code::MulR: putStmnt_MulR(); break;
      case IR::Code::MulU: putStmnt_MulU(); break;
      case IR::Code::MulX: putStmnt_MulX(); break;
      case IR::Code::NegF: putStmnt_NegF(); break;
      case IR::Code::NegI: putStmnt_NegI(); break;
      case IR::Code::Pltn: putStmnt_Pltn(); break;
      case IR::Code::Retn: putStmnt_Retn(); break;
      case IR::Code::Rjnk: putStmnt_Rjnk(); break;
      case IR::Code::ShLF: putStmnt_ShLF(); break;
      case IR::Code::ShLU: putStmnt_ShLU(); break;
      case IR::Code::ShRF: putStmnt_ShRF(); break;
      case IR::Code::ShRI: putStmnt_ShRI(); break;
      case IR::Code::ShRU: putStmnt_ShRU(); break;
      case IR::Code::SuXU: putStmnt_SuXU(); break;
      case IR::Code::SubF: putStmnt_SubF(); break;
      case IR::Code::SubI: putStmnt_SubI(); break;
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
   void Info::putStmntCall(Core::String name, Core::FastU retn)
   {
      putCode(retn ? Code::Call_Lit : Code::Call_Nul, getWord(resolveGlyph(name)));
      putStmntPushRetn(retn, GetRetnMax(IR::CallType::StkCall));
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
   // Info::putStmntDropParam
   //
   void Info::putStmntDropParam(Core::FastU param, Core::FastU paramMax)
   {
      putStmntDropRetn(param, paramMax);
   }

   //
   // Info::putStmntDropRetn
   //
   void Info::putStmntDropRetn(Core::FastU retn, Core::FastU retnMax)
   {
      if(retn > retnMax) for(Core::FastU i = retn - retnMax; i--;)
      {
         putCode(Code::Push_Lit, ~i);
         putCode(Code::Swap);
         putCode(Code::Drop_GblArr, StaArray);
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
   void Info::putStmntPushRetn(Core::FastU retn, Core::FastU retnMax)
   {
      if(retn > retnMax) for(Core::FastU i = 0; i != retn - retnMax; ++i)
      {
         putCode(Code::Push_Lit, ~i);
         putCode(Code::Push_GblArr, StaArray);
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

   //
   // Info::putStmntStkBin
   //
   void Info::putStmntStkBin(Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1 && code != Code::Nop)
         return putCode(code);

      putStmntCall(getCallName(stmnt->code, n), stmnt->args[0].getSize());
   }

   //
   // Info::putStmntStkCmp
   //
   void Info::putStmntStkCmp(int res0, Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Push_Lit, res0);

      if(n == 1 && code != Code::Nop)
         return putCode(code);

      putStmntCall(getCallName(stmnt->code, n), stmnt->args[0].getSize());
   }
}

// EOF

