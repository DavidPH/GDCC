//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly macros.
//
//-----------------------------------------------------------------------------

#ifndef Asm__Macro_H__
#define Asm__Macro_H__

#include "GDCC/IR/Block.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Asm
{
   //
   // Macro
   //
   class Macro
   {
   public:
      Macro(GDCC::String, GDCC::IR::Block &&list_) : list{std::move(list_)} {}

      void expand(GDCC::IR::Block &block, GDCC::IR::Arg const *argv,
         std::size_t argc) const;


      static void Add(GDCC::String name, GDCC::IR::Block &&list);

      static Macro const *Find(GDCC::String name);

   private:
      GDCC::IR::Block list;
   };
}

#endif//Asm__Macro_H__

