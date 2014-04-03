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

#include "CC/Scope/Block.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Scope_Block constructor
      //
      Scope_Block::Scope_Block(Scope_Local &parent_) :
         Scope_Local{parent_}
      {
      }

      //
      // Scope_Block::allocAuto
      //
      Scope_Block::AllocAutoInfo Scope_Block::allocAuto(AllocAutoInfo const &base)
      {
         // Allocate local objects.
         auto alloc = base;
         for(auto &itr : localObj)
            allocAutoObj(alloc, itr.second);

         // Allocate sub-scopes.
         auto allocSub = alloc;
         for(auto &ctx : subScopes)
            allocSub.setMax(ctx->allocAuto(alloc));

         return allocSub;
      }
   }
}

//EOF

