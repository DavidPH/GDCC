//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation library import handling.
//
//-----------------------------------------------------------------------------

#include "IR/Import.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // Import constructor
   //
   Import::Import(Core::String glyph_) :
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

// EOF

