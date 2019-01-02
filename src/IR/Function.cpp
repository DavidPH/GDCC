//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function handling.
//
//-----------------------------------------------------------------------------

#include "IR/Function.hpp"

#include "IR/Linkage.hpp"
#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include "Core/NumberAlloc.hpp"

#include "Target/Alloc.hpp"
#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // Function constructor
   //
   Function::Function(Core::String glyph_) :
      allocAut{0},
      block   {},
      ctype   {CallType::None},
      glyph   {glyph_},
      label   {Core::STRNULL},
      linka   {Linkage::None},
      localAut{0},
      localReg{0},
      localTmp{0},
      param   {0},
      retrn   {0},
      stype   {},
      valueInt{0},
      valueStr{Core::STRNULL},

      alloc   {false},
      defin   {false}
   {
   }

   //
   // Function::allocValue
   //
   void Function::allocValue(Core::NumberAllocMerge<Core::FastU> &allocator)
   {
      auto valueMin = Target::GetAllocMin_Funct(ctype);

      if(valueInt < valueMin)
         valueInt = valueMin;

      valueInt = allocator.alloc(1, valueInt);

      alloc = false;
   }

   //
   // Function::getOrigin
   //
   Core::Origin Function::getOrigin() const
   {
      if(!block.empty())
         return block.begin()->pos;

      return {nullptr, 0, 0};
   }

   //
   // operator OArchive << Function
   //
   OArchive &operator << (OArchive &out, Function const &in)
   {
      return out
         << in.allocAut
         << in.block
         << in.ctype
         << in.label
         << in.linka
         << in.localArr
         << in.localAut
         << in.localReg
         << in.param
         << in.retrn
         << in.stype
         << in.valueInt
         << in.valueStr
         << in.alloc
         << in.defin;
   }

   //
   // operator IArchive >> Function
   //
   IArchive &operator >> (IArchive &in, Function &out)
   {
      in >> out.allocAut
         >> out.block
         >> out.ctype
         >> out.label
         >> out.linka
         >> out.localArr
         >> out.localAut
         >> out.localReg
         >> out.param
         >> out.retrn
         >> out.stype
         >> out.valueInt
         >> out.valueStr;

      out.alloc = GetIR<bool>(in);
      out.defin = GetIR<bool>(in);

      return in;
   }
}

// EOF

