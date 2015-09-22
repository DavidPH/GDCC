//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation address space handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Space_H__
#define GDCC__IR__Space_H__

#include "../IR/Addr.hpp"

#include "../Core/Number.hpp"


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
      enum class Linkage;
      class Program;

      //
      // Space
      //
      class Space
      {
      public:
         explicit Space(AddrSpace as);

         void allocValue(Core::NumberAllocMerge<Core::FastU> &allocator);

         Core::String glyph;
         Linkage      linka;
         AddrSpace    space;
         Core::FastU  value;
         Core::FastU  words;

         bool         alloc : 1;
         bool         defin : 1;
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
      OArchive &operator << (OArchive &out, Space const &in);

      IArchive &operator >> (IArchive &in, Space &out);
   }
}

#endif//GDCC__IR__Space_H__

