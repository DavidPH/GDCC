//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information and handling base class.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__Info_H__
#define GDCC__BC__Info_H__

#include "../BC/Types.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_BC_CodeTypeCases
//
#define GDCC_BC_CodeTypeCases(phase, name) \
   case 'A': phase##Stmnt_##name##_A(); break; \
 /*case 'E': phase##Stmnt_##name##_E(); break;*/ \
   case 'F': phase##Stmnt_##name##_F(); break; \
   case 'I': phase##Stmnt_##name##_I(); break; \
   case 'K': phase##Stmnt_##name##_K(); break; \
   case 'R': phase##Stmnt_##name##_R(); break; \
   case 'U': phase##Stmnt_##name##_U(); break; \
   case 'X': phase##Stmnt_##name##_X(); break

//
// GDCC_BC_CodeTypeCasesFIU
//
#define GDCC_BC_CodeTypeCasesFIU(phase, name) \
   case 'A': phase##Stmnt_##name##_U(); break; \
   case 'F': phase##Stmnt_##name##_F(); break; \
   case 'I': phase##Stmnt_##name##_I(); break; \
   case 'K': phase##Stmnt_##name##_U(); break; \
   case 'R': phase##Stmnt_##name##_I(); break; \
   case 'U': phase##Stmnt_##name##_U(); break; \
   case 'X': phase##Stmnt_##name##_I(); break

//
// GDCC_BC_CodeTypeCasesIU
//
#define GDCC_BC_CodeTypeCasesIU(phase, name) \
   case 'A': phase##Stmnt_##name##_U(); break; \
   case 'I': phase##Stmnt_##name##_I(); break; \
   case 'K': phase##Stmnt_##name##_U(); break; \
   case 'R': phase##Stmnt_##name##_I(); break; \
   case 'U': phase##Stmnt_##name##_U(); break; \
   case 'X': phase##Stmnt_##name##_I(); break

//
// GDCC_BC_CodeTypeCasesIUx
//
#define GDCC_BC_CodeTypeCasesIUx(phase, name) \
   case 'I': phase##Stmnt_##name##_I(); break; \
   case 'U': phase##Stmnt_##name##_U(); break

//
// GDCC_BC_CodeTypeCasesUx
//
#define GDCC_BC_CodeTypeCasesUx(phase, name) \
   case 'U': phase##Stmnt_##name##_U(); break

