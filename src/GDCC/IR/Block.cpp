//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statement blocks.
//
//-----------------------------------------------------------------------------

#include "Block.hpp"

#include "IArchive.hpp"
#include "OArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Block::addStatement
      //
      Block &Block::addStatement(Code code)
      {
         head.labs = Array<String>(Move, labs.begin(), labs.end());
         labs.clear();
         new Statement(&head, code);
         return *this;
      }

      //
      // Block::getIR
      //
      IArchive &Block::getIR(IArchive &in)
      {
         in >> labs >> head;
         for(auto count = GetIR<size_type>(in); count--;)
            in >> *new Statement(&head);
         return in;
      }

      //
      // Block::writeIR
      //
      OArchive &Block::writeIR(OArchive &out) const
      {
         out << labs << head << size();
         for(auto const &stmnt : *this)
            out << stmnt;
         return out;
      }

      //
      // operator OArchive << Block
      //
      OArchive &operator << (OArchive &out, Block const &in)
      {
         return in.writeIR(out);
      }

      //
      // operator IArchive >> Block
      //
      IArchive &operator >> (IArchive &in, Block &out)
      {
         return out.getIR(in);
      }
   }
}

// EOF

