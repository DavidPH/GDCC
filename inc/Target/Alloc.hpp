//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Allocation algorithm information.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Target__Alloc_H__
#define GDCC__Target__Alloc_H__

#include "../Target/Types.hpp"

#include "../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Target
{
   Core::FastU GetAllocMin(AddrSpace space);

   Core::FastU GetAllocMin_Funct(CallType ctype);
   Core::FastU GetAllocMin_StrEn();
}

#endif//GDCC__Target__Alloc_H__

