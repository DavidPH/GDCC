//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression statement.
//
//-----------------------------------------------------------------------------

#include "Exp.hpp"

#include "../Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Statement_Exp constructor
      //
      Statement_Exp::Statement_Exp(Array<String> const &labels_, Origin pos_,
         Exp *exp_) : Super{labels_, pos_}, exp{exp_}
      {
      }

      //
      // Statement_Exp constructor
      //
      Statement_Exp::Statement_Exp(Array<String> &&labels_, Origin pos_,
         Exp *exp_) : Super{std::move(labels_), pos_}, exp{exp_}
      {
      }

      //
      // Statement_Exp destructor
      //
      Statement_Exp::~Statement_Exp()
      {
      }

      //
      // Statement_Exp::v_genStmnt
      //
      void Statement_Exp::v_genStmnt(IR::Block &block, Function *fn) const
      {
         exp->genStmnt(block, fn);
      }

      //
      // Statement_Exp::v_isEffect
      //
      bool Statement_Exp::v_isEffect() const
      {
         return exp->isEffect();
      }

      //
      // StatementCreate_Exp
      //
      Statement::Ref StatementCreate_Exp(Array<String> const &labels,
         Origin pos, Exp *exp)
      {
         return static_cast<Statement::Ref>(new Statement_Exp(labels, pos, exp));
      }

      //
      // StatementCreate_Exp
      //
      Statement::Ref StatementCreate_Exp(Array<String> &&labels, Origin pos, Exp *exp)
      {
         return static_cast<Statement::Ref>(
            new Statement_Exp(std::move(labels), pos, exp));
      }
   }
}

// EOF

