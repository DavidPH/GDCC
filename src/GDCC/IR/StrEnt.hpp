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

#include "../Number.hpp"
#include "../String.hpp"
#include "../Utility.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // StrEnt
      //
      class StrEnt
      {
      public:
         explicit StrEnt(String glyph);

         void allocValue();

         String glyph;
         FastU  valueInt;
         String valueStr;

         bool alias : 1;
         bool alloc : 1;
         bool defin : 1;


         static StrEnt *FindValue(String value);

         static StrEnt &Get(String glyph);
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

      GDCC::Range<std::unordered_map<String, StrEnt>::iterator> StrEntRange();
   }
}

#endif//GDCC__IR__StrEnt_H__

