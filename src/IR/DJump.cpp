//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation dynamic jump target handling.
//
//-----------------------------------------------------------------------------

#include "IR/DJump.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include "Core/NumberAlloc.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // DJump constructor
   //
   DJump::DJump(Core::String glyph_) :
      glyph{glyph_},
      label{nullptr},
      value{0},

      alloc{false},
      defin{false}
   {
   }

   //
   // DJump::allocValue
   //
   void DJump::allocValue(Core::NumberAllocMerge<Core::FastU> &allocator)
   {
      value = allocator.alloc(1, value);

      alloc = false;
   }

   //
   // operator OArchive << DJump
   //
   OArchive &operator << (OArchive &out, DJump const &in)
   {
      return out << in.label << in.value << in.alloc << in.defin;
   }

   //
   // Operator IArchive >> DJump
   //
   IArchive &operator >> (IArchive &in, DJump &out)
   {
      in >> out.label >> out.value;

      out.alloc = GetIR<bool>(in);
      out.defin = GetIR<bool>(in);

      return in;
   }
}

// EOF

