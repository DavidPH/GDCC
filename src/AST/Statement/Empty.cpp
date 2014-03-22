//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
      void Statement_Empty::v_genStmnt(GenStmntCtx const &) const
      {
      }

      //
      // StatementCreate_Empty
      //
      Statement::CRef StatementCreate_Empty(
         Core::Array<Core::String> const &labels, Core::Origin pos)
      {
         return static_cast<Statement::CRef>(new Statement_Empty(labels, pos));
      }

      //
      // StatementCreate_Empty
      //
      Statement::CRef StatementCreate_Empty(Core::Array<Core::String> &&labels,
         Core::Origin pos)
      {
         return static_cast<Statement::CRef>(
            new Statement_Empty(std::move(labels), pos));
      }

      //
      // StatementCreate_Empty
      //
      Statement::CRef StatementCreate_Empty(Core::Origin pos)
      {
         return static_cast<Statement::CRef>(new Statement_Empty(pos));
      }
   }
}

// EOF