//
// GDCC_BC_CodeTypeSwitch
//
#define GDCC_BC_CodeTypeSwitch(phase, name, cases) \
   if(!stmnt->code.type[0]) \
      phase##Stmnt_##name##_U(); \
   \
   else if(!stmnt->code.type[1]) switch(stmnt->code.type[0]) \
   { \
      GDCC_BC_CodeTypeCases##cases(phase, name); \
   default: errorCode("unsupported " #phase); \
   } \
   else \
      errorCode("unsupported " #phase);

//
// GDCC_BC_CodeTypeSwitchFn
//
#define GDCC_BC_CodeTypeSwitchFn(phase, name, cases) \
   void Info::phase##Stmnt_##name() \
   { \
      GDCC_BC_CodeTypeSwitch(phase, name, cases); \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::BC
{
   //
   // FixedInfo
   //
   class FixedInfo
   {
   public:
      Core::FastU bitsF;
      Core::FastU bitsI;
      Core::FastU bitsS;

      Core::FastU words;
      Core::FastU wordsF;
      Core::FastU wordsI;
   };

   //
   // FloatInfo
   //
   class FloatInfo
   {
   public:
      Core::FastU bitsExp;
      Core::FastU bitsMan;
      Core::FastU bitsManFull;
      Core::FastU bitsSig;

      Core::FastU maskExp;
      Core::FastU maskMan;
      Core::FastU maskSig;

      Core::FastU maxExp;

      Core::FastU offExp;

      Core::FastU words;
      Core::FastU wordsExp;
      Core::FastU wordsMan;
   };

   //
   // Info
   //
   class Info
   {
   public:
      Info() :
         block{nullptr},
         func{nullptr},
         obj{nullptr},
         out{nullptr},
         prog{nullptr},
         space{nullptr},
         stmnt{nullptr},
         strent{nullptr},
         putPos{0}
      {
      }

      virtual ~Info() {}

      void chk(IR::Program &prog);

      void gen(IR::Program &prog);

      void opt(IR::Program &prog);

      void pre(IR::Program &prog);

      void put(IR::Program &prog, std::ostream &out);

      void putExtra(IR::Program &prog);

      void tr(IR::Program &prog);

   protected:
      using AddFunc = void (Info::*)(Core::FastU);
      using IRExpCPtr = Core::CounterPtr<IR::Exp const>;

      class ResetFunc {};
      class ResetStmnt {};

      class WordValue
      {
      public:
         IRExpCPtr   exp;
         Core::FastU val;
      };

      using WordArray = Core::Array<WordValue>;


      virtual void chk();
      virtual void chkBlock();
              void chkBlock(IR::Block &block);
      virtual void chkDJump() {}
              void chkDJump(IR::DJump &djump);
      virtual void chkFunc();
              void chkFunc(IR::Function &func);
      virtual void chkObj() {}
              void chkObj(IR::Object &obj);
      virtual void chkSpace() {}
              void chkSpace(IR::Space &space);
      virtual void chkStmnt();
              void chkStmnt(IR::Statement &stmnt);
      virtual void chkStrEnt() {}
              void chkStrEnt(IR::StrEnt &strent);

      virtual void gen();
      virtual void genBlock();
              void genBlock(IR::Block &block);
      virtual void genDJump() {}
              void genDJump(IR::DJump &djump);
      virtual void genFunc();
              void genFunc(IR::Function &func);
      virtual void genObj() {}
              void genObj(IR::Object &obj);
      virtual void genSpace() {}
              void genSpace(IR::Space &space);
      virtual void genStmnt() {}
              void genStmnt(IR::Statement &stmnt);
      virtual void genStrEnt() {}
              void genStrEnt(IR::StrEnt &strent);

      virtual void opt();
      virtual void optBlock();
              void optBlock(IR::Block &block);
      virtual void optDJump() {}
              void optDJump(IR::DJump &djump);
      virtual void optFunc();
              void optFunc(IR::Function &func);
      virtual void optObj() {}
              void optObj(IR::Object &obj);
      virtual void optSpace() {}
              void optSpace(IR::Space &space);
      virtual void optStmnt();
              void optStmnt(IR::Statement &stmnt);
      virtual void optStrEnt() {}
              void optStrEnt(IR::StrEnt &strent);

      virtual void pre();
      virtual void preBlock();
              void preBlock(IR::Block &block);
      virtual void preDJump() {}
              void preDJump(IR::DJump &djump);
      virtual void preFunc();
              void preFunc(IR::Function &func);
      virtual void preObj() {}
              void preObj(IR::Object &obj);
      virtual void preSpace() {}
              void preSpace(IR::Space &space);
      virtual void preStmnt() {}
              void preStmnt(IR::Statement &stmnt);
      virtual void preStrEnt() {}
              void preStrEnt(IR::StrEnt &strent);

      virtual void put() = 0;
      virtual void putBlock();
              void putBlock(IR::Block &block);
      virtual void putDJump() {}
              void putDJump(IR::DJump &djump);
      virtual void putFunc();
              void putFunc(IR::Function &func);
      virtual void putObj() {}
              void putObj(IR::Object &obj);
      virtual void putSpace() {}
              void putSpace(IR::Space &space);
      virtual void putStmnt() {}
              void putStmnt(IR::Statement &stmnt);
      virtual void putStrEnt() {}
              void putStrEnt(IR::StrEnt &strent);

      virtual void putExtra() {}

      virtual void tr();
      virtual void trBlock();
              void trBlock(IR::Block &block);
      virtual void trDJump() {}
              void trDJump(IR::DJump &djump);
      virtual void trFunc();
              void trFunc(IR::Function &func);
      virtual void trObj() {}
              void trObj(IR::Object &obj);
      virtual void trSpace() {}
              void trSpace(IR::Space &space);
      virtual void trStmnt() {}
              void trStmnt(IR::Statement &stmnt);
      virtual void trStrEnt() {}
              void trStrEnt(IR::StrEnt &strent);

      void addFunc(Core::String name, Core::FastU retrn, Core::FastU param);

      void addFunc_Add_FW(Core::FastU n);
      void addFunc_Add_UW(Core::FastU n);
      void addFunc_Bclo_W(Core::FastU n);
      void addFunc_Bclz_W(Core::FastU n);
      void addFunc_CmpEQ_FW(Core::FastU n);
      void addFunc_CmpGE_FW(Core::FastU n);
      void addFunc_CmpGT_FW(Core::FastU n);
      void addFunc_CmpLE_FW(Core::FastU n);
      void addFunc_CmpLT_FW(Core::FastU n);
      void addFunc_CmpNE_FW(Core::FastU n);
      void addFunc_CmpGE_IW(Core::FastU n);
      void addFunc_CmpGT_IW(Core::FastU n);
      void addFunc_CmpLE_IW(Core::FastU n);
      void addFunc_CmpLT_IW(Core::FastU n);
      void addFunc_CmpEQ_UW(Core::FastU n);
      void addFunc_CmpGE_UW(Core::FastU n);
      void addFunc_CmpGT_UW(Core::FastU n);
      void addFunc_CmpLE_UW(Core::FastU n);
      void addFunc_CmpLT_UW(Core::FastU n);
      void addFunc_CmpNE_UW(Core::FastU n);
      void addFunc_Div_AW(Core::FastU n);
      void addFunc_Div_FW(Core::FastU n);
      void addFunc_Div_KW(Core::FastU n);
      void addFunc_Div_RW(Core::FastU n);
      void addFunc_Div_XW(Core::FastU n);
      void addFunc_DivX_IW(Core::FastU n);
      void addFunc_DivX_UW(Core::FastU n);
      void addFunc_Mul_AW(Core::FastU n);
      void addFunc_Mul_FW(Core::FastU n);
      void addFunc_Mul_KW(Core::FastU n);
      void addFunc_Mul_RW(Core::FastU n);
      void addFunc_Mul_UW(Core::FastU n);
      void addFunc_Mul_XW(Core::FastU n);
      void addFunc_MulX_UW(Core::FastU n);
      void addFunc_Neg_IW(Core::FastU n);
      void addFunc_ShL_FW(Core::FastU n);
      void addFunc_ShL_UW(Core::FastU n);
      void addFunc_ShR_FW(Core::FastU n);
      void addFunc_ShR_IW(Core::FastU n);
      void addFunc_ShR_UW(Core::FastU n);
      void addFunc_Sub_FW(Core::FastU n);
      void addFunc_Sub_UW(Core::FastU n);
      void addFunc_Tr_W(IR::CodeType type, Core::FastU dstN, Core::FastU srcN);

      virtual void chkStmntArg(IR::Arg const &arg);

      void chkStmntArgA(IR::ArgPtr2 const &arg, IR::ArgBase a);

      void chkStmntArgB(Core::FastU arg, IR::ArgBase a);

      void chkStmntArgC(Core::FastU n) {chkStmntArgC(n, n);}
      void chkStmntArgC(Core::FastU min, Core::FastU max);

      void chkStmntArgI(IR::ArgPtr1 const &arg, IR::ArgBase a);
      void chkStmntArgI(IR::ArgPtr2 const &arg, IR::ArgBase a);

      [[noreturn]]
      void errorCode(char const *msg);

      Core::CounterRef<IR::Exp const> getExpAddPtr(IR::Exp const *l, Core::FastU r);
      Core::CounterRef<IR::Exp const> getExpGlyph(Core::String glyph);

      virtual FixedInfo getFixedInfo(Core::FastU n, bool s);
      FixedInfo getFixedInfo(Core::FastU n, char t);

      // Default behavior is to assume IEEE float layout and 32-bit word.
      virtual FloatInfo getFloatInfo(Core::FastU n, bool s);
      FloatInfo getFloatInfo(Core::FastU n, char t);

      virtual FixedInfo getFractInfo(Core::FastU n, bool s);

      IR::Function *getFuncDefn(Core::String name, Core::FastU retrn,
         Core::FastU param, Core::FastU localReg, char const *file);

      Core::String getFuncName(IR::Code code, Core::FastU n0, Core::FastU n1 = 0);

      virtual FixedInfo getIntegInfo(Core::FastU n, bool s);

      Core::Origin getOrigin();

      virtual Core::FastU getStmntSize();

      Core::FastU getWord(IR::Arg_Lit const &arg, Core::FastU w = 0);
      Core::FastU getWord(IR::Exp const *exp, Core::FastU w = 0);
      virtual Core::FastU getWord(Core::Origin pos, IR::Value const &val, Core::FastU w = 0);
      Core::FastU getWord_Array(Core::Origin pos, IR::Value_Array const &val, Core::FastU w);
      Core::FastU getWord_Assoc(Core::Origin pos, IR::Value_Assoc const &val, Core::FastU w);
      virtual Core::FastU getWord_Fixed(IR::Value_Fixed const &val, Core::FastU w);
      virtual Core::FastU getWord_Float(IR::Value_Float const &val, Core::FastU w);

      Core::FastU getWordCount(IR::Type const &type);
      Core::FastU getWordCount_Assoc(IR::Type_Assoc const &type);
      virtual Core::FastU getWordCount_Point(IR::Type_Point const &type);
      Core::FastU getWordCount_Tuple(IR::Type_Tuple const &type);
      Core::FastU getWordCount_Union(IR::Type_Union const &type);

      IR::TypeBase getWordType(IR::Type const &type, Core::FastU w = 0);
      IR::TypeBase getWordType_Array(IR::Type_Array const &type, Core::FastU w);
      IR::TypeBase getWordType_Assoc(IR::Type_Assoc const &type, Core::FastU w);
      virtual IR::TypeBase getWordType_Funct(IR::Type_Funct const &type, Core::FastU w);
      virtual IR::TypeBase getWordType_StrEn(IR::Type_StrEn const &type, Core::FastU w);

      WordArray getWords(IR::Arg_Lit const &arg);
      WordArray getWords(IR::Exp const *exp);
      WordArray getWords(Core::Origin pos, IR::Value const &val);
      WordArray getWords_Array(IR::Exp_Array const *exp);
      WordArray getWords_Assoc(IR::Exp_Assoc const *exp);
      WordArray getWords_Tuple(IR::Exp_Tuple const *exp);
      WordArray getWords_Union(IR::Exp_Union const *exp);

      void putData(char const *data, std::size_t size);

      void moveArgStk_dst(IR::Arg &idx);
      void moveArgStk_src(IR::Arg &idx, bool swap = false);

      bool optStmnt_Cspe_Drop();
      bool optStmnt_JumpNext();
      bool optStmnt_LNot_Jcnd();

      void trStmntStk2();
      void trStmntStk3(bool ordered);
      bool trStmntShift(bool moveLit);

      IR::Block     *block;
      IR::DJump     *djump;
      IR::Function  *func;
      IR::Object    *obj;
      std::ostream  *out;
      IR::Program   *prog;
      IR::Space     *space;
      IR::Statement *stmnt;
      IR::StrEnt    *strent;
      std::size_t    putPos;

   private:
      void addFunc_Add_UW(Core::FastU n, IR::Code codeAdd, IR::Code codeAdX);
      void addFunc_Bclz_W(Core::FastU n, IR::Code code, Core::FastU skip);
      void addFunc_Cmp_FW(Core::FastU n, IR::Code code, IR::Code codePos, IR::Code codeNeg);
      void addFunc_CmpEQ_UW(Core::FastU n, IR::Code codeCmp, IR::Code codeAnd);
      void addFunc_CmpGE_UW(Core::FastU n, IR::Code codeCmpHi, IR::Code codeCmpLo);
      void addFunc_Div_XW(Core::FastU n, IR::Code code, IR::Code codeDiv, bool sign);
      void addFunc_ShL_FW(Core::FastU n, IR::Code code, bool left);
      void addFunc_ShL_UW(Core::FastU n, IR::Code code, bool left, bool sign);
      void addFunc_Tr_W(IR::CodeType type, FixedInfo dstFI, FixedInfo srcFI);
      void addFunc_Tr_W(IR::CodeType type, FixedInfo dstFI, FloatInfo srcFI);
      void addFunc_Tr_W(IR::CodeType type, FloatInfo dstFI, FixedInfo srcFI);
      void addFunc_Tr_W(IR::CodeType type, FloatInfo dstFI, FloatInfo srcFI);
   };
}

#endif//GDCC__BC__Info_H__

