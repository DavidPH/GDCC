//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Allocation algorithm information.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Platform__Alloc_H__
#define GDCC__Platform__Alloc_H__

#include "../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class AddrSpace;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Platform
   {
      Core::FastU GetAllocMin(IR::AddrSpace space);

      Core::FastU GetAllocMin_StrEn();
   }
}

#endif//GDCC__Platform__Alloc_H__

