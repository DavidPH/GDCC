//-----------------------------------------------------------------------------
//
// Copyright (C) 2019-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Translate statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Statement.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::genStmnt_Tr
   //
   void Info::genStmnt_Tr()
   {
      auto code = stmnt->code;
      if(!code.type[0]) code.type[0] = 'U';
      if(!code.type[1]) code.type[1] = code.type[0];

      auto dstN = stmnt->args[0].getSize();
      auto srcN = stmnt->args[1].getSize();

      if(dstN == srcN && code.type[0] == code.type[1])
         return;

      // TODO 2025-01-10: This needs inline codegen for simple conversions.

      auto name = getFuncName(code, dstN, srcN);

      if(dstN == 1)
      {
         genStmntArgR1Pre(true);
         genStmntCall(name, IR::Arg_Stk{dstN});
         genStmntArgR1Suf();
      }
      else
      {
         if(stmnt->args[1].a != IR::ArgBase::Stk)
            genStmntPushArg(stmnt->args[1]);

         genStmntCall(name, stmnt->args[0]);
      }
   }

   //
   // Info::preStmnt_Tr
   //
   void Info::preStmnt_Tr()
   {
      auto code = stmnt->code;
      if(!code.type[0]) code.type[0] = 'U';
      if(!code.type[1]) code.type[1] = code.type[0];

      auto dstN = stmnt->args[0].getSize();
      auto srcN = stmnt->args[1].getSize();

      if(dstN == srcN && code.type[0] == code.type[1])
         return;

      addFunc_Tr_W(code.type, dstN, srcN);
   }
}

// EOF

