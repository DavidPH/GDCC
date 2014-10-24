//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information and handling base class.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Bytecode__Info_H__
#define GDCC__Bytecode__Info_H__

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class Origin;
      class String;
   }

   namespace IR
   {
      enum class AddrBase;
      enum class CallType;
      enum class Code;

      class Arg;
      class ArgPtr1;
      class ArgPtr2;
      class Block;
      class Exp;
      class Function;
      class Glyph;
      class Object;
      class Program;
      class Value;
      class Value_Multi;
      class Space;
      class Statement;
      class StrEnt;
      class Type;
      class Type_Fixed;
   }

   namespace Bytecode
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

         void pre(IR::Program &prog);

         void put(IR::Program &prog, std::ostream &out);

         void tr(IR::Program &prog);

      protected:
         virtual void gen();
         virtual void genBlock();
                 void genBlock(IR::Block &block);
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

         virtual void pre();
         virtual void preBlock();
                 void preBlock(IR::Block &block);
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

         virtual void tr();
         virtual void trBlock();
                 void trBlock(IR::Block &block);
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

         bool moveArgStk_dst(IR::Arg &idx, IR::Code codeMove);
         bool moveArgStk_src(IR::Arg &idx, IR::Code codeMove);

         IR::Block     *block;
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

         static bool IsExp0(IR::Exp const *exp);
      };
   }
}

#endif//GDCC__Bytecode__Info_H__

