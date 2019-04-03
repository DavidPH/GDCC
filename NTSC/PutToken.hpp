//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// NTS token output.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__NTSC__PutToken_H__
#define GDCC__NTSC__PutToken_H__

#include "../NTSC/Types.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::NTSC
{
   void PutToken(std::ostream &out, Core::Token const &tok);
   void PutToken_Number(std::ostream &out, Core::Token const &tok);
}

#endif//GDCC__NTSC__PutToken_H__

