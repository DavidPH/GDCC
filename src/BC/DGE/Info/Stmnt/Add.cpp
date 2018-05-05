//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::preStmnt_AddU
   //
   void Info::preStmnt_AddU()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_AddU_W(n);
   }

   //
   // Info::preStmnt_SubU
   //
   void Info::preStmnt_SubU()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_SubU_W(n);
   }

   //
   // Info::putStmnt_AdXU
   //
   void Info::putStmnt_AdXU()
   {
      if(stmnt->args.size() > 3)
         putCode("Ad3U");
      else
         putCode("AdXU");
   }

   //
   // Info::putStmnt_AddU
   //
   void Info::putStmnt_AddU()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode("AddU");

      putStmntCall(getFuncName(IR::Code::AddU, n), n * 2);
   }

   //
   // Info::putStmnt_SuXU
   //
   void Info::putStmnt_SuXU()
   {
      if(stmnt->args.size() > 3)
         putCode("Su3U");
      else
         putCode("SuXU");
   }

   //
   // Info::putStmnt_SubU
   //
   void Info::putStmnt_SubU()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode("SubU");

      putStmntCall(getFuncName(IR::Code::SubU, n), n * 2);
   }

   //
   // Info::trStmnt_AdXU
   //
   void Info::trStmnt_AdXU()
   {
      CheckArgC(stmnt, 3);

      auto n = getStmntSizeW();

      if(n != 1)
         throw Core::ExceptStr(stmnt->pos, "unsupported AdXU_W size");

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
   // Info::trStmnt_AddU
   //
   void Info::trStmnt_AddU()
   {
      trStmntStk3(false);
   }

   //
   // Info::trStmnt_SuXU
   //
   void Info::trStmnt_SuXU()
   {
      CheckArgC(stmnt, 3);

      auto n = getStmntSizeW();

      if(n != 1)
         throw Core::ExceptStr(stmnt->pos, "unsupported SuXU_W size");

      if(stmnt->args.size() > 3)
      {
         bool stk1 = stmnt->args[1].a == IR::ArgBase::Stk;
         bool stk2 = stmnt->args[2].a == IR::ArgBase::Stk;
         bool stk3 = stmnt->args[3].a == IR::ArgBase::Stk;

         if((!stk1 && (stk2 || stk3)) || (!stk2 && stk3))
            throw Core::ExceptStr(stmnt->pos, "SuXU_W disorder");

         moveArgStk_dst(stmnt->args[0]);
         moveArgStk_src(stmnt->args[1]);
         moveArgStk_src(stmnt->args[2]);
         moveArgStk_src(stmnt->args[3]);
      }
      else
         trStmntStk3(true);
   }

   //
   // Info::trStmnt_SubU
   //
   void Info::trStmnt_SubU()
   {
      trStmntStk3(true);
   }
}

// EOF

