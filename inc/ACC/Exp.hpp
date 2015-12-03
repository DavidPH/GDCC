//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Exp_H__
#define GDCC__ACC__Exp_H__

#include "../Core/Counter.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Exp;
      class Type;
   }

   namespace Core
   {
      class Origin;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      // Conversion as if by assignment.
      Core::CounterRef<AST::Exp const> ExpPromo_Assign(AST::Type const *t,
         AST::Exp const *e, Core::Origin pos);
   }
}

#endif//GDCC__ACC__Exp_H__

