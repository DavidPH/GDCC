//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation string table entry handling.
//
//-----------------------------------------------------------------------------

#include "IR/StrEnt.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include "Core/NumberAlloc.hpp"

#include "Platform/Alloc.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // StrEnt constructor
   //
   StrEnt::StrEnt(Core::String glyph_) :
      glyph{glyph_},
      valueInt{0},
      valueStr{Core::STRNULL},

      alias   {false},
      alloc   {false},
      defin   {false},
      multiDef{false}
   {
   }

   //
   // StrEnt::allocValue
   //
   void StrEnt::allocValue(Program &prog, Core::NumberAllocMerge<Core::FastU> &allocator)
   {
      Core::FastU allocMin = Platform::GetAllocMin_StrEn();

      if(valueInt < allocMin)
         valueInt = allocMin;

      // First, check for any strings to alias with.
      if(alias) for(auto const &itr : prog.rangeStrEnt())
      {
         if(&itr == this || itr.alloc || !itr.alias) continue;
         if(itr.valueStr != valueStr) continue;
         if(itr.valueInt <  valueInt) continue;

         alloc    = false;
         valueInt = itr.valueInt;
         return;
      }

      // Otherwise, run allocation.
      valueInt = allocator.alloc(1, valueInt);

      alloc = false;
   }

   //
   // operator OArchive << StrEnt
   //
   OArchive &operator << (OArchive &out, StrEnt const &in)
   {
      return out
         << in.valueInt
         << in.valueStr
         << in.alias
         << in.alloc
         << in.defin
         << in.multiDef;
   }

   //
   // Operator IArchive >> StrEnt
   //
   IArchive &operator >> (IArchive &in, StrEnt &out)
   {
      in >> out.valueInt
         >> out.valueStr;

      out.alias    = GetIR<bool>(in);
      out.alloc    = GetIR<bool>(in);
      out.defin    = GetIR<bool>(in);
      out.multiDef = GetIR<bool>(in);

      return in;
   }
}

// EOF

