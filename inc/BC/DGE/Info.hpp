//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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

#include "../../BC/Info.hpp"

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   class Exp_Cst;
}

namespace GDCC::BC::DGE
{
   //
   // Info
   //
   class Info : public InfoBase
   {
   protected:
      void backGlyphObj(Core::String glyph, Core::FastU val);

      virtual void genObj();

      Core::FastU getStkPtrIdx();

      bool isPushArg(IR::Arg const &arg);

      bool isDropArg(IR::Arg const &arg);

      virtual void preStmnt();

      void preStmnt_AddF_W() {preStmntStkBin(1, &Info::addFunc_AddF_W);}
      void preStmnt_AddI_W() {preStmnt_AddU_W();}
      void preStmnt_AddU_W();
      void preStmnt_Bclo_W();
      void preStmnt_Bclz_W();
      void preStmnt_CmpF_EQ_W() {preStmntStkCmp(1, &Info::addFunc_CmpF_EQ_W);}
      void preStmnt_CmpF_GE_W() {preStmntStkCmp(1, &Info::addFunc_CmpF_GE_W);}
      void preStmnt_CmpF_GT_W() {preStmntStkCmp(1, &Info::addFunc_CmpF_GT_W);}
      void preStmnt_CmpF_LE_W() {preStmntStkCmp(1, &Info::addFunc_CmpF_LE_W);}
      void preStmnt_CmpF_LT_W() {preStmntStkCmp(1, &Info::addFunc_CmpF_LT_W);}
      void preStmnt_CmpF_NE_W() {preStmntStkCmp(1, &Info::addFunc_CmpF_NE_W);}
      void preStmnt_CmpI_EQ_W() {preStmnt_CmpU_EQ_W();}
      void preStmnt_CmpI_GE_W() {preStmntStkCmp(2, &Info::addFunc_CmpI_GE_W);}
      void preStmnt_CmpI_GT_W() {preStmntStkCmp(2, &Info::addFunc_CmpI_GT_W);}
      void preStmnt_CmpI_LE_W() {preStmntStkCmp(2, &Info::addFunc_CmpI_LE_W);}
      void preStmnt_CmpI_LT_W() {preStmntStkCmp(2, &Info::addFunc_CmpI_LT_W);}
      void preStmnt_CmpI_NE_W() {preStmnt_CmpU_NE_W();}
      void preStmnt_CmpU_EQ_W(AddFunc add = &Info::addFunc_CmpU_EQ_W);
      void preStmnt_CmpU_GE_W() {preStmntStkCmp(2, &Info::addFunc_CmpU_GE_W);}
      void preStmnt_CmpU_GT_W() {preStmntStkCmp(2, &Info::addFunc_CmpU_GT_W);}
      void preStmnt_CmpU_LE_W() {preStmntStkCmp(2, &Info::addFunc_CmpU_LE_W);}
      void preStmnt_CmpU_LT_W() {preStmntStkCmp(2, &Info::addFunc_CmpU_LT_W);}
      void preStmnt_CmpU_NE_W() {preStmnt_CmpU_EQ_W(&Info::addFunc_CmpU_NE_W);}
      void preStmnt_DiXI_W() {preStmntStkBin(2, &Info::addFunc_DiXI_W);}
      void preStmnt_DiXU_W() {preStmntStkBin(2, &Info::addFunc_DiXU_W);}
      void preStmnt_DivF_W() {preStmntStkBin(1, &Info::addFunc_DivF_W);}
      void preStmnt_DivI_W() {preStmntStkBin(2, &Info::addFunc_DiXI_W);}
      void preStmnt_DivU_W() {preStmntStkBin(2, &Info::addFunc_DiXU_W);}
      void preStmnt_ModI_W() {preStmntStkBin(2, &Info::addFunc_DiXI_W);}
      void preStmnt_ModU_W() {preStmntStkBin(2, &Info::addFunc_DiXU_W);}
      void preStmnt_MuXU_W() {preStmntStkBin(2, &Info::addFunc_MuXU_W);}
      void preStmnt_MulF_W() {preStmntStkBin(1, &Info::addFunc_MulF_W);}
      void preStmnt_MulI_W() {preStmntStkBin(2, &Info::addFunc_MulU_W);}
      void preStmnt_MulU_W() {preStmntStkBin(2, &Info::addFunc_MulU_W);}
      void preStmnt_NegI_W() {preStmntStkUna(2, &Info::addFunc_NegI_W);}
      void preStmnt_ShLF_W() {preStmntStkShi(1, &Info::addFunc_ShLF_W);}
      void preStmnt_ShLU_W();
      void preStmnt_ShRF_W() {preStmntStkShi(1, &Info::addFunc_ShRF_W);}
      void preStmnt_ShRI_W();
      void preStmnt_ShRU_W();
      void preStmnt_SubF_W() {preStmntStkBin(1, &Info::addFunc_SubF_W);}
      void preStmnt_SubI_W() {preStmnt_SubU_W();}
      void preStmnt_SubU_W();

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

