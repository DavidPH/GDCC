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

#include "AST/Statement/Empty.hpp"


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
      Statement::Ref StatementCreate_Empty(
         Core::Array<Core::String> const &labels, Core::Origin pos)
      {
         return static_cast<Statement::Ref>(new Statement_Empty(labels, pos));
      }

      //
      // StatementCreate_Empty
      //
      Statement::Ref StatementCreate_Empty(Core::Array<Core::String> &&labels,
         Core::Origin pos)
      {
         return static_cast<Statement::Ref>(new Statement_Empty(std::move(labels), pos));
      }
   }
}

// EOF

