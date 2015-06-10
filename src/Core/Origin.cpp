//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source position/origin information.
//
//-----------------------------------------------------------------------------

#include "Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // operator std::ostream << Origin
      //
      std::ostream &operator << (std::ostream &out, Origin const &in)
      {
         if(in.file)
            out << in.file;
         else
            out << "(no file)";

         if(in.line) out << ':' << in.line;
         if(in.col)  out << ':' << in.col;

         return out;
      }
   }
}

// EOF

