//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Storage durations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Storage_H__
#define GDCC__AST__Storage_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
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

#endif//GDCC__AST__Storage_H__

