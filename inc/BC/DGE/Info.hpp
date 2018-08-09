//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
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

      void chkStmnt_AdXU();
      void chkStmnt_Call();
      void chkStmnt_Cnat();
      void chkStmnt_Copy();
      void chkStmnt_Jcnd_Nil();
      void chkStmnt_Jcnd_Tab();
      void chkStmnt_Jcnd_Tru() {chkStmnt_Jcnd_Nil();}
      void chkStmnt_Jfar();
      void chkStmnt_Jset();
      void chkStmnt_Move();
      void chkStmnt_SuXU() {chkStmnt_AdXU();}
      void chkStmnt_Swap();

      virtual void chkStmntArg(IR::Arg const &arg);

      virtual void genObj();

      virtual FixedInfo getFixedInfo(Core::FastU n, bool s);

      Core::FastU getStkPtrIdx();

      Core::FastU getStmntSizeW();

      bool isPushArg(IR::Arg const &arg);

      bool isDropArg(IR::Arg const &arg);

      virtual void preStmnt();

      void preStmnt_AddF() {preStmntStkBin(1, &Info::addFunc_AddF_W);}
      void preStmnt_AddI() {preStmnt_AddU();}
      void preStmnt_AddU();
      void preStmnt_Bclo();
      void preStmnt_Bclz();
      void preStmnt_CmpF_EQ() {preStmntStkCmp(1, &Info::addFunc_CmpF_EQ_W);}
      void preStmnt_CmpF_GE() {preStmntStkCmp(1, &Info::addFunc_CmpF_GE_W);}
      void preStmnt_CmpF_GT() {preStmntStkCmp(1, &Info::addFunc_CmpF_GT_W);}
      void preStmnt_CmpF_LE() {preStmntStkCmp(1, &Info::addFunc_CmpF_LE_W);}
      void preStmnt_CmpF_LT() {preStmntStkCmp(1, &Info::addFunc_CmpF_LT_W);}
      void preStmnt_CmpF_NE() {preStmntStkCmp(1, &Info::addFunc_CmpF_NE_W);}
      void preStmnt_CmpI_EQ() {preStmnt_CmpU_EQ();}
      void preStmnt_CmpI_GE() {preStmntStkCmp(2, &Info::addFunc_CmpI_GE_W);}
      void preStmnt_CmpI_GT() {preStmntStkCmp(2, &Info::addFunc_CmpI_GT_W);}
      void preStmnt_CmpI_LE() {preStmntStkCmp(2, &Info::addFunc_CmpI_LE_W);}
      void preStmnt_CmpI_LT() {preStmntStkCmp(2, &Info::addFunc_CmpI_LT_W);}
      void preStmnt_CmpI_NE() {preStmnt_CmpU_NE();}
      void preStmnt_CmpU_EQ(AddFunc add = &Info::addFunc_CmpU_EQ_W);
      void preStmnt_CmpU_GE() {preStmntStkCmp(2, &Info::addFunc_CmpU_GE_W);}
      void preStmnt_CmpU_GT() {preStmntStkCmp(2, &Info::addFunc_CmpU_GT_W);}
      void preStmnt_CmpU_LE() {preStmntStkCmp(2, &Info::addFunc_CmpU_LE_W);}
      void preStmnt_CmpU_LT() {preStmntStkCmp(2, &Info::addFunc_CmpU_LT_W);}
      void preStmnt_CmpU_NE() {preStmnt_CmpU_EQ(&Info::addFunc_CmpU_NE_W);}
      void preStmnt_DiXI() {preStmntStkBin(2, &Info::addFunc_DiXI_W);}
      void preStmnt_DiXU() {preStmntStkBin(2, &Info::addFunc_DiXU_W);}
      void preStmnt_DivF() {preStmntStkBin(1, &Info::addFunc_DivF_W);}
      void preStmnt_DivI() {preStmntStkBin(2, &Info::addFunc_DiXI_W);}
      void preStmnt_DivK() {preStmntStkBin(1, &Info::addFunc_DivK_W);}
      void preStmnt_DivU() {preStmntStkBin(2, &Info::addFunc_DiXU_W);}
      void preStmnt_DivX() {preStmntStkBin(1, &Info::addFunc_DivX_W);}
      void preStmnt_ModI() {preStmntStkBin(2, &Info::addFunc_DiXI_W);}
      void preStmnt_ModU() {preStmntStkBin(2, &Info::addFunc_DiXU_W);}
      void preStmnt_MuXU() {preStmntStkBin(2, &Info::addFunc_MuXU_W);}
      void preStmnt_MulF() {preStmntStkBin(1, &Info::addFunc_MulF_W);}
      void preStmnt_MulI() {preStmntStkBin(2, &Info::addFunc_MulU_W);}
      void preStmnt_MulK() {preStmntStkBin(1, &Info::addFunc_MulK_W);}
      void preStmnt_MulU() {preStmntStkBin(2, &Info::addFunc_MulU_W);}
      void preStmnt_MulX() {preStmntStkBin(1, &Info::addFunc_MulX_W);}
      void preStmnt_NegI() {preStmntStkUna(2, &Info::addFunc_NegI_W);}
      void preStmnt_ShLF() {preStmntStkShi(1, &Info::addFunc_ShLF_W);}
      void preStmnt_ShLU();
      void preStmnt_ShRF() {preStmntStkShi(1, &Info::addFunc_ShRF_W);}
      void preStmnt_ShRI();
      void preStmnt_ShRU();
      void preStmnt_SubF() {preStmntStkBin(1, &Info::addFunc_SubF_W);}
      void preStmnt_SubI() {preStmnt_SubU();}
      void preStmnt_SubU();

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

      void putStmnt_AdXU();
      void putStmnt_AddF() {putStmntStkBin();}
      void putStmnt_AddI() {putStmnt_AddU();}
      void putStmnt_AddU();
      void putStmnt_BAnd(char const *code = "BAnd");
      void putStmnt_BNot();
      void putStmnt_BOrI() {putStmnt_BAnd("BOrI");}
      void putStmnt_BOrX() {putStmnt_BAnd("BOrX");}
      void putStmnt_Bclo() {putStmnt_Bclz("Bclo");}
      void putStmnt_Bclz(char const *code = "Bclz");
      void putStmnt_Call();
      void putStmnt_CmpF_EQ() {putStmntStkCmp(1);}
      void putStmnt_CmpF_GE() {putStmntStkCmp(1);}
      void putStmnt_CmpF_GT() {putStmntStkCmp(0);}
      void putStmnt_CmpF_LE() {putStmntStkCmp(1);}
      void putStmnt_CmpF_LT() {putStmntStkCmp(0);}
      void putStmnt_CmpF_NE() {putStmntStkCmp(0);}
      void putStmnt_CmpI_EQ() {putStmnt_CmpU_EQ();}
      void putStmnt_CmpI_GE() {putStmntStkCmp(1, "CmpI_GE");}
      void putStmnt_CmpI_GT() {putStmntStkCmp(0, "CmpI_GT");}
      void putStmnt_CmpI_LE() {putStmntStkCmp(1, "CmpI_LE");}
      void putStmnt_CmpI_LT() {putStmntStkCmp(0, "CmpI_LT");}
      void putStmnt_CmpI_NE() {putStmnt_CmpU_NE();}
      void putStmnt_CmpU_EQ(IR::Code code = IR::Code::CmpU_EQ);
      void putStmnt_CmpU_GE() {putStmntStkCmp(1, "CmpU_GE");}
      void putStmnt_CmpU_GT() {putStmntStkCmp(0, "CmpU_GT");}
      void putStmnt_CmpU_LE() {putStmntStkCmp(1, "CmpU_LE");}
      void putStmnt_CmpU_LT() {putStmntStkCmp(0, "CmpU_LT");}
      void putStmnt_CmpU_NE() {putStmnt_CmpU_EQ(IR::Code::CmpU_NE);}
      void putStmnt_Cnat();
      void putStmnt_Copy();
      void putStmnt_DiXI() {putStmntStkBin("DiXI");}
      void putStmnt_DiXU() {putStmntStkBin("DiXU");}
      void putStmnt_DivF() {putStmntStkBin();}
      void putStmnt_DivI() {putStmnt_DivU("DivI", IR::Code::DiXI, false);}
      void putStmnt_DivK() {putStmntStkBin();}
      void putStmnt_DivU() {putStmnt_DivU("DivU", IR::Code::DiXU, false);}
      void putStmnt_DivX() {putStmntStkBin();}
      void putStmnt_Jcnd_Nil(char const *code = "Jcnd_Nil");
      void putStmnt_Jcnd_Tab();
      void putStmnt_Jcnd_Tru();
      void putStmnt_Jfar();
      void putStmnt_Jset();
      void putStmnt_Jump();
      void putStmnt_LAnd(char const *code = "LAnd");
      void putStmnt_LNot();
      void putStmnt_LOrI() {putStmnt_LAnd("LOrI");}
      void putStmnt_ModI() {putStmnt_DivU("ModI", IR::Code::DiXI, true);}
      void putStmnt_ModU() {putStmnt_DivU("ModU", IR::Code::DiXU, true);}
      void putStmnt_Move();
      void putStmnt_MuXU() {putStmntStkBin("MuXU");}
      void putStmnt_MulF() {putStmntStkBin();}
      void putStmnt_MulI() {putStmnt_MulU();}
      void putStmnt_MulK() {putStmntStkBin();}
      void putStmnt_MulU();
      void putStmnt_MulX() {putStmntStkBin();}
      void putStmnt_NegF();
      void putStmnt_NegI() {putStmntStkUna("NegI");}
      void putStmnt_Pltn();
      void putStmnt_Retn();
      void putStmnt_ShLF() {putStmntStkShi();}
      void putStmnt_ShLU(char const *code = "ShLU");
      void putStmnt_ShRF() {putStmntStkShi();}
      void putStmnt_ShRI() {putStmnt_ShLU("ShRI");}
      void putStmnt_ShRU() {putStmnt_ShLU("ShRU");}
      void putStmnt_SuXU();
      void putStmnt_SubF() {putStmntStkBin();}
      void putStmnt_SubI() {putStmnt_SubU();}
      void putStmnt_SubU();
      void putStmnt_Swap();

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

      void putStmntStkBin(char const *code = nullptr);
      void putStmntStkCmp(int res0, char const *code = nullptr);
      void putStmntStkShi(char const *code = nullptr);
      void putStmntStkUna(char const *code = nullptr);

      virtual void putStr();
      using InfoBase::putStr;

      void putValue(Core::Origin pos, IR::Value const &val); // Full output.
      void putValue(Core::Origin pos, IR::Value const &val, Core::FastU w);

      void putValueMulti(Core::Origin pos, Core::Array<IR::Value> const &val); // Full output.
      void putValueMulti(Core::Origin pos, Core::Array<IR::Value> const &val, Core::FastU w);

      void putWord(WordValue const &w);

      virtual void trStmnt();
      void trStmnt_AdXU();
      void trStmnt_AddF() {trStmntStkBin(false);}
      void trStmnt_AddI() {trStmnt_AddU();}
      void trStmnt_AddU();
      void trStmnt_BAnd();
      void trStmnt_BNot();
      void trStmnt_BOrI() {trStmnt_BAnd();}
      void trStmnt_BOrX() {trStmnt_BAnd();}
      void trStmnt_Bclo() {trStmnt_Bclz();}
      void trStmnt_Bclz();
      void trStmnt_Call();
      void trStmnt_CmpF_EQ() {trStmntStkCmp(false);}
      void trStmnt_CmpF_GE() {trStmntStkCmp(true);}
      void trStmnt_CmpF_GT() {trStmntStkCmp(true);}
      void trStmnt_CmpF_LE() {trStmntStkCmp(true);}
      void trStmnt_CmpF_LT() {trStmntStkCmp(true);}
      void trStmnt_CmpF_NE() {trStmntStkCmp(false);}
      void trStmnt_CmpI_EQ() {trStmntStkCmp(false);}
      void trStmnt_CmpI_GE() {trStmntStkCmp(true);}
      void trStmnt_CmpI_GT() {trStmntStkCmp(true);}
      void trStmnt_CmpI_LE() {trStmntStkCmp(true);}
      void trStmnt_CmpI_LT() {trStmntStkCmp(true);}
      void trStmnt_CmpI_NE() {trStmntStkCmp(false);}
      void trStmnt_CmpU_EQ() {trStmntStkCmp(false);}
      void trStmnt_CmpU_GE() {trStmntStkCmp(true);}
      void trStmnt_CmpU_GT() {trStmntStkCmp(true);}
      void trStmnt_CmpU_LE() {trStmntStkCmp(true);}
      void trStmnt_CmpU_LT() {trStmntStkCmp(true);}
      void trStmnt_CmpU_NE() {trStmntStkCmp(false);}
      void trStmnt_Cnat();
      void trStmnt_Copy() {}
      void trStmnt_DiXI() {trStmnt_DiXU();}
      void trStmnt_DiXU();
      void trStmnt_DivF() {trStmntStkBin(true);}
      void trStmnt_DivI() {trStmntStkBin(true);}
      void trStmnt_DivK() {trStmntStkBin(true);}
      void trStmnt_DivU() {trStmntStkBin(true);}
      void trStmnt_DivX() {trStmntStkBin(true);}
      void trStmnt_Jcnd_Nil();
      void trStmnt_Jcnd_Tab() {}
      void trStmnt_Jcnd_Tru() {trStmnt_Jcnd_Nil();}
      void trStmnt_Jfar() {}
      void trStmnt_Jset() {}
      void trStmnt_Jump();
      void trStmnt_LAnd();
      void trStmnt_LNot();
      void trStmnt_LOrI() {trStmnt_LAnd();}
      void trStmnt_ModI() {trStmnt_ModU();}
      void trStmnt_ModU();
      void trStmnt_Move();
      void trStmnt_MuXU();
      void trStmnt_MulF() {trStmntStkBin(false);}
      void trStmnt_MulI() {trStmntStkBin(false);}
      void trStmnt_MulK() {trStmntStkBin(false);}
      void trStmnt_MulU() {trStmntStkBin(false);}
      void trStmnt_MulX() {trStmntStkBin(false);}
      void trStmnt_NegF() {trStmntStkUna();}
      void trStmnt_NegI() {trStmntStkUna();}
      void trStmnt_Pltn();
      void trStmnt_Retn();
      void trStmnt_ShLF() {trStmntStkShi();}
      void trStmnt_ShLU();
      void trStmnt_ShRF() {trStmntStkShi();}
      void trStmnt_ShRI() {trStmnt_ShLU();}
      void trStmnt_ShRU() {trStmnt_ShLU();}
      void trStmnt_SuXU();
      void trStmnt_SubF() {trStmntStkBin(true);}
      void trStmnt_SubI() {trStmnt_SubU();}
      void trStmnt_SubU();
      void trStmnt_Swap();

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

      void putStmnt_DivU(char const *code, IR::Code codeX, bool mod);

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

