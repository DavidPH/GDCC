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
      class Scope_Block;
      class Scope_Function;
      class Scope_Global;

      //
      // Scope_Local
      //
      // A non-global scope. In C itself, a function or block scope.
      //
      class Scope_Local : public Scope
      {
      public:
         Scope_Local(Scope *parent, Scope_Global *global);
         virtual ~Scope_Local();

         Scope_Block *createScopeBlock();
         Scope_Block *createScopeLoop();
         Scope_Block *createScopeSwitch();

         Core::String genGlyphObj(Core::String name, IR::Linkage linka);

         void genIR(IR::Program &prog);

         virtual Core::CounterRef<AST::Function> getFunction() = 0;

         virtual Scope_Function *getScopeFunction() = 0;

         Core::CounterRef<AST::Object> getObject(AST::Attribute const &attr);

         Scope_Global *global;
         Core::String  label;

      protected:
         LookupTable<AST::Object> localObj;

         std::vector<Scope_Block *> subScopes;
      };
   }
}

#endif//GDCC__CC__Scope__Local_H__

