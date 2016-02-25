//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Wad on-disk format handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AR__Wad__WadHeader_H__
#define GDCC__AR__Wad__WadHeader_H__

#include "../../Core/Array.hpp"
#include "../../Core/Range.hpp"
#include "../../Core/String.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         //
         // WadHeader
         //
         class WadHeader
         {
         public:
            //
            // Lump
            //
            class Lump
            {
            public:
               Core::String name;
               char const  *data;
               std::size_t  size;
            };

            //
            // Dir
            //
            class Dir
            {
            public:
               Core::Range<Core::Array<Lump>::const_iterator> lumps;
               Core::Array<Lump>::const_iterator head;
               Core::Array<Lump>::const_iterator tail;

               Core::String name;
            };

            WadHeader(char const *data, std::size_t size);

            Core::Array<Dir> findDirs() const;

            Core::Array<Dir>    dirs;
            Core::Array<Lump>   lumps;
            std::array<char, 4> ident;
         };
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
      }
   }
}

#endif//GDCC__AR__Wad__WadHeader_H__

