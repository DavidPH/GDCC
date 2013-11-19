//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation library import handling.
//
//-----------------------------------------------------------------------------

#include "Import.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Import constructor
      //
      Import::Import(String glyph_) :
         glyph{glyph_}
      {
      }

      //
      // operator OArchive << Import
      //
      OArchive &operator << (OArchive &out, Import const &)
      {
         return out;
      }

      //
      // operator IArchive >> Import
      //
      IArchive &operator >> (IArchive &in, Import &)
      {
         return in;
      }
   }
}

// EOF

