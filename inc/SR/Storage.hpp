//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Storage durations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Storage_H__
#define GDCC__SR__Storage_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Storage
      //
      enum class Storage
      {
         None,
         Auto,
         Static,
         Thread,
      };
   }
}

#endif//GDCC__SR__Storage_H__

