//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compare statement handling.
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
   // Info::preStmnt_CmpU_EQ
   //
   void Info::preStmnt_CmpU_EQ(AddFunc add)
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      (this->*add)(n);
   }

   //
   // Info::putStmnt_CmpU_EQ
   //
   void Info::putStmnt_CmpU_EQ(IR::Code code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return putCode("Push_Lit", code == IR::Code::CmpU_EQ);

      if(n == 1)
         return putCode(code == IR::Code::CmpU_EQ ? "CmpU_EQ" : "CmpU_NE");

      putStmntCall(getFuncName(code, n), n * 2);
   }
}

// EOF

