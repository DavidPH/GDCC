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

#include "../../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      class FunctionScope;

      //
      // BlockScope
      //
      class BlockScope : public LocalScope
      {
      public:
         //
         // AllocAutoInfo
         //
         struct AllocAutoInfo
         {
            void setMax(AllocAutoInfo const &alloc);

            Core::FastU localArs = 0;
            Core::FastU localReg = 0;
         };


         BlockScope(LocalScope *parent, FunctionScope *fn);
         virtual ~BlockScope();

         AllocAutoInfo allocAuto(AllocAutoInfo const &base);

         virtual Core::CounterRef<AST::Function> getFunction();

         virtual FunctionScope *getScopeFunction();

         FunctionScope *fn;
      };
   }
}

#endif//GDCC__CC__Scope__Block_H__

