//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function script type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__ScriptType_H__
#define GDCC__IR__ScriptType_H__

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class IArchive;
      class OArchive;

      //
      // ScriptType
      //
      enum class ScriptType
      {
         None,
         Death,
         Disconnect,
         Enter,
         Lightning,
         Open,
         Respawn,
         Return,
         Unloading,
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
      OArchive &operator << (OArchive &out, ScriptType in);
      std::ostream &operator << (std::ostream &out, ScriptType in);

      IArchive &operator >> (IArchive &in, ScriptType &out);
   }
}

#endif//GDCC__IR__ScriptType_H__

