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
      class Type_Fixed;
   }

   namespace BC
   {
      class Info;

      typedef Info InfoBase;

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
         class ResetFunc {};
         class ResetStmnt {};


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

         void putData(char const *data, std::size_t size);

         void moveArgStk_dst(IR::Arg &idx, Core::FastU sizeMove);
         void moveArgStk_src(IR::Arg &idx, Core::FastU sizeMove);

         bool optStmnt_Cspe_Drop();

         void trStmntStk2(Core::FastU sizeDst, Core::FastU sizeSrc);
         void trStmntStk3(Core::FastU sizeDst, Core::FastU sizeSrc, bool ordered);

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
      };
   }
}

#endif//GDCC__BC__Info_H__

