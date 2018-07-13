//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
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

#include "../Platform/Types.hpp"

#include "../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Platform
{
   Core::FastU GetAllocMin(IR::AddrSpace space);

   Core::FastU GetAllocMin_Funct(IR::CallType ctype);
   Core::FastU GetAllocMin_StrEn();
}

#endif//GDCC__Platform__Alloc_H__

