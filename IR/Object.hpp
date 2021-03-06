//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
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

#include "../IR/Types.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"

#include "../Target/Addr.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, Object const &in);

   IArchive &operator >> (IArchive &in, Object &out);
}

#endif//GDCC__IR__Object_H__

