//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source position/origin information.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Origin_H__
#define GDCC__Origin_H__

#include "String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // Origin
   //
   struct Origin
   {
      Origin() = default;
      Origin(String file_, std::size_t line_) : file{file_}, line{line_} {}

      String      file;
      std::size_t line;
   };
}

#endif//GDCC__Origin_H__

