//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement glyph generation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Module.hpp"

#include "IR/Exception.hpp"
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
      for(auto const &lab : stmnt->labs)
         backGlyphLabel(lab);

      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop: genCode(Code::Nop); break;

      case IR::CodeBase::Add:      genStmnt_Add(); break;
      case IR::CodeBase::AddX:     genStmnt_AddX(); break;
      case IR::CodeBase::BAnd:     genStmnt_BAnd(); break;
      case IR::CodeBase::BNot:     genStmnt_BNot(); break;
      case IR::CodeBase::BOrI:     genStmnt_BOrI(); break;
      case IR::CodeBase::BOrX:     genStmnt_BOrX(); break;
      case IR::CodeBase::Bclo:     genStmnt_Bclo(); break;
      case IR::CodeBase::Bclz:     genStmnt_Bclz(); break;
      case IR::CodeBase::Bges:     genStmnt_Bges(); break;
      case IR::CodeBase::Bget:     genStmnt_Bget(); break;
      case IR::CodeBase::Bset:     genStmnt_Bset(); break;
      case IR::CodeBase::Call:     genStmnt_Call(); break;
      case IR::CodeBase::Casm:     genStmnt_Casm(); break;
      case IR::CodeBase::CmpEQ:    genStmnt_CmpEQ(); break;
      case IR::CodeBase::CmpGE:    genStmnt_CmpGE(); break;
      case IR::CodeBase::CmpGT:    genStmnt_CmpGT(); break;
      case IR::CodeBase::CmpLE:    genStmnt_CmpLE(); break;
      case IR::CodeBase::CmpLT:    genStmnt_CmpLT(); break;
      case IR::CodeBase::CmpNE:    genStmnt_CmpNE(); break;
      case IR::CodeBase::Copy:     genStmnt_Copy(); break;
      case IR::CodeBase::Cnat:     genStmnt_Cnat(); break;
      case IR::CodeBase::Cscr_IA:  genStmnt_Cscr_IA(); break;
      case IR::CodeBase::Cscr_IS:  genStmnt_Cscr_IS(); break;
      case IR::CodeBase::Cscr_SA:  genStmnt_Cscr_SA(); break;
      case IR::CodeBase::Cscr_SS:  genStmnt_Cscr_SS(); break;
      case IR::CodeBase::Cspe:     genStmnt_Cspe(); break;
      case IR::CodeBase::Div:      genStmnt_Div(); break;
      case IR::CodeBase::DivX:     genStmnt_DivX(); break;
      case IR::CodeBase::Jcnd_Nil: genStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: genStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: genStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jdyn:     genStmnt_Jdyn(); break;
      case IR::CodeBase::Jfar_Pro: genStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: genStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: genStmnt_Jfar_Sta(); break;
      case IR::CodeBase::Jump:     genStmnt_Jump(); break;
      case IR::CodeBase::LAnd:     genStmnt_LAnd(); break;
      case IR::CodeBase::LNot:     genStmnt_LNot(); break;
      case IR::CodeBase::LOrI:     genStmnt_LOrI(); break;
      case IR::CodeBase::Mod:      genStmnt_Mod(); break;
      case IR::CodeBase::Move:     genStmnt_Move(); break;
      case IR::CodeBase::Mul:      genStmnt_Mul(); break;
      case IR::CodeBase::MulX:     genStmnt_MulX(); break;
      case IR::CodeBase::Neg:      genStmnt_Neg(); break;
      case IR::CodeBase::Pltn:     genStmnt_Pltn(); break;
      case IR::CodeBase::Retn:     genStmnt_Retn(); break;
      case IR::CodeBase::Rjnk:     genStmnt_Rjnk(); break;
      case IR::CodeBase::ShL:      genStmnt_ShL(); break;
      case IR::CodeBase::ShR:      genStmnt_ShR(); break;
      case IR::CodeBase::Sub:      genStmnt_Sub(); break;
      case IR::CodeBase::SubX:     genStmnt_SubX(); break;
      case IR::CodeBase::Swap:     genStmnt_Swap(); break;
      case IR::CodeBase::Tr:       genStmnt_Tr(); break;
      case IR::CodeBase::Xcod_SID: genStmnt_Xcod_SID(); break;

      default:
         IR::ErrorCode(stmnt, "unsupported gen");
      }
   }

   //
   // Info::genStmnt_Pltn
   //
   void Info::genStmnt_Pltn()
   {
      genCode(Code::Push_LocReg, getStkPtrIdx());
      genCode(Code::AddU);
   }

   //
   // Info::genStmnt_Xcod_SID
   //
   void Info::genStmnt_Xcod_SID()
   {
      if(isInitScriptEvent())
         return;

      if(isInitiHubArr())
      {
         genCode(Code::Push_Lit,    getInitHubIndex());
         genCode(Code::Push_HubArr, getInitHubArray());
      }
      else if(isInitiGblArr())
      {
         genCode(Code::Push_Lit,    getInitGblIndex());
         genCode(Code::Push_GblArr, getInitGblArray());
      }
      else
         return;

      auto jump = module->chunkCODE.size();
      genCode(Code::Jcnd_Tru, 0);
      genCode(Code::Wait_Lit, 1);
      module->chunkCODE[jump].args[0] = getCodePos();
   }

   //
   // Info::genStmntCall
   //
   void Info::genStmntCall(Core::String name, Core::FastU retn)
   {
      genCode(retn ? Code::Call_Lit : Code::Call_Nul, getExpGlyph(name));
      genStmntPushRetn(retn, GetRetnMax(IR::CallType::StkCall));
   }

   //
   // Info::genStmntDecUTmp
   //
   void Info::genStmntDecUTmp(Core::FastU w)
   {
      genCode(Code::DecU_LocReg, func->localReg + w);
   }

   //
   // Info::genStmntDropArg
   //
   void Info::genStmntDropArg(IR::Arg const &arg, Core::FastU w)
   {
      //
      // genArr
      //
      auto genArr = [&](IR::ArgPtr2 const &a, Code code)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            genCode(Code::Push_Lit, getExpAddPtr(a.idx->aLit.value, a.off + w));
            genCode(Code::Swap);
            genCode(code,           a.arr->aLit.value);
         }
         else
         {
            genStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               genCode(Code::Push_Lit, a.off + w);
               genCode(Code::AddU);
            }

            genCode(Code::Swap);
            genCode(code, a.arr->aLit.value);
         }
      };

      //
      // genAut
      //
      auto genAut = [&](IR::Arg_Aut const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            genCode(Code::Push_Lit,    getExpAddPtr(a.idx->aLit.value, a.off + w));
            genCode(Code::Push_LocReg, getStkPtrIdx());
            genCode(Code::AddU);
            genCode(Code::Swap);
            genCode(Code::Drop_GblArr, StaArray);
         }
         else
         {
            genStmntPushArg(*a.idx, 0);

            genCode(Code::Push_LocReg, getStkPtrIdx());
            genCode(Code::AddU);

            if(a.off + w)
            {
               genCode(Code::Push_Lit, a.off + w);
               genCode(Code::AddU);
            }

            genCode(Code::Swap);
            genCode(Code::Drop_GblArr, StaArray);
         }
      };

      //
      // genReg
      //
      auto genReg = [&](IR::ArgPtr1 const &a, Code code)
      {
         genCode(code, getExpAddPtr(a.idx->aLit.value, a.off + w));
      };

      //
      // genSta
      //
      auto genSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            genCode(Code::Push_Lit,    getExpAddPtr(a.idx->aLit.value, a.off + w));
            genCode(Code::Swap);
            genCode(Code::Drop_GblArr, StaArray);
         }
         else
         {
            genStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               genCode(Code::Push_Lit, a.off + w);
               genCode(Code::AddU);
            }

            genCode(Code::Swap);
            genCode(Code::Drop_GblArr, StaArray);
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Aut:    genAut(arg.aAut); break;
      case IR::ArgBase::GblArr: genArr(arg.aGblArr, Code::Drop_GblArr); break;
      case IR::ArgBase::GblReg: genReg(arg.aGblReg, Code::Drop_GblReg); break;
      case IR::ArgBase::HubArr: genArr(arg.aHubArr, Code::Drop_HubArr); break;
      case IR::ArgBase::HubReg: genReg(arg.aHubReg, Code::Drop_HubReg); break;
      case IR::ArgBase::LocArr: genArr(arg.aLocArr, Code::Drop_LocArr); break;
      case IR::ArgBase::LocReg: genReg(arg.aLocReg, Code::Drop_LocReg); break;
      case IR::ArgBase::ModArr: genArr(arg.aModArr, Code::Drop_ModArr); break;
      case IR::ArgBase::ModReg: genReg(arg.aModReg, Code::Drop_ModReg); break;
      case IR::ArgBase::Nul:    genCode(Code::Drop_Nul); break;
      case IR::ArgBase::Sta:    genSta(arg.aSta); break;

      default:
         Core::Error(stmnt->pos, "bad genStmntDropArg");
      }
   }

   //
   // Info::genStmntDropArg
   //
   void Info::genStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      while(hi-- != lo)
         genStmntDropArg(arg, hi);
   }

   //
   // Info::genStmntDropParam
   //
   void Info::genStmntDropParam(Core::FastU param, Core::FastU paramMax)
   {
      genStmntDropRetn(param, paramMax);
   }

   //
   // Info::genStmntDropRetn
   //
   void Info::genStmntDropRetn(Core::FastU retn, Core::FastU retnMax)
   {
      if(retn > retnMax) for(Core::FastU i = retn - retnMax; i--;)
      {
         genCode(Code::Push_Lit, ~i);
         genCode(Code::Swap);
         genCode(Code::Drop_GblArr, StaArray);
      }
   }

   //
   // Info::genStmntDropTmp
   //
   void Info::genStmntDropTmp(Core::FastU w)
   {
      genCode(Code::Drop_LocReg, func->localReg + w);
   }

   //
   // Info::genStmntIncUArg
   //
   void Info::genStmntIncUArg(IR::Arg const &arg, Core::FastU w)
   {
      //
      // genReg
      //
      auto genReg = [&](IR::ArgPtr1 const &a, Code code)
      {
         genCode(code, getExpAddPtr(a.idx->aLit.value, a.off + w));
      };

      switch(arg.a)
      {
      case IR::ArgBase::GblReg: genReg(arg.aGblReg, Code::IncU_GblReg); break;
      case IR::ArgBase::HubReg: genReg(arg.aHubReg, Code::IncU_HubReg); break;
      case IR::ArgBase::LocReg: genReg(arg.aLocReg, Code::IncU_LocReg); break;
      case IR::ArgBase::ModReg: genReg(arg.aModReg, Code::IncU_ModReg); break;

      default:
         Core::Error(stmnt->pos, "bad genStmntIncUArg");
      }
   }

   //
   // Info::genStmntIncUArg
   //
   void Info::genStmntIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
   {
      for(; lo != hi; ++lo)
         genStmntIncUArg(arg, lo);
   }

   //
   // Info::genStmntIncUTmp
   //
   void Info::genStmntIncUTmp(Core::FastU w)
   {
      genCode(Code::IncU_LocReg, func->localReg + w);
   }

   //
   // Info::genStmntPushArg
   //
   void Info::genStmntPushArg(IR::Arg const &arg, Core::FastU w)
   {
      //
      // genArr
      //
      auto genArr = [&](IR::ArgPtr2 const &a, Code code)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            genCode(Code::Push_Lit, getExpAddPtr(a.idx->aLit.value, a.off + w));
            genCode(code,           a.arr->aLit.value);
         }
         else
         {
            genStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               genCode(Code::Push_Lit, a.off + w);
               genCode(Code::AddU);
            }

            genCode(code, a.arr->aLit.value);
         }
      };

      //
      // genAut
      //
      auto genAut = [&](IR::Arg_Aut const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            genCode(Code::Push_Lit,    getExpAddPtr(a.idx->aLit.value, a.off + w));
            genCode(Code::Push_LocReg, getStkPtrIdx());
            genCode(Code::AddU);
            genCode(Code::Push_GblArr, StaArray);
         }
         else
         {
            genStmntPushArg(*a.idx, 0);

            genCode(Code::Push_LocReg, getStkPtrIdx());
            genCode(Code::AddU);

            if(a.off + w)
            {
               genCode(Code::Push_Lit, a.off + w);
               genCode(Code::AddU);
            }

            genCode(Code::Push_GblArr, StaArray);
         }
      };

      //
      // genLit
      //
      auto genLit = [&](IR::Arg_Lit const &a)
      {
         auto wOff = a.off + w;

         switch(getWordType(a.value->getType(), wOff))
         {
         case IR::ValueBase::Funct: genStmntPushFunct(a.value, wOff); break;
         case IR::ValueBase::StrEn: genStmntPushStrEn(a.value, wOff); break;
         default: genCode(Code::Push_Lit, {a.value, wOff}); break;
         }
      };

      //
      // genReg
      //
      auto genReg = [&](IR::ArgPtr1 const &a, Code code)
      {
         genCode(code, getExpAddPtr(a.idx->aLit.value, a.off + w));
      };

      //
      // genSta
      //
      auto genSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            genCode(Code::Push_Lit,    getExpAddPtr(a.idx->aLit.value, a.off + w));
            genCode(Code::Push_GblArr, StaArray);
         }
         else
         {
            genStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               genCode(Code::Push_Lit, a.off + w);
               genCode(Code::AddU);
            }

            genCode(Code::Push_GblArr, StaArray);
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Aut:    genAut(arg.aAut); break;
      case IR::ArgBase::GblArr: genArr(arg.aGblArr, Code::Push_GblArr); break;
      case IR::ArgBase::GblReg: genReg(arg.aGblReg, Code::Push_GblReg); break;
      case IR::ArgBase::HubArr: genArr(arg.aHubArr, Code::Push_HubArr); break;
      case IR::ArgBase::HubReg: genReg(arg.aHubReg, Code::Push_HubReg); break;
      case IR::ArgBase::Lit:    genLit(arg.aLit); break;
      case IR::ArgBase::LocArr: genArr(arg.aLocArr, Code::Push_LocArr); break;
      case IR::ArgBase::LocReg: genReg(arg.aLocReg, Code::Push_LocReg); break;
      case IR::ArgBase::ModArr: genArr(arg.aModArr, Code::Push_ModArr); break;
      case IR::ArgBase::ModReg: genReg(arg.aModReg, Code::Push_ModReg); break;
      case IR::ArgBase::Sta:    genSta(arg.aSta); break;

      default:
         Core::Error(stmnt->pos, "bad genStmntPushArg ", arg.a);
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

   //
   // Info::genStmntPushFunct
   //
   void Info::genStmntPushFunct(Core::FastU value)
   {
      genCode(IsNull_Funct(value) ? Code::Push_Lit : Code::Pfun_Lit, value);
   }

   //
   // Info::genStmntPushFunct
   //
   void Info::genStmntPushFunct(IR::Exp const *value, Core::FastU w)
   {
      genCode(isNull_Funct(value, w) ? Code::Push_Lit : Code::Pfun_Lit, {value, w});
   }

   //
   // Info::genStmntPushIdx
   //
   void Info::genStmntPushIdx(IR::Arg const &arg, Core::FastU w)
   {
      //
      // genSta
      //
      auto genSta = [&](IR::Arg_Sta const &a)
      {
         if(a.idx->a == IR::ArgBase::Lit)
         {
            genCode(Code::Push_Lit, getExpAddPtr(a.idx->aLit.value, a.off + w));
         }
         else
         {
            genStmntPushArg(*a.idx, 0);

            if(a.off + w)
            {
               genCode(Code::Push_Lit, a.off + w);
               genCode(Code::AddU);
            }
         }
      };

      switch(arg.a)
      {
      case IR::ArgBase::Sta: genSta(arg.aSta); break;

      default:
         Core::Error(stmnt->pos, "bad genStmntPushIdx");
      }
   }

   //
   // Info::genStmntPushRetn
   //
   void Info::genStmntPushRetn(Core::FastU retn, Core::FastU retnMax)
   {
      if(retn > retnMax) for(Core::FastU i = 0; i != retn - retnMax; ++i)
      {
         genCode(Code::Push_Lit,    ~i);
         genCode(Code::Push_GblArr, StaArray);
      }
   }

   //
   // Info::genStmntPushStrEn
   //
   void Info::genStmntPushStrEn(Core::FastU value)
   {
      genCode(Code::Push_Lit, value);
      if(!IsNull_StrEn(value))
         genCode(Code::Pstr_Stk);
   }

   //
   // Info::genStmntPushStrEn
   //
   void Info::genStmntPushStrEn(IR::Exp const *value, Core::FastU w)
   {
      genCode(Code::Push_Lit, {value, w});
      if(!isNull_StrEn(value, w))
         genCode(Code::Pstr_Stk);
   }

   //
   // Info::genStmntPushTmp
   //
   void Info::genStmntPushTmp(Core::FastU w)
   {
      genCode(Code::Push_LocReg, func->localReg + w);
   }

   //
   // Info::genStmntStkBin
   //
   void Info::genStmntStkBin(IR::CodeType type, Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1 && code != Code::Nop)
         return genCode(code);

      genStmntCall(getFuncName(stmnt->code.base+type, n), stmnt->args[0].getSize());
   }

   //
   // Info::genStmntStkCmp
   //
   void Info::genStmntStkCmp(Core::FastU res0, IR::CodeType type, Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return genCode(Code::Push_Lit, res0);

      if(n == 1 && code != Code::Nop)
         return genCode(code);

      genStmntCall(getFuncName(stmnt->code.base+type, n), stmnt->args[0].getSize());
   }
}

// EOF

