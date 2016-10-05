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

      virtual void put();

      void putCode(char const *code);
      void putCode(char const *code, char const *arg0);
      void putCode(char const *code, Core::FastU arg0);
      void putCode(char const *code, Core::FastU arg0, IR::Arg_Lit const &arg1);
      void putCode(char const *code, IR::Arg_Lit const &arg0);

      void putExp(IR::Exp const *exp); // Full output.
      void putExp(IR::Exp const *exp, Core::FastU w);

      void putExp_Cst(IR::Exp_Cst const *exp);

      virtual void putFunc();
      using InfoBase::putFunc;

      void putInt(Core::FastU i);
      void putInt(Core::FastI i);

      virtual void putObj();
      using InfoBase::putObj;

      void putValue(IR::Value const &val); // Full output.
      void putValue(IR::Value const &val, Core::FastU w);

      void putValueMulti(Core::Array<IR::Value> const &val); // Full output.
      void putValueMulti(Core::Array<IR::Value> const &val, Core::FastU w);

      virtual void putStmnt();
      void putStmnt_Call();
      void putStmnt_Cnat();
      void putStmnt_Jfar();
      void putStmnt_Jump();
      void putStmnt_Move_B();
      void putStmnt_Move_W();
      void putStmnt_Retn();

      void putStmntDropArg(IR::Arg const &arg, Core::FastU w);
      void putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putStmntPushArg(IR::Arg const &arg, Core::FastU w);
      void putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

      void putNTS(char nts);
      void putNTS(char const *nts);
      void putNTS(Core::String nts);

      virtual void trStmnt();
      void trStmnt_Call();
      void trStmnt_Cnat();
      void trStmnt_Jfar();
      void trStmnt_Jump();
      void trStmnt_Move_B();
      void trStmnt_Move_W();
      void trStmnt_Retn();


      static void CheckArg(IR::Arg const &arg, Core::Origin const &pos);

      static Core::FastU GetWord(IR::Arg_Lit const &arg, Core::FastU w = 0);
      static Core::FastU GetWord(IR::Exp const *exp, Core::FastU w = 0);

      static Core::FastU GetWord_Fixed(IR::Value_Fixed const &val, Core::FastU w);
   };
}

#endif//GDCC__BC__DGE__Info_H__

