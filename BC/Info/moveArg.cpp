//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
      block->addStmnt(stmnt->next, IR::CodeBase::Move, std::move(idx), IR::Arg_Stk(size));

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
      block->addStmnt(stmnt, IR::CodeBase::Move, IR::Arg_Stk(size), std::move(idx));

      idx = IR::Arg_Stk(size);

      // Reset iterator for further translation.
      stmnt = stmnt->prev;

      throw ResetStmnt();
   }
}

// EOF

