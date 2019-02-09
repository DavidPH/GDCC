//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   GDCC_BC_CodeTypeSwitchFn(pre, Add, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, Sub, FIU)

   GDCC_BC_CodeTypeSwitchFn(put, Add, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, AddX, Ux)
   GDCC_BC_CodeTypeSwitchFn(put, Sub, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, SubX, Ux)

   //
   // Info::chkStmnt_AddX
   //
   void Info::chkStmnt_AddX()
   {
      if(getStmntSizeW() != 1)
         IR::ErrorCode(stmnt, "unsupported size");
   }

   //
   // Info::putStmnt_AddX_U
   //
   void Info::putStmnt_AddX_U()
   {
      if(stmnt->args.size() > 3)
         putCode("Ad3U");
      else
         putCode("AdXU");
   }

   //
   // Info::putStmnt_SubX_U
   //
   void Info::putStmnt_SubX_U()
   {
      if(stmnt->args.size() > 3)
         putCode("Su3U");
      else
         putCode("SuXU");
   }

   //
   // Info::trStmnt_AddX
   //
   void Info::trStmnt_AddX()
   {
      if(stmnt->args.size() > 3)
      {
         moveArgStk_dst(stmnt->args[0]);
         moveArgStk_src(stmnt->args[1]);
         moveArgStk_src(stmnt->args[2]);
         moveArgStk_src(stmnt->args[3]);
      }
      else
         trStmntStk3(false);
   }

   //
   // Info::trStmnt_SubX_U
   //
   void Info::trStmnt_SubX()
   {
      if(stmnt->args.size() > 3)
      {
         bool stk1 = stmnt->args[1].a == IR::ArgBase::Stk;
         bool stk2 = stmnt->args[2].a == IR::ArgBase::Stk;
         bool stk3 = stmnt->args[3].a == IR::ArgBase::Stk;

         if((!stk1 && (stk2 || stk3)) || (!stk2 && stk3))
            IR::ErrorCode(stmnt, "disorder");

         moveArgStk_dst(stmnt->args[0]);
         moveArgStk_src(stmnt->args[1]);
         moveArgStk_src(stmnt->args[2]);
         moveArgStk_src(stmnt->args[3]);
      }
      else
         trStmntStk3(true);
   }
}

// EOF

