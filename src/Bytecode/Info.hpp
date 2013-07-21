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
      class Block;
      class Exp;
      class Function;
      class Glyph;
      class Value;
      class Statement;
   }
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
      virtual ~Info() {}

      virtual void put(std::ostream &out) = 0;

      virtual void translate();
      virtual void translateBlock(GDCC::IR::Block &block) = 0;
   };
}

#endif//Bytecode__Info_H__

