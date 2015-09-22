//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      template<typename T>
      class NumberAllocMerge;
   }

   namespace IR
   {
      enum class CallType;
      enum class Linkage;
      enum class ScriptType;

      //
      // Function
      //
      class Function
      {
      protected:
         using LocalArr = std::unordered_map<Core::FastU, Core::FastU>;

      public:
         explicit Function(Core::String glyph);

         void allocValue(Core::NumberAllocMerge<Core::FastU> &allocator);

         Core::FastU getLocalReg() const {return localReg + localTmp;}

         Core::Origin getOrigin() const;

         void setLocalTmp(Core::FastU words)
            {if(localTmp < words) localTmp = words;}

         Core::FastU  allocAut;
         Block        block;
         CallType     ctype;
         Core::String glyph;
         Core::String label;
         Linkage      linka;
         LocalArr     localArr;
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
// Extern Functions                                                           |
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

