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

#ifndef GDCC__IR__Import_H__
#define GDCC__IR__Import_H__

#include "../IR/Types.hpp"

#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // Import
   //
   class Import
   {
   public:
      explicit Import(Core::String glyph);

      Core::String glyph;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, Import const &in);

   IArchive &operator >> (IArchive &in, Import &out);
}

#endif//GDCC__IR__Import_H__

