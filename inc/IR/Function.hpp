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

#include "../IR/Block.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class Linkage;
      class Program;

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
         explicit Function(Core::String glyph);

         void allocValue(Program &prog, CallType ctypeVec)
            {allocValue(prog, &ctypeVec, 1);}

         template<std::size_t ctypeLen>
         void allocValue(Program &prog, CallType const (&ctypeVec)[ctypeLen])
            {allocValue(prog, ctypeVec, ctypeLen);}

         void allocValue(Program &prog, CallType const *ctypeVec, std::size_t ctypeLen);

         Block        block;
         CallType     ctype;
         Core::String glyph;
         Core::String label;
         Linkage      linka;
         Core::FastU  localArs;
         Core::FastU  localReg;
         Core::FastU  param;
         Core::FastU  retrn;
         ScriptType   stype;
         Core::FastU  valueInt;
         Core::String valueStr;

         bool         alloc    : 1;
         bool         defin    : 1;
         bool         sflagNet : 1;
         bool         sflagClS : 1;
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
      std::ostream &operator << (std::ostream &out, CallType in);
      std::ostream &operator << (std::ostream &out, ScriptType in);

      IArchive &operator >> (IArchive &in, CallType &out);
      IArchive &operator >> (IArchive &in, Function &out);
      IArchive &operator >> (IArchive &in, ScriptType &out);
   }
}

#endif//GDCC__IR__Function_H__

