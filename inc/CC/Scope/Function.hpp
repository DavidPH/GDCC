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
      // Scope_Function
      //
      class Scope_Function : public Scope_Local
      {
      public:
         Scope_Function(Scope_Global *parent, AST::Function *fn,
            Core::Array<Core::CounterRef<AST::Object>> &&params);
         virtual ~Scope_Function();

         void allocAuto();

         virtual Core::CounterRef<AST::Function> getFunction();

         virtual Scope_Function *getScopeFunction();

         Core::Array<Core::CounterRef<AST::Object>> params;
         Core::CounterRef<AST::Function>            fn;
      };
   }
}

#endif//GDCC__CC__Scope__Function_H__

