//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2022 David Hill
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

#include "IR/Exception.hpp"
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
      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop: putCode(Code::Nop); break;

      case IR::CodeBase::AddX:     putStmnt_AddX(); break;

      case IR::CodeBase::Add:      putStmnt_Add(); break;
      case IR::CodeBase::BAnd:     putStmnt_BAnd(); break;
      case IR::CodeBase::BNot:     putStmnt_BNot(); break;
      case IR::CodeBase::BOrI:     putStmnt_BOrI(); break;
      case IR::CodeBase::BOrX:     putStmnt_BOrX(); break;
      case IR::CodeBase::Bclo:     putStmnt_Bclo(); break;
      case IR::CodeBase::Bclz:     putStmnt_Bclz(); break;
      case IR::CodeBase::Bges:     putStmnt_Bges(); break;
      case IR::CodeBase::Bget:     putStmnt_Bget(); break;
      case IR::CodeBase::Bset:     putStmnt_Bset(); break;
      case IR::CodeBase::Call:     putStmnt_Call(); break;
      case IR::CodeBase::Casm:     putStmnt_Casm(); break;
      case IR::CodeBase::CmpEQ:    putStmnt_CmpEQ(); break;
      case IR::CodeBase::CmpGE:    putStmnt_CmpGE(); break;
      case IR::CodeBase::CmpGT:    putStmnt_CmpGT(); break;
      case IR::CodeBase::CmpLE:    putStmnt_CmpLE(); break;
      case IR::CodeBase::CmpLT:    putStmnt_CmpLT(); break;
      case IR::CodeBase::CmpNE:    putStmnt_CmpNE(); break;
      case IR::CodeBase::Cnat:     putStmnt_Cnat(); break;
      case IR::CodeBase::Copy:     putStmnt_Copy(); break;
      case IR::CodeBase::Cscr_IA:  putStmnt_Cscr_IA(); break;
      case IR::CodeBase::Cscr_IS:  putStmnt_Cscr_IS(); break;
      case IR::CodeBase::Cscr_SA:  putStmnt_Cscr_SA(); break;
      case IR::CodeBase::Cscr_SS:  putStmnt_Cscr_SS(); break;
      case IR::CodeBase::Cspe:     putStmnt_Cspe(); break;
      case IR::CodeBase::Div:      putStmnt_Div(); break;
      case IR::CodeBase::DivX:     putStmnt_DivX(); break;
      case IR::CodeBase::Jcnd_Nil: putStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: putStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: putStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jdyn:     putStmnt_Jdyn(); break;
      case IR::CodeBase::Jfar_Pro: putStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: putStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: putStmnt_Jfar_Sta(); break;
      case IR::CodeBase::Jump:     putStmnt_Jump(); break;
      case IR::CodeBase::LAnd:     putStmnt_LAnd(); break;
      case IR::CodeBase::LNot:     putStmnt_LNot(); break;
      case IR::CodeBase::LOrI:     putStmnt_LOrI(); break;
      case IR::CodeBase::Mod:      putStmnt_Mod(); break;
      case IR::CodeBase::Move:     putStmnt_Move(); break;
      case IR::CodeBase::Mul:      putStmnt_Mul(); break;
      case IR::CodeBase::MulX:     putStmnt_MulX(); break;
      case IR::CodeBase::Neg:      putStmnt_Neg(); break;
      case IR::CodeBase::Pltn:     putStmnt_Pltn(); break;
      case IR::CodeBase::Retn:     putStmnt_Retn(); break;
      case IR::CodeBase::Rjnk:     putStmnt_Rjnk(); break;
      case IR::CodeBase::ShL:      putStmnt_ShL(); break;
      case IR::CodeBase::ShR:      putStmnt_ShR(); break;
      case IR::CodeBase::Sub:      putStmnt_Sub(); break;
      case IR::CodeBase::SubX:     putStmnt_SubX(); break;
      case IR::CodeBase::Swap:     putStmnt_Swap(); break;
      case IR::CodeBase::Tr:       putStmnt_Tr(); break;
      case IR::CodeBase::Xcod_SID: putStmnt_Xcod_SID(); break;

      default:
         IR::ErrorCode(stmnt, "unsupported put");
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
         auto wOff = a.off + w;
         auto wVal = getWord(a.value->pos, val, wOff);

         switch(getWordType(val.getType(), wOff))
         {
         case IR::ValueBase::Funct: putStmntPushFunct(wVal); break;
         case IR::ValueBase::StrEn: putStmntPushStrEn(wVal); break;
         default: putCode(Code::Push_Lit, wVal); break;
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
   void Info::putStmntStkBin(IR::CodeType type, Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1 && code != Code::Nop)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code.base+type, n), stmnt->args[0].getSize());
   }

   //
   // Info::putStmntStkCmp
   //
   void Info::putStmntStkCmp(int res0, IR::CodeType type, Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Push_Lit, res0);

      if(n == 1 && code != Code::Nop)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code.base+type, n), stmnt->args[0].getSize());
   }
}

// EOF

