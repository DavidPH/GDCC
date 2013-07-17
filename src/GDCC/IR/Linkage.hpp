//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Linkage type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Types_H__
#define GDCC__IR__Types_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class OArchive;

      //
      // Linkage
      //
      enum class Linkage
      {
         None,
         ExtACS,
         ExtASM,
         ExtC,
         ExtCXX,
         ExtDS,
         IntC,
         IntCXX,
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
      OArchive &operator << (OArchive &out, Linkage in);
   }
}

#endif//GDCC__IR__Types_H__

