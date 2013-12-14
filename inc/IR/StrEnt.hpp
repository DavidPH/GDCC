//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation string table entry handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__StrEnt_H__
#define GDCC__IR__StrEnt_H__

#include "../Core/Number.hpp"
#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Program;

      //
      // StrEnt
      //
      class StrEnt
      {
      public:
         explicit StrEnt(Core::String glyph);

         void allocValue(Program &prog);

         Core::String glyph;
         Core::FastU  valueInt;
         Core::String valueStr;

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
      OArchive &operator << (OArchive &out, StrEnt const &in);

      IArchive &operator >> (IArchive &in, StrEnt &out);
   }
}

#endif//GDCC__IR__StrEnt_H__

