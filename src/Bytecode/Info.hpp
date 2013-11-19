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

#ifndef Bytecode__Info_H__
#define Bytecode__Info_H__

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class AddrBase;
      enum class CallType;

      class Arg;
      struct ArgPtr2;
      class Block;
      class Exp;
      class Function;
      class Glyph;
      class Object;
      class Program;
      class Value;
      struct Value_Multi;
      class Space;
      class Statement;
      class StrEnt;
      class Type;
      struct Type_Fixed;
   }

   struct Origin;
   class String;
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
         out{nullptr},
         prog{nullptr},
         space{nullptr},
         stmnt{nullptr},
         str{nullptr}
      {
      }

      virtual ~Info() {}

      void gen(GDCC::IR::Program &prog);

      void pre(GDCC::IR::Program &prog);

      void put(GDCC::IR::Program &prog, std::ostream &out);

      void tr(GDCC::IR::Program &prog);

   protected:
      virtual void gen();
      virtual void genBlock();
              void genBlock(GDCC::IR::Block &block);
      virtual void genFunc();
              void genFunc(GDCC::IR::Function &func);
      virtual void genSpace() {}
              void genSpace(GDCC::IR::Space &space);
      virtual void genStmnt() {}
              void genStmnt(GDCC::IR::Statement &stmnt);
      virtual void genStr() {}
              void genStr(GDCC::IR::StrEnt &str);

      virtual void pre();
      virtual void preBlock();
              void preBlock(GDCC::IR::Block &block);
      virtual void preFunc();
              void preFunc(GDCC::IR::Function &func);
      virtual void preSpace() {}
              void preSpace(GDCC::IR::Space &space);
      virtual void preStmnt() {}
              void preStmnt(GDCC::IR::Statement &stmnt);
      virtual void preStr() {}
              void preStr(GDCC::IR::StrEnt &str);

      virtual void put() = 0;

      virtual void tr();
      virtual void trBlock();
              void trBlock(GDCC::IR::Block &block);
      virtual void trFunc();
              void trFunc(GDCC::IR::Function &func);
      virtual void trSpace() {}
              void trSpace(GDCC::IR::Space &space);
      virtual void trStmnt() {}
              void trStmnt(GDCC::IR::Statement &stmnt);
      virtual void trStr() {}
              void trStr(GDCC::IR::StrEnt &str);

      GDCC::IR::Block     *block;
      GDCC::IR::Function  *func;
      std::ostream        *out;
      GDCC::IR::Program   *prog;
      GDCC::IR::Space     *space;
      GDCC::IR::Statement *stmnt;
      GDCC::IR::StrEnt    *str;


      static void CheckArgB(GDCC::IR::Arg const &arg, GDCC::IR::AddrBase b, GDCC::Origin pos);
      static void CheckArgB(GDCC::IR::Statement *stmnt, std::size_t a, GDCC::IR::AddrBase b);

      static void CheckArgC(GDCC::IR::Statement *stmnt, std::size_t c);

      static bool IsExp0(GDCC::IR::Exp const *exp);
   };
}

#endif//Bytecode__Info_H__

