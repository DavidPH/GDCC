//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation object handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Object_H__
#define GDCC__IR__Object_H__

#include "Addr.hpp"

#include "../Counter.hpp"
#include "../Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class Linkage;

      class Exp;
      class Program;

      //
      // Object
      //
      class Object
      {
      public:
         using ExpCPtr = CounterPtr<Exp const>;


         explicit Object(String name);
         ~Object();

         void allocValue(Program &prog);

         String    glyph;
         ExpCPtr   initi;
         Linkage   linka;
         AddrSpace space;
         FastU     value;
         FastU     words;

         bool      alias : 1;
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
      OArchive &operator << (OArchive &out, Object const &in);

      IArchive &operator >> (IArchive &in, Object &out);
   }
}

#endif//GDCC__IR__Object_H__

