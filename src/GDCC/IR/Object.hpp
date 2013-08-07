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
#include "../Utility.hpp"

#include <map>
#include <set>


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

         void allocValue();

         String  glyph;
         ExpPtr  initi;
         Linkage linka;
         Space  *space;
         FastU   value;
         FastU   words;

         bool    alias : 1;
         bool    alloc : 1;
         bool    exdef : 1;


         static Object &Get(String name, Space *space);
      };

      //
      // Space
      //
      class Space
      {
      public:
         using ObSet = std::set<Object *, LessMem<Object, String, &Object::glyph>>;

         using SpaceMap = std::map<String, Space>;


         explicit Space(AddrSpace as);

         void allocValue();
         void allocWords();

         Object &get(String name) {return Object::Get(name, this);}

         String    glyph;
         Linkage   linka;
         ObSet     obset;
         AddrBase  space;
         SpaceMap *spmap;
         FastU     value;
         FastU     words;

         bool      alloc : 1;
         bool      exdef : 1;


         static Space *Find(AddrSpace as);

         static Space &Get(AddrSpace as);

         static Space GblReg;
         static Space LocArs;
         static Space MapReg;
         static Space WldReg;

         static SpaceMap GblArs;
         static SpaceMap MapArs;
         static SpaceMap WldArs;
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

