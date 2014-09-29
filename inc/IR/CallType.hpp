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
         None,
         Action,  // __action
         AsmFunc, // __asmfunc
         LangACS, // extern "ACS"
         LangASM, // extern "asm"
         LangAXX, // extern "ACS++"
         LangC,   // extern "C"
         LangCXX, // extern "C++"
         LangDS,  // extern "DS"
         Native,  // __native
         Script,  // __script
         ScriptI, // __scripti
         ScriptS, // __scripts
         Special, // __special
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

      bool IsCallTypeScript(CallType ctype);
   }
}

#endif//GDCC__IR__CallType_H__

