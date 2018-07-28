//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Archive exceptions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AR__Exception_H__
#define GDCC__AR__Exception_H__

#include "../AR/Types.hpp"

#include "../Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AR
{
   [[noreturn]]
   void Error(char const *str);
}

#endif//GDCC__AR__Exception_H__

