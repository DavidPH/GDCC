//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation types.
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

#endif//GDCC__IR__Types_H__
