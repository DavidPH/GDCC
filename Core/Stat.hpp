//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Directory handling abstraction.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Stat_H__
#define GDCC__Core__Stat_H__

#include "../Core/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // Stat
   //
   class Stat
   {
   public:
      //
      // Type
      //
      enum class Type
      {
         None,

         Dir,
         File
      };


      Type type;
   };
}

#endif//GDCC__Core__Stat_H__

