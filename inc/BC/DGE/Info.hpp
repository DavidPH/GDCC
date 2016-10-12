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

      virtual void preStmnt();

      void preStmnt_AddU_W();
      void preStmnt_CmpI_EQ_W() {preStmnt_CmpU_EQ_W();}
      void preStmnt_CmpI_GE_W() {preStmnt_CmpU_GE_W(&Info::addFunc_CmpI_GE_W);}
      void preStmnt_CmpI_GT_W() {preStmnt_CmpU_GE_W(&Info::addFunc_CmpI_GT_W);}
      void preStmnt_CmpI_LE_W() {preStmnt_CmpU_GE_W(&Info::addFunc_CmpI_LE_W);}
      void preStmnt_CmpI_LT_W() {preStmnt_CmpU_GE_W(&Info::addFunc_CmpI_LT_W);}
      void preStmnt_CmpI_NE_W() {preStmnt_CmpU_NE_W();}
      void preStmnt_CmpU_EQ_W(void (Info::*fn)(Core::FastU) = &Info::addFunc_CmpU_EQ_W);
      void preStmnt_CmpU_GE_W(void (Info::*fn)(Core::FastU) = &Info::addFunc_CmpU_GE_W);
      void preStmnt_CmpU_GT_W() {preStmnt_CmpU_GE_W(&Info::addFunc_CmpU_GT_W);}
      void preStmnt_CmpU_LE_W() {preStmnt_CmpU_GE_W(&Info::addFunc_CmpU_LE_W);}
      void preStmnt_CmpU_LT_W() {preStmnt_CmpU_GE_W(&Info::addFunc_CmpU_LT_W);}
      void preStmnt_CmpU_NE_W() {preStmnt_CmpU_EQ_W(&Info::addFunc_CmpU_NE_W);}
      void preStmnt_MuXU_W();
      void preStmnt_MulU_W();
      void preStmnt_SubU_W();

      virtual void put();

      void putCode(char const *code);
      void putCode(char const *code, char const *arg0);
      void putCode(char const *code, int arg0);
      void putCode(char const *code, Core::FastU arg0);
      void putCode(char const *code, Core::FastU arg0, IR::Arg_Lit const &arg1);
      void putCode(char const *code, Core::String arg0);
      void putCode(char const *code, IR::Arg_Lit const &arg0);

      void putExp(IR::Exp const *exp); // Full output.
      void putExp(IR::Exp const *exp, Core::FastU w);

      void putExp_Cst(IR::Exp_Cst const *exp);

      virtual void putFunc();
      using InfoBase::putFunc;

      void putInt(int i);
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
      void putStmnt_AddU_W();
      void putStmnt_AndU_W(char const *code = "AndU");
      void putStmnt_Call();
      void putStmnt_CmpI_EQ_W() {putStmnt_CmpU_EQ_W();}
      void putStmnt_CmpI_GE_W() {putStmnt_CmpU_GE_W("CmpI_GE");}
      void putStmnt_CmpI_GT_W() {putStmnt_CmpU_GE_W("CmpI_GT");}
      void putStmnt_CmpI_LE_W() {putStmnt_CmpU_GE_W("CmpI_LE");}
      void putStmnt_CmpI_LT_W() {putStmnt_CmpU_GE_W("CmpI_LT");}
      void putStmnt_CmpI_NE_W() {putStmnt_CmpU_NE_W();}
      void putStmnt_CmpU_EQ_W(IR::Code code = IR::Code::CmpU_EQ_W);
      void putStmnt_CmpU_GE_W(char const *code = "CmpU_GE");
      void putStmnt_CmpU_GT_W() {putStmnt_CmpU_GE_W("CmpI_GT");}
      void putStmnt_CmpU_LE_W() {putStmnt_CmpU_GE_W("CmpI_LE");}
      void putStmnt_CmpU_LT_W() {putStmnt_CmpU_GE_W("CmpI_LT");}
      void putStmnt_CmpU_NE_W() {putStmnt_CmpU_EQ_W(IR::Code::CmpU_NE_W);}
      void putStmnt_Cnat();
      void putStmnt_Jcnd_Nil(char const *code = "Jcnd_Nil");
      void putStmnt_Jcnd_Tru();
      void putStmnt_Jfar();
      void putStmnt_Jump();
      void putStmnt_LNot();
      void putStmnt_Move_B();
      void putStmnt_Move_W();
      void putStmnt_MuXU_W();
      void putStmnt_MulU_W();
      void putStmnt_OrIU_W();
      void putStmnt_OrXU_W();
      void putStmnt_Retn();
      void putStmnt_SuXU_W();
      void putStmnt_SubU_W();

      void putStmntCall(Core::String name, Core::FastU argc);

      void putStmntDropArg(IR::Arg const &arg, Core::FastU w);
      void putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntPushArg(IR::Arg const &arg, Core::FastU w);
      void putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      virtual void putStr();
      using InfoBase::putStr;

      void putNTS(char nts);
      void putNTS(char const *nts);
      void putNTS(Core::String nts);

      virtual void trStmnt();
      void trStmnt_AdXU_W();
      void trStmnt_AddU_W();
      void trStmnt_AndU_W();
      void trStmnt_Call();
      void trStmnt_CmpI_EQ_W() {trStmnt_CmpU_EQ_W();}
      void trStmnt_CmpI_GE_W() {trStmnt_CmpU_GE_W();}
      void trStmnt_CmpI_GT_W() {trStmnt_CmpU_GE_W();}
      void trStmnt_CmpI_LE_W() {trStmnt_CmpU_GE_W();}
      void trStmnt_CmpI_LT_W() {trStmnt_CmpU_GE_W();}
      void trStmnt_CmpI_NE_W() {trStmnt_CmpU_EQ_W();}
      void trStmnt_CmpU_EQ_W();
      void trStmnt_CmpU_GE_W();
      void trStmnt_CmpU_GT_W() {trStmnt_CmpU_GE_W();}
      void trStmnt_CmpU_LE_W() {trStmnt_CmpU_GE_W();}
      void trStmnt_CmpU_LT_W() {trStmnt_CmpU_GE_W();}
      void trStmnt_CmpU_NE_W() {trStmnt_CmpU_EQ_W();}
      void trStmnt_Cnat();
      void trStmnt_Jcnd_Nil();
      void trStmnt_Jcnd_Tru();
      void trStmnt_Jfar();
      void trStmnt_Jump();
      void trStmnt_LNot();
      void trStmnt_Move_B();
      void trStmnt_Move_W();
      void trStmnt_MuXU_W();
      void trStmnt_MulU_W();
      void trStmnt_OrIU_W();
      void trStmnt_OrXU_W();
      void trStmnt_Retn();
      void trStmnt_SuXU_W();
      void trStmnt_SubU_W();


      static void CheckArg(IR::Arg const &arg, Core::Origin const &pos);

      static Core::FastU GetWord(IR::Arg_Lit const &arg, Core::FastU w = 0);
      static Core::FastU GetWord(IR::Exp const *exp, Core::FastU w = 0);

      static Core::FastU GetWord_Fixed(IR::Value_Fixed const &val, Core::FastU w);
   };
}

#endif//GDCC__BC__DGE__Info_H__