      void putExp(IR::Exp const *exp); // Full output.
      void putExp(IR::Exp const *exp, Core::FastU w);

      void putExp_Cst(IR::Exp_Cst const *exp);

      virtual void putFunc();
      using InfoBase::putFunc;

      void putInt(int i);
      void putInt(unsigned int i);
      void putInt(Core::FastU i);
      void putInt(Core::FastI i);

      virtual void putObj();
      using InfoBase::putObj;

      void putValue(IR::Value const &val); // Full output.
      void putValue(IR::Value const &val, Core::FastU w);

      void putValueMulti(Core::Array<IR::Value> const &val); // Full output.
      void putValueMulti(Core::Array<IR::Value> const &val, Core::FastU w);

      virtual void putStmnt();

      void putStmnt_AdXU_W();
      void putStmnt_AddF_W() {putStmntStkBin();}
      void putStmnt_AddI_W() {putStmnt_AddU_W();}
      void putStmnt_AddU_W();
      void putStmnt_BAnd_W(char const *code = "BAnd");
      void putStmnt_BNot_W();
      void putStmnt_BOrI_W() {putStmnt_BAnd_W("BOrI");}
      void putStmnt_BOrX_W() {putStmnt_BAnd_W("BOrX");}
      void putStmnt_Bclo_W() {putStmnt_Bclz_W("Bclo");}
      void putStmnt_Bclz_W(char const *code = "Bclz");
      void putStmnt_Call();
      void putStmnt_CmpF_EQ_W() {putStmntStkCmp(1);}
      void putStmnt_CmpF_GE_W() {putStmntStkCmp(1);}
      void putStmnt_CmpF_GT_W() {putStmntStkCmp(0);}
      void putStmnt_CmpF_LE_W() {putStmntStkCmp(1);}
      void putStmnt_CmpF_LT_W() {putStmntStkCmp(0);}
      void putStmnt_CmpF_NE_W() {putStmntStkCmp(0);}
      void putStmnt_CmpI_EQ_W() {putStmnt_CmpU_EQ_W();}
      void putStmnt_CmpI_GE_W() {putStmntStkCmp(1, "CmpI_GE");}
      void putStmnt_CmpI_GT_W() {putStmntStkCmp(0, "CmpI_GT");}
      void putStmnt_CmpI_LE_W() {putStmntStkCmp(1, "CmpI_LE");}
      void putStmnt_CmpI_LT_W() {putStmntStkCmp(0, "CmpI_LT");}
      void putStmnt_CmpI_NE_W() {putStmnt_CmpU_NE_W();}
      void putStmnt_CmpU_EQ_W(IR::Code code = IR::Code::CmpU_EQ_W);
      void putStmnt_CmpU_GE_W() {putStmntStkCmp(1, "CmpU_GE");}
      void putStmnt_CmpU_GT_W() {putStmntStkCmp(0, "CmpU_GT");}
      void putStmnt_CmpU_LE_W() {putStmntStkCmp(1, "CmpU_LE");}
      void putStmnt_CmpU_LT_W() {putStmntStkCmp(0, "CmpU_LT");}
      void putStmnt_CmpU_NE_W() {putStmnt_CmpU_EQ_W(IR::Code::CmpU_NE_W);}
      void putStmnt_Cnat();
      void putStmnt_Copy_W();
      void putStmnt_DiXI_W() {putStmntStkBin("DiXI");}
      void putStmnt_DiXU_W() {putStmntStkBin("DiXU");}
      void putStmnt_DivF_W() {putStmntStkBin();}
      void putStmnt_DivI_W() {putStmnt_DivU_W("DivI", IR::Code::DiXI_W, false);}
      void putStmnt_DivU_W() {putStmnt_DivU_W("DivU", IR::Code::DiXU_W, false);}
      void putStmnt_Jcnd_Nil(char const *code = "Jcnd_Nil");
      void putStmnt_Jcnd_Tab();
      void putStmnt_Jcnd_Tru();
      void putStmnt_Jfar();
      void putStmnt_Jump();
      void putStmnt_LAnd(char const *code = "LAnd");
      void putStmnt_LNot();
      void putStmnt_LOrI() {putStmnt_LAnd("LOrI");}
      void putStmnt_ModI_W() {putStmnt_DivU_W("ModI", IR::Code::DiXI_W, true);}
      void putStmnt_ModU_W() {putStmnt_DivU_W("ModU", IR::Code::DiXU_W, true);}
      void putStmnt_Move_B();
      void putStmnt_Move_W();
      void putStmnt_MuXU_W() {putStmntStkBin("MuXU");}
      void putStmnt_MulF_W() {putStmntStkBin();}
      void putStmnt_MulI_W() {putStmnt_MulU_W();}
      void putStmnt_MulU_W();
      void putStmnt_NegF_W();
      void putStmnt_NegI_W() {putStmntStkUna("NegI");}
      void putStmnt_Pltn();
      void putStmnt_Retn();
      void putStmnt_ShLF_W() {putStmntStkShi();}
      void putStmnt_ShLU_W(char const *code = "ShLU");
      void putStmnt_ShRF_W() {putStmntStkShi();}
      void putStmnt_ShRI_W() {putStmnt_ShLU_W("ShRI");}
      void putStmnt_ShRU_W() {putStmnt_ShLU_W("ShRU");}
      void putStmnt_SuXU_W();
      void putStmnt_SubF_W() {putStmntStkBin();}
      void putStmnt_SubI_W() {putStmnt_SubU_W();}
      void putStmnt_SubU_W();
      void putStmnt_Swap_W();

