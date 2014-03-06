//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C local scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Local_H__
#define GDCC__CC__Scope__Local_H__

#include "../../CC/Scope.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Attribute;
      class Function;
   }

   namespace IR
   {
      enum class Linkage;

      class Program;
   }

   namespace CC
   {
      class BlockScope;
      class FunctionScope;
      class GlobalScope;

      //
      // LocalScope
      //
      // A non-global scope. In C itself, a function or block scope.
      //
      class LocalScope : public Scope
      {
      public:
         LocalScope(Scope *parent, GlobalScope *global);
         virtual ~LocalScope();

         BlockScope *createScopeBlock();
         BlockScope *createScopeLoop();
         BlockScope *createScopeSwitch();

         Core::String genGlyphObj(Core::String name, IR::Linkage linka);

         void genIR(IR::Program &prog);

         virtual Core::CounterRef<AST::Function> getFunction() = 0;

         virtual FunctionScope *getScopeFunction() = 0;

         Core::CounterRef<AST::Object> getObject(AST::Attribute const &attr);

         GlobalScope *global;
         Core::String label;

      protected:
         LookupTable<AST::Object> localObj;

         std::vector<BlockScope *> subScopes;
      };
   }
}

#endif//GDCC__CC__Scope__Local_H__

