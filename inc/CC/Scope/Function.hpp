//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Function_H__
#define GDCC__CC__Scope__Function_H__

#include "../../CC/Scope/Local.hpp"

#include "../../Core/Array.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // FunctionScope
      //
      class FunctionScope : public LocalScope
      {
      public:
         FunctionScope(GlobalScope *parent, AST::Function *fn,
            Core::Array<Core::CounterRef<AST::Object>> &&params);
         virtual ~FunctionScope();

         void allocAuto();

         virtual BlockScope *createScope();

         Core::Array<Core::CounterRef<AST::Object>> params;
         Core::CounterRef<AST::Function>            fn;
      };
   }
}

#endif//GDCC__CC__Scope__Function_H__

