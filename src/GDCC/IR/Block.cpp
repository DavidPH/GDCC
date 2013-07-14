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
   }
}

// EOF

