//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS target information and handling class.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__ZDACS__Info_H__
#define GDCC__BC__ZDACS__Info_H__

#include "../../BC/ZDACS/Types.hpp"

#include "../../BC/ZDACS/Code.hpp"

#include "../../BC/Info.hpp"

#include "../../Core/Array.hpp"
#include "../../Core/Counter.hpp"
#include "../../Core/Number.hpp"
#include "../../Core/NumberAlloc.hpp"

#include "../../IR/Code.hpp"

#include "../../Target/Addr.hpp"
#include "../../Target/CallType.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info
   //
   class Info : public InfoBase
   {
   public:
      Info();


      static IR::Type_Fixed const TypeWord;

      static Core::FastU FarJumpIndex;

      static bool         InitDelay;
      static Core::String InitScriptName;
      static bool         InitScriptNamed;
      static Core::FastU  InitScriptNumber;

      static Core::FastU StaArray;

      static bool UseChunkSTRE;
      static bool UseFakeACS0;

   protected:
      //
      // InitTag
      //
      enum class InitTag
      {
         Empty,
         Fixed,
         Funct,
         StrEn,
      };

      //
      // InitVal
      //
      class InitVal
      {
      public:
         InitVal() : val{0}, tag{InitTag::Empty} {}

         Core::FastU val;
         InitTag     tag;
      };

      //
      // InitData
      //
      class InitData
      {
      public:
         InitData() : max{0}, needTag{false}, onlyNil{true}, onlyStr{true} {}

         std::unordered_map<Core::FastU, InitVal> vals;

         Core::FastU max;

         bool needTag : 1;
         bool onlyNil : 1;
         bool onlyStr : 1;
      };


      void backGlyphDJump(Core::String glyph, Core::FastU val);
      void backGlyphFunc(Core::String glyph, Core::FastU val, IR::CallType ctype);
      void backGlyphGlyph(Core::String glyph, Core::String val);
      void backGlyphObj(Core::String glyph, Core::FastU val);
      void backGlyphStrEnt(Core::String glyph, Core::FastU val);
      void backGlyphWord(Core::String glyph, Core::FastU val);

      virtual void chkStmnt();

      void chkStmnt_AdXU();
      void chkStmnt_Bges() {chkStmnt_Bget();}
      void chkStmnt_Bget();
      void chkStmnt_Bset() {chkStmnt_Bget();}
      void chkStmnt_Call();
      void chkStmnt_Casm();
      void chkStmnt_Cnat();
      void chkStmnt_Copy();
      void chkStmnt_Cscr_IA();
      void chkStmnt_Cscr_IS();
      void chkStmnt_Cscr_SA() {chkStmnt_Cscr_IA();}
      void chkStmnt_Cscr_SS() {chkStmnt_Cscr_IS();}
      void chkStmnt_Cspe();
      void chkStmnt_Jcnd_Nil();
      void chkStmnt_Jcnd_Tab();
      void chkStmnt_Jcnd_Tru() {chkStmnt_Jcnd_Nil();}
      void chkStmnt_Jfar_Pro();
      void chkStmnt_Jfar_Set();
      void chkStmnt_Jfar_Sta();
      void chkStmnt_Jump();
      void chkStmnt_LAnd();
      void chkStmnt_LNot() {chkStmnt_LAnd();}
      void chkStmnt_LOrI() {chkStmnt_LAnd();}
      void chkStmnt_Swap();
      void chkStmnt_SuXU() {chkStmnt_AdXU();}

      virtual void chkStmntArg(IR::Arg const &arg);

      virtual void gen();

      virtual void genDJump();

      virtual void genFunc();

      void genIniti();
      void genInitiSpace(IR::Space &space);

      virtual void genObj();

      virtual void genSpace();
      void genSpaceIniti();
      void genSpaceIniti(IR::Space &space);
      void genSpaceInitiValue(InitData &ini, Core::FastU &itr, IR::Value const &val);

      virtual void genStmnt();
      void genStmnt_AdXU();
      void genStmnt_AddF() {genStmntStkBin();}
      void genStmnt_AddI() {genStmnt_AddU();}
      void genStmnt_AddU();
      void genStmnt_BAnd();
      void genStmnt_BNot();
      void genStmnt_BOrI() {genStmnt_BAnd();}
      void genStmnt_BOrX() {genStmnt_BAnd();}
      void genStmnt_Bclo() {genStmnt_Bclz();}
      void genStmnt_Bclz();
      void genStmnt_Bges();
      void genStmnt_Bget();
      void genStmnt_Bset();
      void genStmnt_Call();
      void genStmnt_Casm();
      void genStmnt_CmpF_EQ() {genStmntStkCmp();}
      void genStmnt_CmpF_GE() {genStmntStkCmp();}
      void genStmnt_CmpF_GT() {genStmntStkCmp();}
      void genStmnt_CmpF_LE() {genStmntStkCmp();}
      void genStmnt_CmpF_LT() {genStmntStkCmp();}
      void genStmnt_CmpF_NE() {genStmntStkCmp();}
      void genStmnt_CmpI_EQ() {genStmnt_CmpU_EQ();}
      void genStmnt_CmpI_GE() {genStmntStkCmp(Code::CmpI_GE);}
      void genStmnt_CmpI_GT() {genStmntStkCmp(Code::CmpI_GT);}
      void genStmnt_CmpI_LE() {genStmntStkCmp(Code::CmpI_LE);}
      void genStmnt_CmpI_LT() {genStmntStkCmp(Code::CmpI_LT);}
      void genStmnt_CmpI_NE() {genStmnt_CmpU_NE();}
      void genStmnt_CmpU_EQ();
      void genStmnt_CmpU_GE() {genStmntStkCmp();}
      void genStmnt_CmpU_GT() {genStmntStkCmp();}
      void genStmnt_CmpU_LE() {genStmntStkCmp();}
      void genStmnt_CmpU_LT() {genStmntStkCmp();}
      void genStmnt_CmpU_NE() {genStmnt_CmpU_EQ();}
      void genStmnt_Copy();
      void genStmnt_Cnat();
      void genStmnt_Cscr_IA();
      void genStmnt_Cscr_IS();
      void genStmnt_Cscr_SA();
      void genStmnt_Cscr_SS();
      void genStmnt_Cscr_SS_post();
      void genStmnt_Cspe();
      void genStmnt_DiXI();
      void genStmnt_DiXU() {genStmntStkBin();}
      void genStmnt_DivF() {genStmntStkBin();}
      void genStmnt_DivI();
      void genStmnt_DivK() {genStmntStkBin();}
      void genStmnt_DivU();
      void genStmnt_DivX();
      void genStmnt_Jcnd_Nil();
      void genStmnt_Jcnd_Tab();
      void genStmnt_Jcnd_Tru();
      void genStmnt_Jdyn();
      void genStmnt_Jfar();
      void genStmnt_Jfar_Pro();
      void genStmnt_Jfar_Set();
      void genStmnt_Jfar_Sta();
      void genStmnt_Jump();
      void genStmnt_LAnd();
      void genStmnt_LNot();
      void genStmnt_LOrI() {genStmnt_LAnd();}
      void genStmnt_ModI();
      void genStmnt_ModU();
      void genStmnt_Move();
      void genStmnt_Move__Arr_Stk(IR::ArgPtr2 const &arr);
      void genStmnt_Move__Stk_Arr(IR::ArgPtr2 const &arr);
      void genStmnt_MuXU() {genStmntStkBin();}
      void genStmnt_MulF() {genStmntStkBin();}
      void genStmnt_MulI() {genStmnt_MulU();}
      void genStmnt_MulK() {genStmntStkBin();}
      void genStmnt_MulU() {genStmntStkBin(Code::MulU);}
      void genStmnt_MulX() {genStmntStkBin(Code::MulX);}
      void genStmnt_NegF();
      void genStmnt_NegI();
      void genStmnt_Pltn();
      void genStmnt_Retn();
      void genStmnt_ShLF();
      void genStmnt_ShLU();
      void genStmnt_ShRI();
      void genStmnt_ShRU();
      void genStmnt_ShRU1();
      void genStmnt_SuXU();
      void genStmnt_SubF() {genStmntStkBin();}
      void genStmnt_SubI() {genStmnt_AddU();}
      void genStmnt_SubU() {genStmnt_AddU();}
      void genStmnt_Swap();
      void genStmnt_Xcod_SID();

      void genStmntCall(Core::FastU ret);

      void genStmntDropArg(IR::Arg const &arg, Core::FastU w);
      void genStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void genStmntDropParam(Core::FastU param, Core::FastU paramMax);

      void genStmntDropRetn(Core::FastU retn, Core::FastU retnMax);

      void genStmntPushArg(IR::Arg const &arg, Core::FastU w);
      void genStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void genStmntPushRetn(Core::FastU retn, Core::FastU retnMax);

      void genStmntStkBin(Code code = Code::Nop);
      void genStmntStkCmp(Code code = Code::Nop);

      virtual void genStrEnt();

      Core::NumberAllocMerge<Core::FastU> &getAllocDJump();
      Core::NumberAllocMerge<Core::FastU> &getAllocFunc(IR::CallType call);
      Core::NumberAllocMerge<Core::FastU> &getAllocObj(IR::AddrSpace addr);
      Core::NumberAllocMerge<Core::FastU> &getAllocSpace(IR::AddrBase addr);
      Core::NumberAllocMerge<Core::FastU> &getAllocStrEnt();

      Core::String getCallName(IR::Code code, Core::FastU n);

      Core::FastU getInitGblArray();
      Core::FastU getInitGblIndex();
      Core::FastU getInitHubArray();
      Core::FastU getInitHubIndex();

      Core::FastU getSpaceInitiSize(IR::Type const &type);

      Core::FastU getStkPtrIdx();

      bool isCopyArg(IR::Arg const &arg);

      bool isDropArg(IR::Arg const &arg);

      bool isFastArg(IR::Arg const &arg);

      bool isFuncJfar_Set(IR::Function const *func);

      bool isInitiGblArr();
      bool isInitiHubArr();

      bool isPushArg(IR::Arg const &arg);

      Core::FastU lenChunk();
      Core::FastU lenChunk(char const *name, Core::Array<Core::String> const &strs, bool junk);
      Core::FastU lenChunkAIMP();
      Core::FastU lenChunkAINI();
      Core::FastU lenChunkARAY();
      Core::FastU lenChunkASTR();
      Core::FastU lenChunkATAG();
      Core::FastU lenChunkCODE();
      Core::FastU lenChunkFARY();
      Core::FastU lenChunkFNAM();
      Core::FastU lenChunkFUNC();
      Core::FastU lenChunkJUMP();
      Core::FastU lenChunkLOAD();
      Core::FastU lenChunkMEXP();
      Core::FastU lenChunkMIMP();
      Core::FastU lenChunkMINI();
      Core::FastU lenChunkMSTR();
      Core::FastU lenChunkSARY();
      Core::FastU lenChunkSFLG();
      Core::FastU lenChunkSNAM();
      Core::FastU lenChunkSPTR();
      Core::FastU lenChunkSTRL();
      Core::FastU lenChunkSVCT();

      Core::FastU lenDropArg(IR::Arg const &arg, Core::FastU w);
      Core::FastU lenDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);
      Core::FastU lenDropTmp(Core::FastU w);

      Core::FastU lenIncUArg(IR::Arg const &arg, Core::FastU w);
      Core::FastU lenIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      Core::FastU lenPushArg(IR::Arg const &arg, Core::FastU w);
      Core::FastU lenPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);
      Core::FastU lenPushIdx(IR::Arg const &arg, Core::FastU w);
      Core::FastU lenPushTmp(Core::FastU w);

      std::size_t lenString(Core::String str);

      virtual void preFunc();

      virtual void preObj();

      virtual void preStmnt();
      void preStmnt_AdXU();
      void preStmnt_AddF();
      void preStmnt_AddI() {preStmnt_AddU();}
      void preStmnt_AddU();
      void preStmnt_Bclo() {preStmnt_Bclz(true);}
      void preStmnt_Bclz(bool ones = false);
      void preStmnt_CmpF_EQ();
      void preStmnt_CmpF_GE();
      void preStmnt_CmpF_GT();
      void preStmnt_CmpF_LE();
      void preStmnt_CmpF_LT();
      void preStmnt_CmpF_NE();
      void preStmnt_CmpI_EQ() {preStmnt_CmpU_EQ(IR::Code::CmpU_EQ);}
      void preStmnt_CmpI_GE();
      void preStmnt_CmpI_GT();
      void preStmnt_CmpI_LE();
      void preStmnt_CmpI_LT();
      void preStmnt_CmpI_NE() {preStmnt_CmpU_EQ(IR::Code::CmpU_NE);}
      void preStmnt_CmpU_EQ(IR::Code codeCmp = IR::Code::CmpU_EQ);
      void preStmnt_CmpU_GE();
      void preStmnt_CmpU_GT();
      void preStmnt_CmpU_LE();
      void preStmnt_CmpU_LT();
      void preStmnt_CmpU_NE() {preStmnt_CmpU_EQ(IR::Code::CmpU_NE);}
      void preStmnt_CmpU1(IR::Code code, IR::Code codeCmp, bool resGT, bool resLT);
      void preStmnt_DiXI();
      void preStmnt_DiXU();
      void preStmnt_DiXU1();
      void preStmnt_DivF();
      void preStmnt_DivI() {preStmnt_DiXI();}
      void preStmnt_DivK() {preStmnt_DivX(IR::Code::DiXU);}
      void preStmnt_DivU() {preStmnt_DiXU();}
      void preStmnt_DivX(IR::Code code = IR::Code::DiXI);
      void preStmnt_ModI() {preStmnt_DiXI();}
      void preStmnt_ModU() {preStmnt_DiXU();}
      void preStmnt_MuXU();
      void preStmnt_MuXU1();
      void preStmnt_MulF();
      void preStmnt_MulI() {preStmnt_MulU();}
      void preStmnt_MulK();
      void preStmnt_MulU();
      void preStmnt_MulX();
      void preStmnt_Retn();
      void preStmnt_ShLF();
      void preStmnt_ShLU();
      void preStmnt_ShRF() {preStmnt_ShLF();}
      void preStmnt_ShRI() {preStmnt_ShLU();}
      void preStmnt_ShRU() {preStmnt_ShLU();}
      void preStmnt_SuXU();
      void preStmnt_SubF();
      void preStmnt_SubI() {preStmnt_SubU();}
      void preStmnt_SubU();

      void preStmntCall(Core::FastU retrn, Core::FastU param);
      void preStmntCall(Core::String name, Core::FastU retrn, Core::FastU param);

      IR::Function *preStmntCallDef(Core::String name, Core::FastU retrn,
         Core::FastU param, Core::FastU localReg, char const *file,
         std::size_t line);

      virtual void preStrEnt();

      virtual void put();

      virtual void putExtra();

      void putByte(Core::FastU i);

      void putChunk();
      void putChunk(char const *name, Core::Array<Core::String> const &strs, bool junk);
      void putChunkAIMP();
      void putChunkAINI();
      void putChunkARAY();
      void putChunkASTR();
      void putChunkATAG();
      void putChunkCODE();
      void putChunkFARY();
      void putChunkFNAM();
      void putChunkFUNC();
      void putChunkJUMP();
      void putChunkLOAD();
      void putChunkMEXP();
      void putChunkMIMP();
      void putChunkMINI();
      void putChunkMSTR();
      void putChunkSARY();
      void putChunkSFLG();
      void putChunkSNAM();
      void putChunkSPTR();
      void putChunkSTRL();
      void putChunkSVCT();

      void putCode(Code code);
      void putCode(Code code, Core::FastU arg0);
      void putCode(Code code, Core::FastU arg0, Core::FastU);

      using InfoBase::putFunc;
      virtual void putFunc();

      void putHWord(Core::FastU i);

      void putIniti();
      void putInitiSpace(IR::Space &space, Code code);

      virtual void putStmnt();
      void putStmnt_AdXU();
      void putStmnt_AddF() {putStmntStkBin();}
      void putStmnt_AddI() {putStmnt_AddU();}
      void putStmnt_AddU();
      void putStmnt_BAnd(Code code = Code::BAnd);
      void putStmnt_BNot();
      void putStmnt_BOrI() {putStmnt_BAnd(Code::BOrI);}
      void putStmnt_BOrX() {putStmnt_BAnd(Code::BOrX);}
      void putStmnt_Bclo() {putStmnt_Bclz(true);}
      void putStmnt_Bclz(bool ones = false);
      void putStmnt_Bges();
      void putStmnt_Bget();
      void putStmnt_Bset();
      void putStmnt_Call();
      void putStmnt_Casm();
      void putStmnt_CmpF_EQ() {putStmntStkCmp(1);}
      void putStmnt_CmpF_GE() {putStmntStkCmp(1);}
      void putStmnt_CmpF_GT() {putStmntStkCmp(0);}
      void putStmnt_CmpF_LE() {putStmntStkCmp(1);}
      void putStmnt_CmpF_LT() {putStmntStkCmp(0);}
      void putStmnt_CmpF_NE() {putStmntStkCmp(0);}
      void putStmnt_CmpI_EQ() {putStmnt_CmpU_EQ();}
      void putStmnt_CmpI_GE() {putStmntStkCmp(1, Code::CmpI_GE);}
      void putStmnt_CmpI_GT() {putStmntStkCmp(0, Code::CmpI_GT);}
      void putStmnt_CmpI_LE() {putStmntStkCmp(1, Code::CmpI_LE);}
      void putStmnt_CmpI_LT() {putStmntStkCmp(0, Code::CmpI_LT);}
      void putStmnt_CmpI_NE() {putStmnt_CmpU_NE();}
      void putStmnt_CmpU_EQ();
      void putStmnt_CmpU_GE() {putStmntStkCmp(1);}
      void putStmnt_CmpU_GT() {putStmntStkCmp(0);}
      void putStmnt_CmpU_LE() {putStmntStkCmp(1);}
      void putStmnt_CmpU_LT() {putStmntStkCmp(0);}
      void putStmnt_CmpU_NE();
      void putStmnt_Cnat();
      void putStmnt_Copy();
      void putStmnt_Cscr_IA();
      void putStmnt_Cscr_IS();
      void putStmnt_Cscr_SA();
      void putStmnt_Cscr_SS();
      void putStmnt_Cscr_SS_post();
      void putStmnt_Cspe();
      void putStmnt_DiXI();
      void putStmnt_DiXU() {putStmntStkBin();}
      void putStmnt_DivF() {putStmntStkBin();}
      void putStmnt_DivI();
      void putStmnt_DivK() {putStmntStkBin();}
      void putStmnt_DivU();
      void putStmnt_DivX();
      void putStmnt_Jcnd_Nil();
      void putStmnt_Jcnd_Tab();
      void putStmnt_Jcnd_Tru();
      void putStmnt_Jdyn();
      void putStmnt_Jfar();
      void putStmnt_Jfar_Pro();
      void putStmnt_Jfar_Set();
      void putStmnt_Jfar_Sta();
      void putStmnt_Jump();
      void putStmnt_LAnd(Code code = Code::LAnd);
      void putStmnt_LNot();
      void putStmnt_LOrI() {putStmnt_LAnd(Code::LOrI);}
      void putStmnt_ModI();
      void putStmnt_ModU();
      void putStmnt_Move();
      void putStmnt_Move__Arr_Stk(IR::ArgPtr2 const &arr, Code code);
      void putStmnt_Move__Stk_Arr(IR::ArgPtr2 const &arr, Code code);
      void putStmnt_MuXU() {putStmntStkBin();}
      void putStmnt_MulF() {putStmntStkBin();}
      void putStmnt_MulI() {putStmnt_MulU();}
      void putStmnt_MulK() {putStmntStkBin();}
      void putStmnt_MulU() {putStmntStkBin(Code::MulU);}
      void putStmnt_MulX() {putStmntStkBin(Code::MulX);}
      void putStmnt_NegF();
      void putStmnt_NegI();
      void putStmnt_Pltn();
      void putStmnt_Retn();
      void putStmnt_ShLF();
      void putStmnt_ShLU();
      void putStmnt_ShRF() {putStmnt_ShLF();}
      void putStmnt_ShRI();
      void putStmnt_ShRU();
      void putStmnt_ShRU1();
      void putStmnt_SuXU();
      void putStmnt_SubF() {putStmntStkBin();}
      void putStmnt_SubI() {putStmnt_SubU();}
      void putStmnt_SubU();
      void putStmnt_Swap();
      void putStmnt_Xcod_SID();

      void putStmntCall(Core::String name, Core::FastU ret);

      void putStmntDropArg(IR::Arg const &arg, Core::FastU w);
      void putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntDropParam(Core::FastU param, Core::FastU paramMax);

      void putStmntDropRetn(Core::FastU retn, Core::FastU retnMax);

      void putStmntDropTmp(Core::FastU w);

      void putStmntIncUArg(IR::Arg const &arg, Core::FastU w);
      void putStmntIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntPushArg(IR::Arg const &arg, Core::FastU w);
      void putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntPushFunct(Core::FastU value);

      void putStmntPushIdx(IR::Arg const &arg, Core::FastU w);

      void putStmntPushRetn(Core::FastU retn, Core::FastU retnMax);

      void putStmntPushStrEn(Core::FastU value);

      void putStmntPushTmp(Core::FastU w);

      void putStmntShiftRU(Core::FastU shift);

      void putStmntStkBin(Code code = Code::Nop);
      void putStmntStkCmp(int res0, Code code = Code::Nop);

      void putString(Core::String str);
      void putString(Core::String str, Core::FastU key);

      void putWord(Core::FastU i);

      Core::CounterRef<IR::Exp const> resolveGlyph(Core::String glyph);

      virtual void trFunc();

      virtual void trStmnt();
      void trStmnt_AdXU();
      void trStmnt_AddF() {trStmntStkBin(false);}
      void trStmnt_AddI() {trStmnt_AddU();}
      void trStmnt_AddU();
      void trStmnt_BAnd();
      void trStmnt_BNot();
      void trStmnt_BOrI() {trStmnt_BAnd();}
      void trStmnt_BOrX() {trStmnt_BAnd();}
      void trStmnt_Bclz();
      void trStmnt_Bges() {trStmnt_Bget();}
      void trStmnt_Bget();
      void trStmnt_Bset();
      void trStmnt_Call();
      void trStmnt_Casm() {trStmnt_Call();}
      void trStmnt_CmpF_EQ() {trStmntStkCmp(false);}
      void trStmnt_CmpF_GE() {trStmntStkCmp(true);}
      void trStmnt_CmpF_GT() {trStmntStkCmp(true);}
      void trStmnt_CmpF_LE() {trStmntStkCmp(true);}
      void trStmnt_CmpF_LT() {trStmntStkCmp(true);}
      void trStmnt_CmpF_NE() {trStmntStkCmp(false);}
      void trStmnt_CmpI_EQ() {trStmnt_CmpU_EQ();}
      void trStmnt_CmpI_GE() {trStmntStkCmp(true);}
      void trStmnt_CmpI_GT() {trStmntStkCmp(true);}
      void trStmnt_CmpI_LE() {trStmntStkCmp(true);}
      void trStmnt_CmpI_LT() {trStmntStkCmp(true);}
      void trStmnt_CmpI_NE() {trStmnt_CmpU_EQ();}
      void trStmnt_CmpU_EQ();
      void trStmnt_CmpU_GE() {trStmntStkCmp(true);}
      void trStmnt_CmpU_GT() {trStmntStkCmp(true);}
      void trStmnt_CmpU_LE() {trStmntStkCmp(true);}
      void trStmnt_CmpU_LT() {trStmntStkCmp(true);}
      void trStmnt_CmpU_NE() {trStmnt_CmpU_EQ();}
      void trStmnt_Cnat() {trStmnt_Call();}
      void trStmnt_Copy() {}
      void trStmnt_Cscr_IA() {trStmnt_Call();}
      void trStmnt_Cscr_IS() {trStmnt_Call();}
      void trStmnt_Cscr_SA() {trStmnt_Call();}
      void trStmnt_Cscr_SS() {trStmnt_Call();}
      void trStmnt_Cspe();
      void trStmnt_DiXI();
      void trStmnt_DiXU();
      void trStmnt_DivF() {trStmntStkBin(true);}
      void trStmnt_DivI() {trStmntStkBin(true);}
      void trStmnt_DivK() {trStmntStkBin(true);}
      void trStmnt_DivU() {trStmntStkBin(true);}
      void trStmnt_DivX() {trStmntStkBin(true);}
      void trStmnt_Jcnd_Nil();
      void trStmnt_Jcnd_Tab() {}
      void trStmnt_Jcnd_Tru() {trStmnt_Jcnd_Nil();}
      void trStmnt_Jdyn();
      void trStmnt_Jfar_Pro();
      void trStmnt_Jfar_Set() {}
      void trStmnt_Jfar_Sta() {}
      void trStmnt_Jump() {}
      void trStmnt_LAnd();
      void trStmnt_LNot();
      void trStmnt_LOrI() {trStmnt_LAnd();}
      void trStmnt_ModI();
      void trStmnt_ModU();
      void trStmnt_Move();
      void trStmnt_MuXU() {trStmntStkBin(false);}
      void trStmnt_MulF() {trStmntStkBin(false);}
      void trStmnt_MulI() {trStmntStkBin(false);}
      void trStmnt_MulK() {trStmntStkBin(false);}
      void trStmnt_MulU() {trStmntStkBin(false);}
      void trStmnt_MulX() {trStmntStkBin(false);}
      void trStmnt_NegF() {trStmntStkUna();}
      void trStmnt_NegI();
      void trStmnt_Pltn() {trStmntStkUna();}
      void trStmnt_Retn();
      void trStmnt_ShLF();
      void trStmnt_ShLU();
      void trStmnt_ShRF() {trStmnt_ShLF();}
      void trStmnt_ShRI();
      void trStmnt_ShRU();
      void trStmnt_ShRU1();
      void trStmnt_SuXU();
      void trStmnt_SubF() {trStmntStkBin(true);}
      void trStmnt_SubI() {trStmnt_SubU();}
      void trStmnt_SubU();
      void trStmnt_Swap();

      void trStmntStkBin(bool ordered);
      void trStmntStkCmp(bool ordered);
      void trStmntStkUna();

      void trStmntTmp(Core::FastU n);

      std::unique_ptr<Core::NumberAllocMerge<Core::FastU>> allocDJump;
      std::unordered_map<IR::CallType, Core::NumberAllocMerge<Core::FastU>> allocFunc;
      std::unordered_map<IR::AddrSpace, Core::NumberAllocMerge<Core::FastU>> allocObj;
      std::unordered_map<IR::AddrBase, Core::NumberAllocMerge<Core::FastU>> allocSpace;
      std::unique_ptr<Core::NumberAllocMerge<Core::FastU>> allocStrEnt;

      Core::FastU codeInit;
      Core::FastU codeInitEnd;

      std::unordered_map<IR::Function const *, bool> funcJfar_Set;

      std::unordered_map<IR::Space const *, InitData> init;

      Core::FastU numChunkAIMP;
      Core::FastU numChunkAINI;
      Core::FastU numChunkARAY;
      Core::FastU numChunkASTR;
      Core::FastU numChunkATAG;
      Core::FastU numChunkCODE;
      Core::FastU numChunkFNAM;
      Core::FastU numChunkFUNC;
      Core::FastU numChunkJUMP;
      Core::FastU numChunkLOAD;
      Core::FastU numChunkMEXP;
      Core::FastU numChunkMIMP;
      Core::FastU numChunkMINI;
      Core::FastU numChunkMSTR;
      Core::FastU numChunkSFLG;
      Core::FastU numChunkSNAM;
      Core::FastU numChunkSPTR;
      Core::FastU numChunkSTRL;
      Core::FastU numChunkSVCT;

      std::unordered_map<IR::Space const *, bool> spaceUsed;


      static Core::FastU CodeBase();

      static Core::FastU GetParamMax(IR::CallType call);

      static Core::FastU GetRetnMax(IR::CallType call);

      static Core::FastU GetScriptValue(IR::Function const &script);

      static bool IsNull(IR::Value_Funct const &val);
      static bool IsNull(IR::Value_StrEn const &val);

      static bool IsNull_Funct(Core::FastU val);
      static bool IsNull_StrEn(Core::FastU val);

      static bool IsScript(IR::CallType ctype);
      static bool IsScriptS(IR::CallType ctype);
   };
}

#endif//GDCC__BC__ZDACS__Info_H__

