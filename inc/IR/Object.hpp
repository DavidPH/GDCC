//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "../IR/Addr.hpp"

#include "../Core/Counter.hpp"
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

      class Exp;

      //
      // Object
      //
      class Object
      {
      public:
         using ExpCPtr = Core::CounterPtr<Exp const>;


         explicit Object(Core::String name);
         ~Object();

         void allocValue(Core::NumberAllocMerge<Core::FastU> &allocator);

         Core::String glyph;
         ExpCPtr      initi;
         Linkage      linka;
         AddrSpace    space;
         Core::FastU  value;
         Core::FastU  words;

         bool         alias : 1;
         bool         alloc : 1;
         bool         defin : 1;
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

