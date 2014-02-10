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
   }

   namespace CC
   {
      class BlockScope;
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

         virtual BlockScope *createScope() = 0;

         Core::CounterRef<AST::Object> getObject(AST::Attribute const &attr);

         GlobalScope *global;

      protected:
         LookupTable<AST::Object> localObj;

         std::vector<BlockScope *> subScopes;
      };
   }
}

#endif//GDCC__CC__Scope__Local_H__

