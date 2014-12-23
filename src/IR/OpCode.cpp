//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation operation-code handling.
//
//-----------------------------------------------------------------------------

#include "IR/OpCode.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << OpCode
      //
      OArchive &operator << (OArchive &out, OpCode in)
      {
         return out << in.code << in.size;
      }

      //
      // operator std::ostream << OpCode
      //
      std::ostream &operator << (std::ostream &out, OpCode in)
      {
         return out << in.code << ' ' << in.size;
      }

      //
      // operator IArchive >> OpCode
      //
      IArchive &operator >> (IArchive &in, OpCode &out)
      {
         return in >> out.code >> out.size;
      }
   }
}

// EOF

