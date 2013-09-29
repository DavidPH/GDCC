//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree compound statement.
//
//-----------------------------------------------------------------------------

#include "Multi.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Statement_Multi::v_genLabel
      //
      void Statement_Multi::v_genLabel(IR::Block &block) const
      {
         for(auto const &stmnt : stmnts)
            stmnt->genLabel(block);
      }

      //
      // Statement_Multi::v_genStmnt
      //
      void Statement_Multi::v_genStmnt(IR::Block &block, Function *fn) const
      {
         for(auto const &stmnt : stmnts)
            stmnt->genStmnt(block, fn);
      }

      //
      // Statement_Multi::v_isBranch
      //
      bool Statement_Multi::v_isBranch() const
      {
         for(auto const &stmnt : stmnts)
            if(stmnt->isBranch()) return true;

         return false;
      }

      //
      // Statement_Multi::v_isEffect
      //
      bool Statement_Multi::v_isEffect() const
      {
         for(auto const &stmnt : stmnts)
            if(stmnt->isEffect()) return true;

         return false;
      }

      //
      // Statement_Multi::v_isLabel
      //
      bool Statement_Multi::v_isLabel() const
      {
         for(auto const &stmnt : stmnts)
            if(stmnt->isLabel()) return true;

         return false;
      }

      //
      // StatementCreate_Multi
      //
      Statement::Ref StatementCreate_Multi(Array<String> const &labels,
         Origin pos, Array<Statement::Ref> const &stmnts)
      {
         return static_cast<Statement::Ref>(
            new Statement_Multi(labels, pos, stmnts));
      }

      //
      // StatementCreate_Multi
      //
      Statement::Ref StatementCreate_Multi(Array<String> const &labels,
         Origin pos, Array<Statement::Ref> &&stmnts)
      {
         return static_cast<Statement::Ref>(
            new Statement_Multi(labels, pos, std::move(stmnts)));
      }

      //
      // StatementCreate_Multi
      //
      Statement::Ref StatementCreate_Multi(Array<String> &&labels, Origin pos,
         Array<Statement::Ref> const &stmnts)
      {
         return static_cast<Statement::Ref>(
            new Statement_Multi(std::move(labels), pos, stmnts));
      }

      //
      // StatementCreate_Multi
      //
      Statement::Ref StatementCreate_Multi(Array<String> &&labels, Origin pos,
         Array<Statement::Ref> &&stmnts)
      {
         return static_cast<Statement::Ref>(
            new Statement_Multi(std::move(labels), pos, std::move(stmnts)));
      }
   }
}

// EOF

