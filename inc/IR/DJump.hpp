//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation dynamic jump target handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__DJump_H__
#define GDCC__IR__DJump_H__

#include "../Core/Number.hpp"
#include "../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class IArchive;
      class OArchive;
      class Program;

      //
      // DJump
      //
      class DJump
      {
      public:
         explicit DJump(Core::String glyph);

         void allocValue(Program &prog);

         Core::String glyph;
         Core::String label;
         Core::FastU  value;

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
      OArchive &operator << (OArchive &out, DJump const &in);

      IArchive &operator >> (IArchive &in, DJump &out);
   }
}

#endif//GDCC__IR__DJump_H__

