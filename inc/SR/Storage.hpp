//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Storage durations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Storage_H__
#define GDCC__SR__Storage_H__

#include "../SR/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Storage
   //
   enum class Storage
   {
      None,
      Auto,
      Static,
      Thread,
   };
}

#endif//GDCC__SR__Storage_H__

