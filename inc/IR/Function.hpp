//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
      enum class CallType;
      enum class Linkage;
      enum class ScriptType;
      class Program;

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

         Core::FastU getLocalReg() const {return localReg + localTmp;}

         void setLocalTmp(Core::FastU words)
            {if(localTmp < words) localTmp = words;}

         Block        block;
         CallType     ctype;
         Core::String glyph;
         Core::String label;
         Linkage      linka;
         Core::FastU  localAut;
         Core::FastU  localReg;
         Core::FastU  localTmp;
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
      OArchive &operator << (OArchive &out, Function const &in);

      IArchive &operator >> (IArchive &in, Function &out);
   }
}

#endif//GDCC__IR__Function_H__

