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

#include "BC/DGE/Info.hpp"

#include "IR/Exception.hpp"
#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::preStmnt_Tr
   //
   void Info::preStmnt_Tr()
   {
      auto code = stmnt->code;
      if(!code.type[0]) code.type[0] = 'U';
      if(!code.type[1]) code.type[1] = code.type[0];

      auto dstB = stmnt->args[0].getSize();
      auto srcB = stmnt->args[1].getSize();

      if((dstB != 1 && dstB != 2 && dstB % 4) || (srcB != 1 && srcB != 2 && srcB % 4))
         IR::ErrorCode(stmnt, "bad Tr size");

      auto dstN = (dstB + 3) / 4;
      auto srcN = (srcB + 3) / 4;

      if(dstN != srcN || code.type[0] != code.type[1])
         addFunc_Tr_W(stmnt->code.type, dstN, srcN);
   }

   //
   // Info::putStmnt_Tr
   //
   void Info::putStmnt_Tr()
   {
      auto code = stmnt->code;
      if(!code.type[0]) code.type[0] = 'U';
      if(!code.type[1]) code.type[1] = code.type[0];

      auto dstB = stmnt->args[0].getSize();
      auto srcB = stmnt->args[1].getSize();

      if(dstB == srcB && code.type[0] == code.type[1])
         return;

      if((dstB != 1 && dstB != 2 && dstB % 4) || (srcB != 1 && srcB != 2 && srcB % 4))
         IR::ErrorCode(stmnt, "bad Tr size");

      auto dstN = (dstB + 3) / 4;
      auto srcN = (srcB + 3) / 4;

      if(srcB == 1 || srcB == 2)
         IR::ErrorCode(stmnt, "Tr subword stub");

      if(dstN != srcN || code.type[0] != code.type[1])
         putStmntCall(getFuncName(code, dstN, srcN), srcN);

      if(dstB == 1 || dstB == 2)
         IR::ErrorCode(stmnt, "Tr subword stub");
   }
}

// EOF

