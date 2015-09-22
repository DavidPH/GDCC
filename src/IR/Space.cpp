//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation address space handling.
//
//-----------------------------------------------------------------------------

#include "IR/Space.hpp"

#include "IR/IArchive.hpp"
#include "IR/Linkage.hpp"
#include "IR/OArchive.hpp"

#include "Core/NumberAlloc.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Space constructor
      //
      Space::Space(AddrSpace as) :
         glyph{as.name},
         linka{Linkage::None},
         space{as},
         value{0},
         words{0},

         alloc{false},
         defin{false}
      {
      }

      //
      // Space::allocValue
      //
      void Space::allocValue(Core::NumberAllocMerge<Core::FastU> &allocator)
      {
         value = allocator.alloc(1, value);

         alloc = false;
      }

      //
      // operator OArchive << Space
      //
      OArchive &operator << (OArchive &out, Space const &in)
      {
         return out
            << in.linka
            << in.value
            << in.words
            << in.alloc
            << in.defin
         ;
      }

      //
      // operator IArchive >> Space
      //
      IArchive &operator >> (IArchive &in, Space &out)
      {
         in
            >> out.linka
            >> out.value
            >> out.words
         ;

         out.alloc = GetIR<bool>(in);
         out.defin = GetIR<bool>(in);

         return in;
      }
   }
}

// EOF

