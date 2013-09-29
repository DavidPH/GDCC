//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree empty statement.
//
//-----------------------------------------------------------------------------

#include "Empty.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Statement_Empty::v_genStmnt
      //
      void Statement_Empty::v_genStmnt(IR::Block &, Function *) const
      {
      }

      //
      // StatementCreate_Empty
      //
      Statement::Ref StatementCreate_Empty(Array<String> const &labels, Origin pos)
      {
         return static_cast<Statement::Ref>(new Statement_Empty(labels, pos));
      }

      //
      // StatementCreate_Empty
      //
      Statement::Ref StatementCreate_Empty(Array<String> &&labels, Origin pos)
      {
         return static_cast<Statement::Ref>(new Statement_Empty(std::move(labels), pos));
      }
   }
}

// EOF

