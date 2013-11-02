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
      Info() : func{nullptr}, out{nullptr} {}
      virtual ~Info() {}

      virtual void gen();

      virtual void put(std::ostream &out) = 0;

      virtual void tr();

   protected:
      virtual void genBlock(GDCC::IR::Block &block);
      virtual void genFunc(GDCC::IR::Function &func);
      virtual void genSpace(GDCC::IR::Space &space);
      virtual void genStmnt();
      virtual void genStr(GDCC::IR::StrEnt &str);

      virtual void trBlock(GDCC::IR::Block &block);
      virtual void trFunc(GDCC::IR::Function &func);
      virtual void trSpace(GDCC::IR::Space &space);
      virtual void trStmnt() = 0;
      virtual void trStr(GDCC::IR::StrEnt &str);

      GDCC::IR::Block     *block;
      GDCC::IR::Function  *func;
      std::ostream        *out;
      GDCC::IR::Statement *stmnt;


      static void CheckArgB(GDCC::IR::Arg const &arg, GDCC::IR::AddrBase b, GDCC::Origin pos);
      static void CheckArgB(GDCC::IR::Statement *stmnt, std::size_t a, GDCC::IR::AddrBase b);

      static void CheckArgC(GDCC::IR::Statement *stmnt, std::size_t c);

      static bool IsExp0(GDCC::IR::Exp const *exp);
   };
}

#endif//Bytecode__Info_H__