      void putStmntCall(Core::String name, Core::FastU argc);

      void putStmntDropArg(IR::Arg const &arg, Core::FastU w);
      void putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntDropTmp(Core::FastU w);
      void putStmntDropTmp(Core::FastU lo, Core::FastU hi);

      void putStmntPushArg(IR::Arg const &arg, Core::FastU w);
      void putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntPushTmp(Core::FastU w);
      void putStmntPushTmp(Core::FastU lo, Core::FastU hi);

      void putStmntStkBin(char const *code = nullptr);
      void putStmntStkCmp(int res0, char const *code = nullptr);
      void putStmntStkShi(char const *code = nullptr);
      void putStmntStkUna(char const *code = nullptr);

      virtual void putStr();
      using InfoBase::putStr;

      void putNTS(char nts);
      void putNTS(char const *nts);
      void putNTS(Core::String nts);

      virtual void trStmnt();
      void trStmnt_AdXU_W();
      void trStmnt_AddF_W() {trStmntStkBin(false);}
      void trStmnt_AddI_W() {trStmnt_AddU_W();}
      void trStmnt_AddU_W();
      void trStmnt_BAnd_W();
      void trStmnt_BNot_W();
      void trStmnt_BOrI_W() {trStmnt_BAnd_W();}
      void trStmnt_BOrX_W() {trStmnt_BAnd_W();}
      void trStmnt_Bclo_W() {trStmnt_Bclz_W();}
      void trStmnt_Bclz_W();
      void trStmnt_Call();
      void trStmnt_CmpF_EQ_W() {trStmntStkCmp(false);}
      void trStmnt_CmpF_GE_W() {trStmntStkCmp(true);}
      void trStmnt_CmpF_GT_W() {trStmntStkCmp(true);}
      void trStmnt_CmpF_LE_W() {trStmntStkCmp(true);}
      void trStmnt_CmpF_LT_W() {trStmntStkCmp(true);}
      void trStmnt_CmpF_NE_W() {trStmntStkCmp(false);}
      void trStmnt_CmpI_EQ_W() {trStmnt_CmpU_EQ_W();}
      void trStmnt_CmpI_GE_W() {trStmntStkCmp(true);}
      void trStmnt_CmpI_GT_W() {trStmntStkCmp(true);}
      void trStmnt_CmpI_LE_W() {trStmntStkCmp(true);}
      void trStmnt_CmpI_LT_W() {trStmntStkCmp(true);}
      void trStmnt_CmpI_NE_W() {trStmnt_CmpU_EQ_W();}
      void trStmnt_CmpU_EQ_W();
      void trStmnt_CmpU_GE_W() {trStmntStkCmp(true);}
      void trStmnt_CmpU_GT_W() {trStmntStkCmp(true);}
      void trStmnt_CmpU_LE_W() {trStmntStkCmp(true);}
      void trStmnt_CmpU_LT_W() {trStmntStkCmp(true);}
      void trStmnt_CmpU_NE_W() {trStmnt_CmpU_EQ_W();}
      void trStmnt_Cnat();
      void trStmnt_Copy_W();
      void trStmnt_DiXI_W() {trStmnt_DiXU_W();}
      void trStmnt_DiXU_W();
      void trStmnt_DivF_W() {trStmntStkBin(true);}
      void trStmnt_DivI_W() {trStmntStkBin(true);}
      void trStmnt_DivU_W() {trStmntStkBin(true);}
      void trStmnt_Jcnd_Nil();
      void trStmnt_Jcnd_Tab();
      void trStmnt_Jcnd_Tru();
      void trStmnt_Jfar();
      void trStmnt_Jump();
      void trStmnt_LAnd();
      void trStmnt_LNot();
      void trStmnt_LOrI() {trStmnt_LAnd();}
      void trStmnt_ModI_W() {trStmnt_ModU_W();}
      void trStmnt_ModU_W();
      void trStmnt_Move_B();
      void trStmnt_Move_W();
      void trStmnt_MuXU_W();
      void trStmnt_MulF_W() {trStmntStkBin(false);}
      void trStmnt_MulI_W() {trStmntStkBin(false);}
      void trStmnt_MulU_W() {trStmntStkBin(false);}
      void trStmnt_NegF_W() {trStmntStkUna();}
      void trStmnt_NegI_W() {trStmntStkUna();}
      void trStmnt_Pltn();
      void trStmnt_Retn();
      void trStmnt_ShLF_W() {trStmntStkShi();}
      void trStmnt_ShLU_W();
      void trStmnt_ShRF_W() {trStmntStkShi();}
      void trStmnt_ShRI_W() {trStmnt_ShLU_W();}
      void trStmnt_ShRU_W() {trStmnt_ShLU_W();}
      void trStmnt_SuXU_W();
      void trStmnt_SubF_W() {trStmntStkBin(true);}
      void trStmnt_SubI_W() {trStmnt_SubU_W();}
      void trStmnt_SubU_W();
      void trStmnt_Swap_W();

      void trStmntStkBin(bool ordered);
      void trStmntStkCmp(bool ordered);
      void trStmntStkShi();
      void trStmntStkUna();

      void trStmntTmp(Core::FastU n);


      static void CheckArg(IR::Arg const &arg, Core::Origin const &pos);

      static Core::FastU GetWord(IR::Arg_Lit const &arg, Core::FastU w = 0);
      static Core::FastU GetWord(IR::Exp const *exp, Core::FastU w = 0);

      static Core::FastU GetWord_Fixed(IR::Value_Fixed const &val, Core::FastU w);

   private:
      void putCodeArg(char const        *arg) {putNTS(arg);}
      void putCodeArg(int                arg) {putInt(arg);}
      void putCodeArg(unsigned int       arg) {putInt(arg);}
      void putCodeArg(Core::FastU        arg) {putInt(arg);}
      void putCodeArg(Core::String       arg);
      void putCodeArg(IR::Arg_Lit const &arg);

      void putStmnt_DivU_W(char const *code, IR::Code codeX, bool mod);
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

