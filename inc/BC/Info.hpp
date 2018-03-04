//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
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

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      template<typename T> class Array;
      class Origin;
      class String;
   }

   namespace IR
   {
      enum class AddrBase;
      enum class CallType;
      enum class Code;

      class Arg;
      class Arg_Lit;
      class ArgPtr1;
      class ArgPtr2;
      class Block;
      class DJump;
      class Exp;
      class Exp_Array;
      class Exp_Assoc;
      class Exp_Tuple;
      class Exp_Union;
      class Function;
      class Glyph;
      class Object;
      class OpCode;
      class Program;
      class Value;
      class Value_Array;
      class Value_Fixed;
      class Value_Float;
      class Value_Funct;
      class Value_StrEn;
      class Value_Tuple;
      class Space;
      class Statement;
      class StrEnt;
      class Type;
      class Type_Assoc;
      class Type_Fixed;
      class Type_Point;
      class Type_Tuple;
      class Type_Union;
   }

   namespace BC
   {
      class Info;

      typedef Info InfoBase;

      //
      // FixedInfo
      //
      class FixedInfo
      {
      public:
         Core::FastU bitsF;
         Core::FastU bitsI;
         Core::FastU bitsS;

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
            str{nullptr},
            putPos{0}
         {
         }

         virtual ~Info() {}

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
         virtual void genStr() {}
                 void genStr(IR::StrEnt &str);

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
         virtual void optStr() {}
                 void optStr(IR::StrEnt &str);

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
         virtual void preStr() {}
                 void preStr(IR::StrEnt &str);

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
         virtual void putStr() {}
                 void putStr(IR::StrEnt &str);

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
         virtual void trStr() {}
                 void trStr(IR::StrEnt &str);

         void addFunc(Core::String name, Core::FastU retrn, Core::FastU param);

         void addFunc_AddF_W(Core::FastU n);
         void addFunc_AddU_W(Core::FastU n);
         void addFunc_Bclo_W(Core::FastU n);
         void addFunc_Bclz_W(Core::FastU n);
         void addFunc_CmpF_EQ_W(Core::FastU n);
         void addFunc_CmpF_GE_W(Core::FastU n);
         void addFunc_CmpF_GT_W(Core::FastU n);
         void addFunc_CmpF_LE_W(Core::FastU n);
         void addFunc_CmpF_LT_W(Core::FastU n);
         void addFunc_CmpF_NE_W(Core::FastU n);
         void addFunc_CmpI_GE_W(Core::FastU n);
         void addFunc_CmpI_GT_W(Core::FastU n);
         void addFunc_CmpI_LE_W(Core::FastU n);
         void addFunc_CmpI_LT_W(Core::FastU n);
         void addFunc_CmpU_EQ_W(Core::FastU n);
         void addFunc_CmpU_GE_W(Core::FastU n);
         void addFunc_CmpU_GT_W(Core::FastU n);
         void addFunc_CmpU_LE_W(Core::FastU n);
         void addFunc_CmpU_LT_W(Core::FastU n);
         void addFunc_CmpU_NE_W(Core::FastU n);
         void addFunc_DiXI_W(Core::FastU n);
         void addFunc_DiXU_W(Core::FastU n);
         void addFunc_DivF_W(Core::FastU n);
         void addFunc_DivK_W(Core::FastU n);
         void addFunc_DivX_W(Core::FastU n);
         void addFunc_MuXU_W(Core::FastU n);
         void addFunc_MulF_W(Core::FastU n);
         void addFunc_MulK_W(Core::FastU n);
         void addFunc_MulU_W(Core::FastU n);
         void addFunc_MulX_W(Core::FastU n);
         void addFunc_NegI_W(Core::FastU n);
         void addFunc_ShLF_W(Core::FastU n);
         void addFunc_ShLU_W(Core::FastU n);
         void addFunc_ShRF_W(Core::FastU n);
         void addFunc_ShRI_W(Core::FastU n);
         void addFunc_ShRU_W(Core::FastU n);
         void addFunc_SubF_W(Core::FastU n);
         void addFunc_SubU_W(Core::FastU n);

         virtual FixedInfo getFixedInfo(Core::FastU n, bool s);

         // Default behavior is to assume IEEE float layout and 32-bit word.
         virtual FloatInfo getFloatInfo(Core::FastU n);

         IR::Function *getFuncDefn(Core::String name, Core::FastU retrn,
            Core::FastU param, Core::FastU localReg, char const *file);

         Core::String getFuncName(IR::Code code, Core::FastU n);

         Core::FastU getWord(IR::Arg_Lit const &arg, Core::FastU w = 0);
         Core::FastU getWord(IR::Exp const *exp, Core::FastU w = 0);
         virtual Core::FastU getWord(Core::Origin pos, IR::Value const &val, Core::FastU w = 0);
         virtual Core::FastU getWord_Fixed(IR::Value_Fixed const &val, Core::FastU w);
         virtual Core::FastU getWord_Float(IR::Value_Float const &val, Core::FastU w);

         Core::FastU getWordCount(IR::Type const &type);
         Core::FastU getWordCount_Assoc(IR::Type_Assoc const &type);
         virtual Core::FastU getWordCount_Point(IR::Type_Point const &type);
         Core::FastU getWordCount_Tuple(IR::Type_Tuple const &type);
         Core::FastU getWordCount_Union(IR::Type_Union const &type);

         WordArray getWords(IR::Arg_Lit const &arg);
         WordArray getWords(IR::Exp const *exp);
         WordArray getWords(Core::Origin pos, IR::Value const &val);
         WordArray getWords_Array(IR::Exp_Array const *exp);
         WordArray getWords_Assoc(IR::Exp_Assoc const *exp);
         WordArray getWords_Tuple(IR::Exp_Tuple const *exp);
         WordArray getWords_Union(IR::Exp_Union const *exp);

         void putData(char const *data, std::size_t size);

         void moveArgStk_dst(IR::Arg &idx);
         void moveArgStk_src(IR::Arg &idx);
         void moveArgStkB_dst(IR::Arg &idx);
         void moveArgStkB_src(IR::Arg &idx);

         bool optStmnt_Cspe_Drop();

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
         IR::StrEnt    *str;
         std::size_t   putPos;


         static void CheckArgB(IR::Arg const &arg, IR::AddrBase b, Core::Origin pos);
         static void CheckArgB(IR::Statement *stmnt, std::size_t a, IR::AddrBase b);

         static void CheckArgC(IR::Statement *stmnt, std::size_t c);

      private:
         void addFunc_AddU_W(Core::FastU n, IR::Code codeAdd, IR::Code codeAdX);
         void addFunc_Bclz_W(Core::FastU n, IR::Code code, Core::FastU skip);
         void addFunc_CmpF_W(Core::FastU n, IR::Code code, IR::Code codePos, IR::Code codeNeg);
         void addFunc_CmpU_EQ_W(Core::FastU n, IR::Code codeCmp, IR::Code codeAnd);
         void addFunc_CmpU_GE_W(Core::FastU n, IR::Code codeCmpHi, IR::Code codeCmpLo);
         void addFunc_DivX_W(Core::FastU n, IR::Code code, IR::Code codeDiv, bool sign);
         void addFunc_ShLF_W(Core::FastU n, IR::Code code, bool left);
         void addFunc_ShLU_W(Core::FastU n, IR::Code code, bool left, bool sign);
      };
   }
}

#endif//GDCC__BC__Info_H__

