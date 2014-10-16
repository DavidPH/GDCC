//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Argument "moving" functions.
//
//-----------------------------------------------------------------------------

#include "Bytecode/Info.hpp"

#include "IR/Arg.hpp"
#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      //
      // Info::moveArgStk_W_dst
      //
      // If idx is not Stk, makes it one by adding a new Move_W statement.
      //
      bool Info::moveArgStk_W_dst(IR::Arg &idx, IR::Code codeMove)
      {
         if(idx.a == IR::ArgBase::Stk) return false;

         block->setOrigin(stmnt->pos);
         block->addStatementArgs(stmnt->next, codeMove,
            std::move(idx), IR::Arg_Stk());

         idx = IR::Arg_Stk();

         return true;
      }

      //
      // Info::moveArgStk_W_src
      //
      // If idx is not Stk, makes it one by adding a new Move_W statement.
      //
      bool Info::moveArgStk_W_src(IR::Arg &idx, IR::Code codeMove)
      {
         if(idx.a == IR::ArgBase::Stk) return false;

         block->setOrigin(stmnt->pos);
         block->addLabel(std::move(stmnt->labs));
         block->addStatementArgs(stmnt, codeMove,
            IR::Arg_Stk(), std::move(idx));

         // Reset iterator for further translation.
         stmnt = stmnt->prev->prev;

         idx = IR::Arg_Stk();

         return true;
      }
   }
}

// EOF

