//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
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

      genStmntCall(stmnt->args[0].getSize());
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

   //
   // Info::putStmnt_Tr
   //
   void Info::putStmnt_Tr()
   {
      auto code = stmnt->code;
      if(!code.type[0]) code.type[0] = 'U';
      if(!code.type[1]) code.type[1] = code.type[0];

      auto dstN = stmnt->args[0].getSize();
      auto srcN = stmnt->args[1].getSize();

      if(dstN == srcN && code.type[0] == code.type[1])
         return;

      putStmntCall(getFuncName(code, dstN, srcN), dstN);
   }
}

// EOF

