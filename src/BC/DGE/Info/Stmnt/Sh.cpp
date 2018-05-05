//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Shift statement handling.
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
   // Info::preStmnt_ShLU
   //
   void Info::preStmnt_ShLU()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_ShLU_W(n);
   }

   //
   // Info::preStmnt_ShRI
   //
   void Info::preStmnt_ShRI()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_ShRI_W(n);
   }

   //
   // Info::preStmnt_ShRU
   //
   void Info::preStmnt_ShRU()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_ShRU_W(n);
   }

   //
   // Info::putStmnt_ShLU
   //
   void Info::putStmnt_ShLU(char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return putCode("Drop_Nul");

      if(n == 1)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code, n), n + 1);
   }

   //
   // Info::trStmnt_ShLU
   //
   void Info::trStmnt_ShLU()
   {
      trStmntShift(true);
   }
}

// EOF

