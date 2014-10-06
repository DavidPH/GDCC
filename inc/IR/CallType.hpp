//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function call type/convention handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__CallType_H__
#define GDCC__IR__CallType_H__

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
      // CallType
      //
      enum class CallType
      {
         #define GDCC_IR_CallTypeList(name) name,
         #include "../IR/CallTypeList.hpp"
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
      OArchive &operator << (OArchive &out, CallType in);
      std::ostream &operator << (std::ostream &out, CallType in);

      IArchive &operator >> (IArchive &in, CallType &out);

      // Maps a CallType to its IR implementation.
      CallType GetCallTypeIR(CallType ctype);

      bool IsCallTypeScript(CallType ctype);
   }
}

#endif//GDCC__IR__CallType_H__

