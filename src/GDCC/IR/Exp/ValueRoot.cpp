//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: value primitive.
//
//-----------------------------------------------------------------------------

#include "ValueRoot.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_ValueRoot constructor
      //
      Exp_ValueRoot::Exp_ValueRoot(IArchive &in) : Super{in}, type{in}, value{in}
      {
      }

      //
      // Exp_ValueRoot::v_writeIR
      //
      OArchive &Exp_ValueRoot::v_writeIR(OArchive &out) const
      {
         return Super::v_writeIR(out) << type << value;
      }

      //
      // ExpCreate_ValueRoot
      //
      Exp::Ref ExpCreate_ValueRoot(Value const &value, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueRoot(value, pos));
      }

      //
      // ExpCreate_ValueRoot
      //
      Exp::Ref ExpCreate_ValueRoot(Value &&value, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueRoot(std::move(value), pos));
      }

      //
      // ExpGetIR_ValueRoot
      //
      Exp::Ref ExpGetIR_ValueRoot(IArchive &in)
      {
         return static_cast<Exp::Ref>(new Exp_ValueRoot(in));
      }
   }
}

// EOF

