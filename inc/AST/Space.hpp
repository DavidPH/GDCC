//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree space handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Space_H__
#define GDCC__AST__Space_H__

#include "../Core/Counter.hpp"
#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class Linkage;

      class Exp;
   }

   namespace AST
   {
      //
      // Space
      //
      class Space final : public Core::PlainCounter
      {
         GDCC_Core_CounterPreambleNoVirtual(
            GDCC::AST::Space, GDCC::Core::Counter);

      protected:
         using IRExpCPtr = Core::CounterPtr<IR::Exp const>;

      public:
         Core::String glyph;
         IR::Linkage  linka;
         Core::String name;
         IRExpCPtr    value;

         bool         defin : 1; // Is defined?


         static Ref Create(Core::String name, Core::String glyph);

      protected:
         Space(Core::String name, Core::String glyph);
         ~Space();
      };
   }
}

#endif//GDCC__AST__Space_H__

