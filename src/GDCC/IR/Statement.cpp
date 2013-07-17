//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statements/instructions.
//
//-----------------------------------------------------------------------------

#include "Statement.hpp"

#include "OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << Statement
      //
      OArchive &operator << (OArchive &out, Statement const &in)
      {
         return out << in.pos << in.code << in.args << in.labs;
      }
   }
}

// EOF

