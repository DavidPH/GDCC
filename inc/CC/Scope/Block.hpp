//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C block scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Block_H__
#define GDCC__CC__Scope__Block_H__

#include "../../CC/Scope/Local.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      class Scope_Function;

      //
      // Scope_Block
      //
      class Scope_Block : public Scope_Local
      {
      public:
         Scope_Block(Scope_Local *parent, Scope_Function *fn);
         virtual ~Scope_Block();

         AllocAutoInfo allocAuto(AllocAutoInfo const &base);

         virtual Core::CounterRef<AST::Function> getFunction();

         virtual Scope_Function *getScopeFunction();

         Scope_Function *fn;
      };
   }
}

#endif//GDCC__CC__Scope__Block_H__

