//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression base.
//
//-----------------------------------------------------------------------------

#include "Exp.hpp"

#include "OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp::v_writeIR
      //
      OArchive &Exp::v_writeIR(OArchive &out) const
      {
         return out << pos;
      }

      //
      // Exp::writeIR
      //
      OArchive &Exp::writeIR(OArchive &out) const
      {
         return v_writeIR(out << getName());
      }

      //
      // operator OArchive << Exp const *
      //
      OArchive &operator << (OArchive &out, Exp const *in)
      {
         if(in)
            return in->writeIR(out << true);
         else
            return out << false;
      }
   }
}

// EOF

