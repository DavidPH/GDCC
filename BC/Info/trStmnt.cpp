//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic translation functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::trStmntStk2
   //
   void Info::trStmntStk2()
   {
      moveArgStk_dst(stmnt->args[0]);
      moveArgStk_src(stmnt->args[1]);
   }

   //
   // Info::trStmntStk3
   //
   void Info::trStmntStk3(bool ordered)
   {
      moveArgStk_dst(stmnt->args[0]);
      moveArgStk_src(stmnt->args[1], ordered && stmnt->args[2].a == IR::ArgBase::Stk);
      moveArgStk_src(stmnt->args[2]);
   }

   //
   // Info::trStmntShift
   //
   bool Info::trStmntShift(bool moveLit)
   {
      if(stmnt->args[1].a != IR::ArgBase::Stk &&
         stmnt->args[2].a == IR::ArgBase::Stk)
         Core::Error(stmnt->pos, "trStmntShift disorder");

      moveArgStk_dst(stmnt->args[0]);
      moveArgStk_src(stmnt->args[1]);

      if(!moveLit && stmnt->args[2].a == IR::ArgBase::Lit)
         return true;

      moveArgStk_src(stmnt->args[2]);
      return false;
   }
}

// EOF

