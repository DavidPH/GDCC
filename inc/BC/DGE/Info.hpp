//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati target information and handling class.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__DGE__Info_H__
#define GDCC__BC__DGE__Info_H__

#include "../../BC/DGE/Types.hpp"

#include "../../BC/Info.hpp"

#include "../../Core/Origin.hpp"

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::BC::DGE
{
   //
   // Info
   //
   class Info : public InfoBase
   {
   protected:
      void backGlyphObj(Core::String glyph, Core::FastU val);

      virtual void chkStmnt();

      void chkStmnt_AddX();
      void chkStmnt_Call();
      void chkStmnt_Cnat();
      void chkStmnt_Copy();
      void chkStmnt_Jcnd_Nil();
      void chkStmnt_Jcnd_Tab();
      void chkStmnt_Jcnd_Tru() {chkStmnt_Jcnd_Nil();}
      void chkStmnt_Jfar_Pro();
      void chkStmnt_Jfar_Set();
      void chkStmnt_Jfar_Sta();
      void chkStmnt_Move();
      void chkStmnt_SubX() {chkStmnt_AddX();}
      void chkStmnt_Swap();

      virtual void chkStmntArg(IR::Arg const &arg);

      virtual void genObj();

      virtual FixedInfo getFixedInfo(Core::FastU n, bool s);

      Core::FastU getStkPtrIdx();

      Core::FastU getStmntSizeW();
      Core::FastU getStmntSizeW(Core::FastU b);

      bool isPushArg(IR::Arg const &arg);

      bool isDropArg(IR::Arg const &arg);

      virtual void preStmnt();

      void preStmnt_Add();
      void preStmnt_Add_F() {preStmntStkBin(1, &Info::addFunc_Add_FW);}
      void preStmnt_Add_I() {preStmntStkBin(2, &Info::addFunc_Add_UW);}
      void preStmnt_Add_U() {preStmntStkBin(2, &Info::addFunc_Add_UW);}
      void preStmnt_Bclo();
      void preStmnt_Bclz();
      void preStmnt_CmpEQ();
      void preStmnt_CmpEQ_F() {preStmntStkCmp(1, &Info::addFunc_CmpEQ_FW);}
      void preStmnt_CmpEQ_I() {preStmntStkCmp(2, &Info::addFunc_CmpEQ_UW);}
      void preStmnt_CmpEQ_U() {preStmntStkCmp(2, &Info::addFunc_CmpEQ_UW);}
      void preStmnt_CmpGE();
      void preStmnt_CmpGE_F() {preStmntStkCmp(1, &Info::addFunc_CmpGE_FW);}
      void preStmnt_CmpGE_I() {preStmntStkCmp(2, &Info::addFunc_CmpGE_IW);}
      void preStmnt_CmpGE_U() {preStmntStkCmp(2, &Info::addFunc_CmpGE_UW);}
      void preStmnt_CmpGT();
      void preStmnt_CmpGT_F() {preStmntStkCmp(1, &Info::addFunc_CmpGT_FW);}
      void preStmnt_CmpGT_I() {preStmntStkCmp(2, &Info::addFunc_CmpGT_IW);}
      void preStmnt_CmpGT_U() {preStmntStkCmp(2, &Info::addFunc_CmpGT_UW);}
      void preStmnt_CmpLE();
      void preStmnt_CmpLE_F() {preStmntStkCmp(1, &Info::addFunc_CmpLE_FW);}
      void preStmnt_CmpLE_I() {preStmntStkCmp(2, &Info::addFunc_CmpLE_IW);}
      void preStmnt_CmpLE_U() {preStmntStkCmp(2, &Info::addFunc_CmpLE_UW);}
      void preStmnt_CmpLT();
      void preStmnt_CmpLT_F() {preStmntStkCmp(1, &Info::addFunc_CmpLT_FW);}
      void preStmnt_CmpLT_I() {preStmntStkCmp(2, &Info::addFunc_CmpLT_IW);}
      void preStmnt_CmpLT_U() {preStmntStkCmp(2, &Info::addFunc_CmpLT_UW);}
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
      void preStmnt_Div_U() {preStmntStkBin(2, &Info::addFunc_DivX_UW);}
      void preStmnt_Div_X() {preStmntStkBin(1, &Info::addFunc_Div_XW);}
      void preStmnt_DivX();
      void preStmnt_DivX_I() {preStmntStkBin(2, &Info::addFunc_DivX_IW);}
      void preStmnt_DivX_U() {preStmntStkBin(2, &Info::addFunc_DivX_UW);}
      void preStmnt_Mod();
      void preStmnt_Mod_I() {preStmntStkBin(2, &Info::addFunc_DivX_IW);}
      void preStmnt_Mod_U() {preStmntStkBin(2, &Info::addFunc_DivX_UW);}
      void preStmnt_Mul();
      void preStmnt_Mul_A() {preStmntStkBin(1, &Info::addFunc_Mul_AW);}
      void preStmnt_Mul_F() {preStmntStkBin(1, &Info::addFunc_Mul_FW);}
      void preStmnt_Mul_I() {preStmntStkBin(2, &Info::addFunc_Mul_UW);}
      void preStmnt_Mul_K() {preStmntStkBin(1, &Info::addFunc_Mul_KW);}
      void preStmnt_Mul_R() {preStmntStkBin(1, &Info::addFunc_Mul_RW);}
      void preStmnt_Mul_U() {preStmntStkBin(2, &Info::addFunc_Mul_UW);}
      void preStmnt_Mul_X() {preStmntStkBin(1, &Info::addFunc_Mul_XW);}
      void preStmnt_MulX();
      void preStmnt_MulX_U() {preStmntStkBin(2, &Info::addFunc_MulX_UW);}
      void preStmnt_Neg();
      void preStmnt_Neg_F() {}
      void preStmnt_Neg_I() {preStmntStkUna(2, &Info::addFunc_Neg_IW);}
      void preStmnt_Neg_U() {preStmntStkUna(2, &Info::addFunc_Neg_IW);}
      void preStmnt_ShL();
      void preStmnt_ShL_F() {preStmntStkShi(1, &Info::addFunc_ShL_FW);}
      void preStmnt_ShL_I() {preStmntStkShi(2, &Info::addFunc_ShL_UW);}
      void preStmnt_ShL_U() {preStmntStkShi(2, &Info::addFunc_ShL_UW);}
      void preStmnt_ShR();
      void preStmnt_ShR_F() {preStmntStkShi(1, &Info::addFunc_ShR_FW);}
      void preStmnt_ShR_I() {preStmntStkShi(2, &Info::addFunc_ShR_IW);}
      void preStmnt_ShR_U() {preStmntStkShi(2, &Info::addFunc_ShR_UW);}
      void preStmnt_Sub();
      void preStmnt_Sub_F() {preStmntStkBin(1, &Info::addFunc_Sub_FW);}
      void preStmnt_Sub_I() {preStmntStkBin(2, &Info::addFunc_Sub_UW);}
      void preStmnt_Sub_U() {preStmntStkBin(2, &Info::addFunc_Sub_UW);}
      void preStmnt_Tr();

      void preStmntStkBin(Core::FastU min, AddFunc add);
      void preStmntStkCmp(Core::FastU min, AddFunc add) {preStmntStkBin(min, add);}
      void preStmntStkShi(Core::FastU min, AddFunc add) {preStmntStkBin(min, add);}
      void preStmntStkUna(Core::FastU min, AddFunc add) {preStmntStkBin(min, add);}

      virtual void put();

      void putCode(char const *code) {putNTS(code); putNTS('('); putNTS(')');}
      template<typename Arg0>
      void putCode(char const *code, Arg0 &&arg0);
      template<typename Arg0, typename Arg1>
      void putCode(char const *code, Arg0 &&arg0, Arg1 &&arg1);

      virtual void putDJump();
      using InfoBase::putDJump;

      void putExp(IR::Exp const *exp); // Full output.
      void putExp(IR::Exp const *exp, Core::FastU w);

      void putExpAdd(IR::Exp const *expL, Core::FastU expR);

      void putExp_Cst(IR::Exp_Cst const *exp);

      virtual void putFunc();
      using InfoBase::putFunc;

      void putInt(int i);
      void putInt(unsigned int i);
      void putInt(Core::FastU i);
      void putInt(Core::FastI i);

      void putNTS(char nts);
      void putNTS(char const *nts);
      void putNTS(Core::String nts);

      virtual void putObj();
      using InfoBase::putObj;

      void putOrigin(Core::Origin pos);
      void putOriginFunc(Core::String pos);

      virtual void putStmnt();

      void putStmnt_Add();
      void putStmnt_Add_F() {putStmntStkBin('F');}
      void putStmnt_Add_I() {putStmntStkBin('U', "AddU");}
      void putStmnt_Add_U() {putStmntStkBin('U', "AddU");}
      void putStmnt_AddX();
      void putStmnt_AddX_U();
      void putStmnt_BAnd(char const *code = "BAnd");
      void putStmnt_BNot();
      void putStmnt_BOrI() {putStmnt_BAnd("BOrI");}
      void putStmnt_BOrX() {putStmnt_BAnd("BOrX");}
      void putStmnt_Bclo() {putStmnt_Bclz("Bclo");}
      void putStmnt_Bclz(char const *code = "Bclz");
      void putStmnt_Call();
      void putStmnt_CmpEQ();
      void putStmnt_CmpEQ_F() {putStmntStkCmp(1, 'F');}
      void putStmnt_CmpEQ_I() {putStmnt_CmpEQ_U();}
      void putStmnt_CmpEQ_U(IR::Code code = IR::CodeBase::CmpEQ+'U');
      void putStmnt_CmpGE();
      void putStmnt_CmpGE_F() {putStmntStkCmp(1, 'F');}
      void putStmnt_CmpGE_I() {putStmntStkCmp(1, 'I', "CmpI_GE");}
      void putStmnt_CmpGE_U() {putStmntStkCmp(1, 'U', "CmpU_GE");}
      void putStmnt_CmpGT();
      void putStmnt_CmpGT_F() {putStmntStkCmp(0, 'F');}
      void putStmnt_CmpGT_I() {putStmntStkCmp(0, 'I', "CmpI_GT");}
      void putStmnt_CmpGT_U() {putStmntStkCmp(0, 'U', "CmpU_GT");}
      void putStmnt_CmpLE();
      void putStmnt_CmpLE_F() {putStmntStkCmp(1, 'F');}
      void putStmnt_CmpLE_I() {putStmntStkCmp(1, 'I', "CmpI_LE");}
      void putStmnt_CmpLE_U() {putStmntStkCmp(1, 'U', "CmpU_LE");}
      void putStmnt_CmpLT();
      void putStmnt_CmpLT_F() {putStmntStkCmp(0, 'F');}
      void putStmnt_CmpLT_I() {putStmntStkCmp(0, 'I', "CmpI_LT");}
      void putStmnt_CmpLT_U() {putStmntStkCmp(0, 'U', "CmpU_LT");}
      void putStmnt_CmpNE();
      void putStmnt_CmpNE_F() {putStmntStkCmp(0, 'F');}
      void putStmnt_CmpNE_I() {putStmnt_CmpNE_U();}
      void putStmnt_CmpNE_U() {putStmnt_CmpEQ_U(IR::CodeBase::CmpNE+'U');}
      void putStmnt_Cnat();
      void putStmnt_Copy();
      void putStmnt_Div();
      void putStmnt_Div_A();
      void putStmnt_Div_F() {putStmntStkBin('F');}
      void putStmnt_Div_I() {putStmnt_Div_U("DivI", IR::CodeBase::DivX+'I', false);}
      void putStmnt_Div_K() {putStmntStkBin('K');}
      void putStmnt_Div_R();
      void putStmnt_Div_U() {putStmnt_Div_U("DivU", IR::CodeBase::DivX+'U', false);}
      void putStmnt_Div_X() {putStmntStkBin('X');}
      void putStmnt_DivX();
      void putStmnt_DivX_I() {putStmntStkBin('I', "DiXI");}
      void putStmnt_DivX_U() {putStmntStkBin('U', "DiXU");}
      void putStmnt_Jcnd_Nil(char const *code = "Jcnd_Nil");
      void putStmnt_Jcnd_Tab();
      void putStmnt_Jcnd_Tru();
      void putStmnt_Jfar_Pro();
      void putStmnt_Jfar_Set();
      void putStmnt_Jfar_Sta();
      void putStmnt_Jump();
      void putStmnt_LAnd(char const *code = "LAnd");
      void putStmnt_LNot();
      void putStmnt_LOrI() {putStmnt_LAnd("LOrI");}
      void putStmnt_Mod();
      void putStmnt_Mod_I() {putStmnt_Div_U("ModI", IR::CodeBase::DivX+'I', true);}
      void putStmnt_Mod_U() {putStmnt_Div_U("ModU", IR::CodeBase::DivX+'U', true);}
      void putStmnt_Move();
      void putStmnt_Mul();
      void putStmnt_Mul_A();
      void putStmnt_Mul_F() {putStmntStkBin('F');}
      void putStmnt_Mul_I() {putStmntStkBin('U', "MulU");}
      void putStmnt_Mul_K() {putStmntStkBin('K');}
      void putStmnt_Mul_R();
      void putStmnt_Mul_U() {putStmntStkBin('U', "MulU");}
      void putStmnt_Mul_X() {putStmntStkBin('X');}
      void putStmnt_MulX();
      void putStmnt_MulX_U() {putStmntStkBin('U', "MuXU");}
      void putStmnt_Neg();
      void putStmnt_Neg_F();
      void putStmnt_Neg_I();
      void putStmnt_Neg_U() {putStmnt_Neg_I();}
      void putStmnt_Pltn();
      void putStmnt_Retn();
      void putStmnt_Rjnk() {putStmnt_Retn();}
      void putStmnt_ShL();
      void putStmnt_ShL_F() {putStmntStkShi('F');}
      void putStmnt_ShL_I() {putStmntStkShi('U', "ShLU");}
      void putStmnt_ShL_U() {putStmntStkShi('U', "ShLU");}
      void putStmnt_ShR();
      void putStmnt_ShR_F() {putStmntStkShi('F');}
      void putStmnt_ShR_I() {putStmntStkShi('I', "ShRI");}
      void putStmnt_ShR_U() {putStmntStkShi('U', "ShRU");}
      void putStmnt_Sub();
      void putStmnt_Sub_F() {putStmntStkBin('F');}
      void putStmnt_Sub_I() {putStmntStkBin('U', "SubU");}
      void putStmnt_Sub_U() {putStmntStkBin('U', "SubU");}
      void putStmnt_SubX();
      void putStmnt_SubX_U();
      void putStmnt_Swap();
      void putStmnt_Tr();

      void putStmntCall(Core::String name, Core::FastU argc);

      void putStmntDropArg(IR::Arg const &arg, Core::FastU w);
      void putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntDropTmp(Core::FastU w);
      void putStmntDropTmp(Core::FastU lo, Core::FastU hi);

      void putStmntPushArg(IR::Arg const &arg, Core::FastU w);
      void putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntPushIdx(IR::Arg const &arg, Core::FastU w);

      void putStmntPushTmp(Core::FastU w);
      void putStmntPushTmp(Core::FastU lo, Core::FastU hi);

      void putStmntStkBin(IR::CodeType type, char const *code = nullptr);
      void putStmntStkCmp(int res0, IR::CodeType type, char const *code = nullptr);
      void putStmntStkShi(IR::CodeType type, char const *code = nullptr);
      void putStmntStkUna(IR::CodeType type, char const *code = nullptr);

      virtual void putStrEnt();
      using InfoBase::putStrEnt;

      void putValue(Core::Origin pos, IR::Value const &val); // Full output.
      void putValue(Core::Origin pos, IR::Value const &val, Core::FastU w);

      void putValueMulti(Core::Origin pos, Core::Array<IR::Value> const &val); // Full output.
      void putValueMulti(Core::Origin pos, Core::Array<IR::Value> const &val, Core::FastU w);

      void putWord(WordValue const &w);

      virtual void trStmnt();
      void trStmnt_Add() {trStmntStkBin(false);}
      void trStmnt_AddX();
      void trStmnt_BAnd();
      void trStmnt_BNot();
      void trStmnt_BOrI() {trStmnt_BAnd();}
      void trStmnt_BOrX() {trStmnt_BAnd();}
      void trStmnt_Bclo() {trStmnt_Bclz();}
      void trStmnt_Bclz();
      void trStmnt_Call();
      void trStmnt_CmpEQ() {trStmntStkCmp(false);}
      void trStmnt_CmpGE() {trStmntStkCmp(true);}
      void trStmnt_CmpGT() {trStmntStkCmp(true);}
      void trStmnt_CmpLE() {trStmntStkCmp(true);}
      void trStmnt_CmpLT() {trStmntStkCmp(true);}
      void trStmnt_CmpNE() {trStmntStkCmp(false);}
      void trStmnt_Cnat();
      void trStmnt_Copy() {}
      void trStmnt_Div() {trStmntStkBin(true);}
      void trStmnt_DivX() {trStmntStkBin(true);}
      void trStmnt_Jcnd_Nil();
      void trStmnt_Jcnd_Tab() {}
      void trStmnt_Jcnd_Tru() {trStmnt_Jcnd_Nil();}
      void trStmnt_Jfar_Pro() {}
      void trStmnt_Jfar_Set() {}
      void trStmnt_Jfar_Sta() {}
      void trStmnt_Jump();
      void trStmnt_LAnd();
      void trStmnt_LNot();
      void trStmnt_LOrI() {trStmnt_LAnd();}
      void trStmnt_Mod();
      void trStmnt_Mod_I() {trStmnt_Mod_U();}
      void trStmnt_Mod_U();
      void trStmnt_Move();
      void trStmnt_Mul() {trStmntStkBin(false);}
      void trStmnt_MulX() {trStmntStkBin(false);}
      void trStmnt_Neg() {trStmntStkUna();}
      void trStmnt_Pltn();
      void trStmnt_Retn();
      void trStmnt_Rjnk() {trStmnt_Retn();}
      void trStmnt_ShL() {trStmntStkShi();}
      void trStmnt_ShR() {trStmntStkShi();}
      void trStmnt_Sub() {trStmntStkBin(true);}
      void trStmnt_SubX();
      void trStmnt_Swap();
      void trStmnt_Tr() {trStmntStkUna();}

      void trStmntStkBin(bool ordered);
      void trStmntStkCmp(bool ordered);
      void trStmntStkShi();
      void trStmntStkUna();

      void trStmntTmp(Core::FastU n);

   private:
      void putCodeArg(char const        *arg) {putNTS(arg);}
      void putCodeArg(int                arg) {putInt(arg);}
      void putCodeArg(unsigned int       arg) {putInt(arg);}
      void putCodeArg(Core::FastU        arg) {putInt(arg);}
      void putCodeArg(Core::String       arg);
      void putCodeArg(IR::Arg_Lit const &arg);

      void putStmnt_Div_U(char const *code, IR::Code codeX, bool mod);

      Core::Origin lastOrigin;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::putCode
   //
   template<typename Arg0>
   void Info::putCode(char const *code, Arg0 &&arg0)
   {
      putNTS(    code); putNTS('(');
      putCodeArg(arg0); putNTS(')');
   }

   //
   // Info::putCode
   //
   template<typename Arg0, typename Arg1>
   void Info::putCode(char const *code, Arg0 &&arg0, Arg1 &&arg1)
   {
      putNTS(    code); putNTS('(');
      putCodeArg(arg0); putNTS(',');
      putCodeArg(arg1); putNTS(')');
   }
}

#endif//GDCC__BC__DGE__Info_H__

