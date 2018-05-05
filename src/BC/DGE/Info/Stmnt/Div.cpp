//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Division statement handling.
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
   // Info::putStmnt_DivU
   //
   void Info::putStmnt_DivU(char const *code, IR::Code codeX, bool mod)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode(code);

      putStmntCall(getFuncName(codeX, n), n * 2);
      if(mod) putStmntDropTmp(0, n);
      for(Core::FastU i = n; i--;) putCode("Drop_Nul");
      if(mod) putStmntPushTmp(0, n);
   }

   //
   // Info::trStmnt_DiXU
   //
   void Info::trStmnt_DiXU()
   {
      trStmntStk3(true);
   }

   //
   // Info::trStmnt_ModU
   //
   void Info::trStmnt_ModU()
   {
      auto n = getStmntSizeW();

      trStmntStk3(true);

      if(n > 1)
         trStmntTmp(n);
   }
}

// EOF

