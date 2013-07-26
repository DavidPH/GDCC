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

      class Arg;
      class Block;
      class Exp;
      class Function;
      class Glyph;
      class Value;
      class Statement;
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
      Info() : curFunc{nullptr} {}
      virtual ~Info() {}

      virtual void put(std::ostream &out) = 0;

      virtual void translate();
      virtual void translateBlock(GDCC::IR::Block &block);
      virtual void translateFunction(GDCC::IR::Function &func);
      virtual void translateStatement(GDCC::IR::Statement &stmnt) = 0;

   protected:
      GDCC::IR::Function *curFunc;


      static void CheckArgB(GDCC::IR::Arg const &arg, GDCC::IR::AddrBase b, GDCC::Origin pos);
      static void CheckArgB(GDCC::IR::Statement &stmnt, std::size_t a, GDCC::IR::AddrBase b);

      static void CheckArgC(GDCC::IR::Statement &stmnt, std::size_t c);

      static bool IsExp0(GDCC::IR::Exp const *exp);
   };
}

#endif//Bytecode__Info_H__

