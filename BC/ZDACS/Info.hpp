//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2022 David Hill
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
#include "../../Core/StringOption.hpp"

#include "../../IR/Code.hpp"

#include "../../Option/Bool.hpp"
#include "../../Option/IntMap.hpp"

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
   protected:
      using ScriptTypeMap = Option::IntMap<Core::String, Core::FastU>;

   public:
      Info();


      static IR::Type_Fixed const TypeWord;

      static Core::FastU FarJumpIndex;

      static Option::Bool             InitDelay;
      static Core::StringOption       InitScriptName;
      static Option::Bool             InitScriptNamed;
      static Option::Int<Core::FastU> InitScriptNumber;

      static ScriptTypeMap ScriptFlags;
      static ScriptTypeMap ScriptTypes;

      static Option::Int<Core::FastU> StaArray;

      static Option::Bool UseChunkSTRE;
      static Option::Bool UseFakeACS0;

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


      void addFunc_Cmp_UW1(IR::CodeBase base);
      void addFunc_DivX_UW1();
      void addFunc_MulX_UW1();
      void addFunc_Bclz_W1(bool ones);

      void backGlyphDJump(Core::String glyph, Core::FastU val);
      void backGlyphFunc(Core::String glyph, Core::FastU val, IR::CallType ctype);
      void backGlyphGlyph(Core::String glyph, Core::String val);
      void backGlyphObj(Core::String glyph, Core::FastU val);
      void backGlyphStrEnt(Core::String glyph, Core::FastU val);
      void backGlyphWord(Core::String glyph, Core::FastU val);

      virtual void chkStmnt();

      void chkStmnt_AddX();
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
      void chkStmnt_Rjnk();
      void chkStmnt_Swap();
      void chkStmnt_SubX() {chkStmnt_AddX();}

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
      void genStmnt_Add();
      void genStmnt_Add_F() {genStmntStkBin();}
      void genStmnt_Add_I() {genStmnt_Add_U();}
      void genStmnt_Add_U();
      void genStmnt_AddX();
      void genStmnt_AddX_U();
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
      void genStmnt_CmpEQ();
      void genStmnt_CmpEQ_F() {genStmntStkCmp();}
      void genStmnt_CmpEQ_I() {genStmnt_CmpEQ_U();}
      void genStmnt_CmpEQ_U();
      void genStmnt_CmpGE();
      void genStmnt_CmpGE_F() {genStmntStkCmp();}
      void genStmnt_CmpGE_I() {genStmntStkCmp(Code::CmpI_GE);}
      void genStmnt_CmpGE_U() {genStmntStkCmp();}
      void genStmnt_CmpGT();
      void genStmnt_CmpGT_F() {genStmntStkCmp();}
      void genStmnt_CmpGT_I() {genStmntStkCmp(Code::CmpI_GT);}
      void genStmnt_CmpGT_U() {genStmntStkCmp();}
      void genStmnt_CmpLE();
      void genStmnt_CmpLE_F() {genStmntStkCmp();}
      void genStmnt_CmpLE_I() {genStmntStkCmp(Code::CmpI_LE);}
      void genStmnt_CmpLE_U() {genStmntStkCmp();}
      void genStmnt_CmpLT();
      void genStmnt_CmpLT_F() {genStmntStkCmp();}
      void genStmnt_CmpLT_I() {genStmntStkCmp(Code::CmpI_LT);}
      void genStmnt_CmpLT_U() {genStmntStkCmp();}
      void genStmnt_CmpNE();
      void genStmnt_CmpNE_F() {genStmntStkCmp();}
      void genStmnt_CmpNE_I() {genStmnt_CmpNE_U();}
      void genStmnt_CmpNE_U() {genStmnt_CmpEQ_U();}
      void genStmnt_Copy();
      void genStmnt_Cnat();
      void genStmnt_Cscr_IA();
      void genStmnt_Cscr_IS();
      void genStmnt_Cscr_SA();
      void genStmnt_Cscr_SS();
      void genStmnt_Cscr_SS_post();
      void genStmnt_Cspe();
      void genStmnt_Div();
      void genStmnt_Div_A() {genStmntStkBin();}
      void genStmnt_Div_F() {genStmntStkBin();}
      void genStmnt_Div_I();
      void genStmnt_Div_K() {genStmntStkBin();}
      void genStmnt_Div_R() {genStmntStkBin();}
      void genStmnt_Div_U();
      void genStmnt_Div_X();
      void genStmnt_DivX();
      void genStmnt_DivX_I();
      void genStmnt_DivX_U() {genStmntStkBin();}
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
      void genStmnt_Mod();
      void genStmnt_Mod_I();
      void genStmnt_Mod_U();
      void genStmnt_Move();
      void genStmnt_Move__Arr_Stk(IR::ArgPtr2 const &arr);
      void genStmnt_Move__Stk_Arr(IR::ArgPtr2 const &arr);
      void genStmnt_Mul();
      void genStmnt_Mul_A() {genStmntStkBin();}
      void genStmnt_Mul_F() {genStmntStkBin();}
      void genStmnt_Mul_I() {genStmnt_Mul_U();}
      void genStmnt_Mul_K() {genStmntStkBin();}
      void genStmnt_Mul_R() {genStmntStkBin();}
      void genStmnt_Mul_U() {genStmntStkBin(Code::MulU);}
      void genStmnt_Mul_X() {genStmntStkBin(Code::MulX);}
      void genStmnt_MulX();
      void genStmnt_MulX_U() {genStmntStkBin();}
      void genStmnt_Neg();
      void genStmnt_Neg_F();
      void genStmnt_Neg_I();
      void genStmnt_Neg_U() {genStmnt_Neg_I();}
      void genStmnt_Pltn();
      void genStmnt_Retn();
      void genStmnt_Rjnk();
      void genStmnt_ShL();
      void genStmnt_ShL_F();
      void genStmnt_ShL_I() {genStmnt_ShL_U();}
      void genStmnt_ShL_U();
      void genStmnt_ShR();
      void genStmnt_ShR_F() {genStmnt_ShL_F();}
      void genStmnt_ShR_I();
      void genStmnt_ShR_U();
      void genStmnt_ShR_U1();
      void genStmnt_Sub();
      void genStmnt_Sub_F() {genStmntStkBin();}
      void genStmnt_Sub_I() {genStmnt_Add_U();}
      void genStmnt_Sub_U() {genStmnt_Add_U();}
      void genStmnt_SubX();
      void genStmnt_SubX_U();
      void genStmnt_Swap();
      void genStmnt_Tr();
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

      Core::FastU getInitGblArray();
      Core::FastU getInitGblIndex();
      Core::FastU getInitHubArray();
      Core::FastU getInitHubIndex();

      Core::FastU getSpaceInitiSize(IR::Type const &type);

      Core::FastU getStkPtrIdx();

      virtual IR::TypeBase getWordType_Funct(IR::Type_Funct const &type, Core::FastU w);
      virtual IR::TypeBase getWordType_StrEn(IR::Type_StrEn const &type, Core::FastU w);

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
      void preStmnt_Add();
      void preStmnt_Add_F() {preStmntStkCmp(1, &Info::addFunc_Add_FW);}
      void preStmnt_Add_I() {preStmntStkCmp(2, &Info::addFunc_Add_UW);}
      void preStmnt_Add_U() {preStmntStkCmp(2, &Info::addFunc_Add_UW);}
      void preStmnt_AddX();
      void preStmnt_AddX_U();
      void preStmnt_Bclo() {preStmnt_Bclz(true);}
      void preStmnt_Bclz(bool ones = false);
      void preStmnt_CmpEQ();
      void preStmnt_CmpEQ_F() {preStmntStkCmp(1, &Info::addFunc_CmpEQ_FW);}
      void preStmnt_CmpEQ_I() {preStmntStkCmp(2, &Info::addFunc_CmpEQ_UW);}
      void preStmnt_CmpEQ_U() {preStmntStkCmp(2, &Info::addFunc_CmpEQ_UW);}
      void preStmnt_CmpGE();
      void preStmnt_CmpGE_F() {preStmntStkCmp(1, &Info::addFunc_CmpGE_FW);}
      void preStmnt_CmpGE_I() {preStmntStkCmp(2, &Info::addFunc_CmpGE_IW);}
      void preStmnt_CmpGE_U() {preStmntStkCmpU(&Info::addFunc_CmpGE_UW);}
      void preStmnt_CmpGT();
      void preStmnt_CmpGT_F() {preStmntStkCmp(1, &Info::addFunc_CmpGT_FW);}
      void preStmnt_CmpGT_I() {preStmntStkCmp(2, &Info::addFunc_CmpGT_IW);}
      void preStmnt_CmpGT_U() {preStmntStkCmpU(&Info::addFunc_CmpGT_UW);}
      void preStmnt_CmpLE();
      void preStmnt_CmpLE_F() {preStmntStkCmp(1, &Info::addFunc_CmpLE_FW);}
      void preStmnt_CmpLE_I() {preStmntStkCmp(2, &Info::addFunc_CmpLE_IW);}
      void preStmnt_CmpLE_U() {preStmntStkCmpU(&Info::addFunc_CmpLE_UW);}
      void preStmnt_CmpLT();
      void preStmnt_CmpLT_F() {preStmntStkCmp(1, &Info::addFunc_CmpLT_FW);}
      void preStmnt_CmpLT_I() {preStmntStkCmp(2, &Info::addFunc_CmpLT_IW);}
      void preStmnt_CmpLT_U() {preStmntStkCmpU(&Info::addFunc_CmpLT_UW);}
      void preStmnt_CmpNE();
      void preStmnt_CmpNE_F() {preStmntStkCmp(1, &Info::addFunc_CmpNE_FW);}
      void preStmnt_CmpNE_I() {preStmntStkCmp(2, &Info::addFunc_CmpNE_UW);}
      void preStmnt_CmpNE_U() {preStmntStkCmp(2, &Info::addFunc_CmpNE_UW);}
      void preStmnt_Div();
      void preStmnt_Div_A() {preStmntStkBin(1, &Info::addFunc_Div_AW);}
      void preStmnt_Div_F() {preStmntStkBin(1, &Info::addFunc_Div_FW);}
      void preStmnt_Div_I() {preStmntStkBin(2, &Info::addFunc_DivX_IW);}
      void preStmnt_Div_K() {preStmntStkBin(1, &Info::addFunc_Div_KW);}
      void preStmnt_Div_R() {preStmntStkBin(1, &Info::addFunc_Div_RW);}
      void preStmnt_Div_U() {preStmnt_DivX_U();}
      void preStmnt_Div_X() {preStmntStkBin(2, &Info::addFunc_Div_XW);}
      void preStmnt_DivX();
      void preStmnt_DivX_I() {preStmntStkBin(2, &Info::addFunc_DivX_IW);}
      void preStmnt_DivX_U();
      void preStmnt_Mod();
      void preStmnt_Mod_I() {preStmntStkBin(2, &Info::addFunc_DivX_IW);}
      void preStmnt_Mod_U() {preStmnt_DivX_U();}
      void preStmnt_Mul();
      void preStmnt_Mul_A() {preStmntStkBin(1, &Info::addFunc_Mul_AW);}
      void preStmnt_Mul_F() {preStmntStkBin(1, &Info::addFunc_Mul_FW);}
      void preStmnt_Mul_I() {preStmntStkBin(2, &Info::addFunc_Mul_UW);}
      void preStmnt_Mul_K() {preStmntStkBin(1, &Info::addFunc_Mul_KW);}
      void preStmnt_Mul_R() {preStmntStkBin(1, &Info::addFunc_Mul_RW);}
      void preStmnt_Mul_U() {preStmntStkBin(2, &Info::addFunc_Mul_UW);}
      void preStmnt_Mul_X() {preStmntStkBin(2, &Info::addFunc_Mul_XW);}
      void preStmnt_MulX();
      void preStmnt_MulX_U();
      void preStmnt_Retn();
      void preStmnt_Rjnk() {preStmnt_Retn();}
      void preStmnt_ShL();
      void preStmnt_ShL_F() {preStmntStkBin(1, &Info::addFunc_ShL_FW);}
      void preStmnt_ShL_I() {preStmntStkBin(2, &Info::addFunc_ShL_UW);}
      void preStmnt_ShL_U() {preStmntStkBin(2, &Info::addFunc_ShL_UW);}
      void preStmnt_ShR();
      void preStmnt_ShR_F() {preStmntStkBin(1, &Info::addFunc_ShR_FW);}
      void preStmnt_ShR_I() {preStmntStkBin(2, &Info::addFunc_ShR_IW);}
      void preStmnt_ShR_U() {preStmntStkBin(2, &Info::addFunc_ShR_UW);}
      void preStmnt_Sub();
      void preStmnt_Sub_F() {preStmntStkCmp(1, &Info::addFunc_Sub_FW);}
      void preStmnt_Sub_I() {preStmntStkCmp(2, &Info::addFunc_Sub_UW);}
      void preStmnt_Sub_U() {preStmntStkCmp(2, &Info::addFunc_Sub_UW);}
      void preStmnt_SubX();
      void preStmnt_SubX_U();
      void preStmnt_Tr();

      void preStmntCall(Core::String name, Core::FastU retrn, Core::FastU param);

      void preStmntStkBin(Core::FastU min, AddFunc add);
      void preStmntStkCmp(Core::FastU min, AddFunc add) {preStmntStkBin(min, add);}
      void preStmntStkCmpU(AddFunc add);

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
      void putStmnt_Add();
      void putStmnt_Add_F() {putStmntStkBin('F');}
      void putStmnt_Add_I() {putStmnt_Add_U();}
      void putStmnt_Add_U();
      void putStmnt_AddX();
      void putStmnt_AddX_U();
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
      void putStmnt_CmpEQ();
      void putStmnt_CmpEQ_F() {putStmntStkCmp(1, 'F');}
      void putStmnt_CmpEQ_I() {putStmnt_CmpEQ_U();}
      void putStmnt_CmpEQ_U();
      void putStmnt_CmpEQ_U(Code codeCmp, Code codeAnd, IR::Code codeCall);
      void putStmnt_CmpGE();
      void putStmnt_CmpGE_F() {putStmntStkCmp(1, 'F');}
      void putStmnt_CmpGE_I() {putStmntStkCmp(1, 'I', Code::CmpI_GE);}
      void putStmnt_CmpGE_U() {putStmntStkCmp(1, 'U');}
      void putStmnt_CmpGT();
      void putStmnt_CmpGT_F() {putStmntStkCmp(0, 'F');}
      void putStmnt_CmpGT_I() {putStmntStkCmp(0, 'I', Code::CmpI_GT);}
      void putStmnt_CmpGT_U() {putStmntStkCmp(0, 'U');}
      void putStmnt_CmpLE();
      void putStmnt_CmpLE_F() {putStmntStkCmp(1, 'F');}
      void putStmnt_CmpLE_I() {putStmntStkCmp(1, 'I', Code::CmpI_LE);}
      void putStmnt_CmpLE_U() {putStmntStkCmp(1, 'U');}
      void putStmnt_CmpLT();
      void putStmnt_CmpLT_F() {putStmntStkCmp(0, 'F');}
      void putStmnt_CmpLT_I() {putStmntStkCmp(0, 'I', Code::CmpI_LT);}
      void putStmnt_CmpLT_U() {putStmntStkCmp(0, 'U');}
      void putStmnt_CmpNE();
      void putStmnt_CmpNE_F() {putStmntStkCmp(0, 'F');}
      void putStmnt_CmpNE_I() {putStmnt_CmpNE_U();}
      void putStmnt_CmpNE_U();
      void putStmnt_Cnat();
      void putStmnt_Copy();
      void putStmnt_Cscr_IA();
      void putStmnt_Cscr_IS();
      void putStmnt_Cscr_SA();
      void putStmnt_Cscr_SS();
      void putStmnt_Cscr_SS_post();
      void putStmnt_Cspe();
      void putStmnt_Div();
      void putStmnt_Div_A() {putStmntStkBin('A');}
      void putStmnt_Div_F() {putStmntStkBin('F');}
      void putStmnt_Div_I();
      void putStmnt_Div_K() {putStmntStkBin('K');}
      void putStmnt_Div_R() {putStmntStkBin('R');}
      void putStmnt_Div_U();
      void putStmnt_Div_X() {putStmntStkBin('X', Code::DivX);}
      void putStmnt_DivX();
      void putStmnt_DivX_I();
      void putStmnt_DivX_U() {putStmntStkBin('U');}
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
      void putStmnt_Mod();
      void putStmnt_Mod_I();
      void putStmnt_Mod_U();
      void putStmnt_Move();
      void putStmnt_Move__Arr_Stk(IR::ArgPtr2 const &arr, Code code);
      void putStmnt_Move__Stk_Arr(IR::ArgPtr2 const &arr, Code code);
      void putStmnt_Mul();
      void putStmnt_Mul_A() {putStmntStkBin('A');}
      void putStmnt_Mul_F() {putStmntStkBin('F');}
      void putStmnt_Mul_I() {putStmntStkBin('U', Code::MulU);}
      void putStmnt_Mul_K() {putStmntStkBin('K');}
      void putStmnt_Mul_R() {putStmntStkBin('R');}
      void putStmnt_Mul_U() {putStmntStkBin('U', Code::MulU);}
      void putStmnt_Mul_X() {putStmntStkBin('X', Code::MulX);}
      void putStmnt_MulX();
      void putStmnt_MulX_U() {putStmntStkBin('U');}
      void putStmnt_Neg();
      void putStmnt_Neg_F();
      void putStmnt_Neg_I();
      void putStmnt_Neg_U() {putStmnt_Neg_I();}
      void putStmnt_Pltn();
      void putStmnt_Retn();
      void putStmnt_Rjnk();
      void putStmnt_ShL();
      void putStmnt_ShL_F();
      void putStmnt_ShL_I() {putStmnt_ShL_U();}
      void putStmnt_ShL_U();
      void putStmnt_ShR();
      void putStmnt_ShR_F() {putStmnt_ShL_F();}
      void putStmnt_ShR_I();
      void putStmnt_ShR_U();
      void putStmnt_ShR_U1();
      void putStmnt_Sub();
      void putStmnt_Sub_F() {putStmntStkBin('F');}
      void putStmnt_Sub_I() {putStmnt_Sub_U();}
      void putStmnt_Sub_U();
      void putStmnt_SubX();
      void putStmnt_SubX_U();
      void putStmnt_Swap();
      void putStmnt_Tr();
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

      void putStmntStkBin(IR::CodeType type, Code code = Code::Nop);
      void putStmntStkCmp(int res0, IR::CodeType type, Code code = Code::Nop);

      void putString(Core::String str);
      void putString(Core::String str, Core::FastU key);

      void putWord(Core::FastU i);

      Core::CounterRef<IR::Exp const> resolveGlyph(Core::String glyph);

      virtual void trFunc();

      virtual void trStmnt();
      void trStmnt_Add();
      void trStmnt_Add_F() {trStmntStkBin(false);}
      void trStmnt_Add_I() {trStmnt_Add_U();}
      void trStmnt_Add_U();
      void trStmnt_AddX();
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
      void trStmnt_CmpEQ();
      void trStmnt_CmpEQ_F() {trStmntStkCmp(false);}
      void trStmnt_CmpEQ_I() {trStmnt_CmpEQ_U();}
      void trStmnt_CmpEQ_U();
      void trStmnt_CmpGE() {trStmntStkCmp(true);}
      void trStmnt_CmpGT() {trStmntStkCmp(true);}
      void trStmnt_CmpLE() {trStmntStkCmp(true);}
      void trStmnt_CmpLT() {trStmntStkCmp(true);}
      void trStmnt_CmpNE();
      void trStmnt_CmpNE_F() {trStmntStkCmp(false);}
      void trStmnt_CmpNE_I() {trStmnt_CmpEQ_U();}
      void trStmnt_CmpNE_U() {trStmnt_CmpEQ_U();}
      void trStmnt_Cnat() {trStmnt_Call();}
      void trStmnt_Copy() {}
      void trStmnt_Cscr_IA() {trStmnt_Call();}
      void trStmnt_Cscr_IS() {trStmnt_Call();}
      void trStmnt_Cscr_SA() {trStmnt_Call();}
      void trStmnt_Cscr_SS() {trStmnt_Call();}
      void trStmnt_Cspe();
      void trStmnt_Div() {trStmntStkBin(true);}
      void trStmnt_DivX();
      void trStmnt_DivX_I();
      void trStmnt_DivX_U() {trStmntStkBin(true);}
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
      void trStmnt_Mod();
      void trStmnt_Mod_I();
      void trStmnt_Mod_U();
      void trStmnt_Move();
      void trStmnt_Mul() {trStmntStkBin(false);}
      void trStmnt_MulX() {trStmntStkBin(false);}
      void trStmnt_Neg();
      void trStmnt_Neg_F() {trStmntStkUna();}
      void trStmnt_Neg_I();
      void trStmnt_Neg_U() {trStmnt_Neg_I();}
      void trStmnt_Pltn() {trStmntStkUna();}
      void trStmnt_Retn();
      void trStmnt_Rjnk() {}
      void trStmnt_ShL();
      void trStmnt_ShL_F();
      void trStmnt_ShL_I() {trStmnt_ShL_U();}
      void trStmnt_ShL_U();
      void trStmnt_ShR();
      void trStmnt_ShR_F() {trStmnt_ShL_F();}
      void trStmnt_ShR_I();
      void trStmnt_ShR_U();
      void trStmnt_ShR_U1();
      void trStmnt_Sub();
      void trStmnt_Sub_F() {trStmntStkBin(true);}
      void trStmnt_Sub_I() {trStmnt_Sub_U();}
      void trStmnt_Sub_U();
      void trStmnt_SubX();
      void trStmnt_Swap();
      void trStmnt_Tr() {trStmntStkUna();}
      void trStmnt_Xcod_SID() {}

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

