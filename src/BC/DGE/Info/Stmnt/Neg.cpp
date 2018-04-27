//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Negation statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::putStmnt_NegF_W
   //
   void Info::putStmnt_NegF_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      putCode("Push_Lit", 0x80000000);
      putCode("BOrX");
   }
}

// EOF

