//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Function_H__
#define GDCC__IR__Function_H__

#include "Block.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class Linkage;

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

      //
      // Function
      //
      class Function
      {
      public:
         explicit Function(String glyph);

         Block      block;
         CallType   ctype;
         String     glyph;
         String     label;
         Linkage    linka;
         FastU      localArs;
         FastU      localReg;
         FastU      param;
         FastU      retrn;
         ScriptType stype;
         FastI      valueInt;
         String     valueStr;

         bool       exdef    : 1;
         bool       sflagNet : 1;
         bool       sflagClS : 1;


         static Function *Find(String glyph);

         static Function &Get(String glyph);
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
      OArchive &operator << (OArchive &out, Function const &in);
      OArchive &operator << (OArchive &out, ScriptType in);
   }
}

#endif//GDCC__IR__Function_H__

