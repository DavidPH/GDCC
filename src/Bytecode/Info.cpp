//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Target information and handling base class.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   //
   // Info::translate
   //
   void Info::translate()
   {
      for(auto &itr : GDCC::IR::FunctionRange())
         translateBlock(itr.second.block);
   }
}

// EOF

