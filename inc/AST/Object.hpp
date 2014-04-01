//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree object handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Object_H__
#define GDCC__AST__Object_H__

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
      class Object;
      class Program;
   }

   namespace AST
   {
      enum class Storage;

      class Exp;
      class Type;

      //
      // Object
      //
      class Object final : public Core::PlainCounter
      {
         GDCC_Core_CounterPreambleNoVirtual(
            GDCC::AST::Object, GDCC::Core::Counter);

      protected:
         using ExpCPtr   = Core::CounterPtr<Exp     const>;
         using IRExpCPtr = Core::CounterPtr<IR::Exp const>;
         using TypeCPtr  = Core::CounterPtr<Type    const>;

      public:
         void genObject(IR::Program &prog);

         Core::String glyph;
         ExpCPtr      init;
         IR::Linkage  linka;
         Core::String name;
         Storage      store;
         TypeCPtr     type;
         IRExpCPtr    value;

         bool         alias : 1; // Can alias?
         bool         defin : 1; // Is defined?
         bool         noRef : 1; // Forbids pointer-to?
         bool         refer : 1; // Needs pointer-to?


         static Ref Create(Core::String name, Core::String glyph);

      protected:
         Object(Core::String name, Core::String glyph);
         ~Object();
      };
   }
}

#endif//GDCC__AST__Object_H__

