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

#ifndef GDCC__IR__Import_H__
#define GDCC__IR__Import_H__

#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
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
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, Import const &in);

      IArchive &operator >> (IArchive &in, Import &out);
   }
}

#endif//GDCC__IR__Import_H__

