//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Argument "moving" functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "IR/Arg.hpp"
#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::moveArgStk_dst
   //
   // If idx is not Stk, makes it one by adding a new Move_W statement.
   //
   void Info::moveArgStk_dst(IR::Arg &idx)
   {
      if(idx.a == IR::ArgBase::Stk) return;

      auto size = idx.getSize();

      block->setOrigin(stmnt->pos);
      block->addStmnt(stmnt->next, IR::Code::Move_W, std::move(idx), IR::Arg_Stk(size));

      idx = IR::Arg_Stk(size);
   }

   //
   // Info::moveArgStk_src
   //
   // If idx is not Stk, makes it one by adding a new Move_W statement.
   //
   void Info::moveArgStk_src(IR::Arg &idx)
   {
      if(idx.a == IR::ArgBase::Stk) return;

      auto size = idx.getSize();

      block->setOrigin(stmnt->pos);
      block->addLabel(std::move(stmnt->labs));
      block->addStmnt(stmnt, IR::Code::Move_W, IR::Arg_Stk(size), std::move(idx));

      idx = IR::Arg_Stk(size);

      // Reset iterator for further translation.
      stmnt = stmnt->prev;

      throw ResetStmnt();
   }

   //
   // Info::moveArgStkB_dst
   //
   // If idx is not Stk, makes it one by adding a new Move_W statement.
   //
   void Info::moveArgStkB_dst(IR::Arg &idx)
   {
      if(idx.a == IR::ArgBase::Stk) return;

      auto size = idx.getSize();

      block->setOrigin(stmnt->pos);
      block->addStmnt(stmnt->next, IR::Code::Move_B, std::move(idx), IR::Arg_Stk(size));

      idx = IR::Arg_Stk(size);
   }

   //
   // Info::moveArgStkB_src
   //
   // If idx is not Stk, makes it one by adding a new Move_W statement.
   //
   void Info::moveArgStkB_src(IR::Arg &idx)
   {
      if(idx.a == IR::ArgBase::Stk) return;

      auto size = idx.getSize();

      block->setOrigin(stmnt->pos);
      block->addLabel(std::move(stmnt->labs));
      block->addStmnt(stmnt, IR::Code::Move_B, IR::Arg_Stk(size), std::move(idx));

      idx = IR::Arg_Stk(size);

      // Reset iterator for further translation.
      stmnt = stmnt->prev;

      throw ResetStmnt();
   }
}

// EOF

