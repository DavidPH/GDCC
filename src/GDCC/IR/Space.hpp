//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "Addr.hpp"

#include "../Number.hpp"


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
      // Space
      //
      class Space
      {
      public:
         explicit Space(AddrSpace as);

         void allocValue(Program &prog);
         void allocWords(Program &prog);

         String    glyph;
         Linkage   linka;
         AddrSpace space;
         FastU     value;
         FastU     words;

         bool      alloc : 1;
         bool      defin : 1;
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
      OArchive &operator << (OArchive &out, Space const &in);

      IArchive &operator >> (IArchive &in, Space &out);
   }
}

#endif//GDCC__IR__Space_H__

