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
      Block &Block::addStatement(Statement *link, Code code)
      {
         head.labs = Array<String>(Move, labs.begin(), labs.end());
         labs.clear();
         new Statement(&head, link, code);
         return *this;
      }

      //
      // operator OArchive << Block
      //
      OArchive &operator << (OArchive &out, Block const &in)
      {
         out << in.labs << in.head << in.size();
         for(auto const &i : in)
            out << i;
         return out;
      }

      //
      // operator IArchive >> Block
      //
      IArchive &operator >> (IArchive &in, Block &out)
      {
         in >> out.labs >> out.head;
         for(auto count = GetIR<Block::size_type>(in); count--;)
            in >> *new Statement(&out.head);
         return in;
      }
   }
}

// EOF

