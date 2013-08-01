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

#include <unordered_map>
#include <unordered_set>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class Linkage;

      class Exp;
      class Space;

      //
      // Object
      //
      class Object
      {
      public:
         using ExpPtr = CounterPtr<Exp>;


         explicit Object(String name, Space *space);
         ~Object();

         String  glyph;
         ExpPtr  initi;
         Linkage linka;
         Space  *space;
         FastU   value;
         FastU   words;

         bool    alias : 1;
         bool    exdef : 1;


         static Object &Get(String name, Space *space);
      };

      //
      // Space
      //
      class Space
      {
      public:
         using ObSet = std::unordered_set<Object *>;


         explicit Space(AddrSpace as);

         Object &get(String name) {return Object::Get(name, this);}

         String   glyph;
         Linkage  linka;
         ObSet    obset;
         AddrBase space;
         FastU    value;
         FastU    words;

         bool     exdef : 1;


         static Space *Find(AddrSpace as);

         static Space LocArs;
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
      OArchive &operator << (OArchive &out, Space const &in);
      OArchive &operator << (OArchive &out, Space const *in);

      IArchive &operator >> (IArchive &in, Object &out);
      IArchive &operator >> (IArchive &in, Space &out);
      IArchive &operator >> (IArchive &in, Space *&out);
   }
}

#endif//GDCC__IR__Object_H__

