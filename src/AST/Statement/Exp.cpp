//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression statement.
//
//-----------------------------------------------------------------------------

#include "AST/Statement/Exp.hpp"

#include "AST/Exp.hpp"


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
      Statement_Exp::Statement_Exp(Core::Array<Core::String> const &labels_,
         Core::Origin pos_, Exp const *exp_) :
         Super{labels_, pos_}, exp{exp_}
      {
      }

      //
      // Statement_Exp constructor
      //
      Statement_Exp::Statement_Exp(Core::Array<Core::String> &&labels_,
         Core::Origin pos_, Exp const *exp_) :
         Super{std::move(labels_), pos_}, exp{exp_}
      {
      }

      //
      // Statement_Exp constructor
      //
      Statement_Exp::Statement_Exp(Core::Origin pos_, Exp const *exp_) :
         Super{pos_}, exp{exp_}
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
      Statement::CRef StatementCreate_Exp(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Exp const *exp)
      {
         return static_cast<Statement::CRef>(
            new Statement_Exp(labels, pos, exp));
      }

      //
      // StatementCreate_Exp
      //
      Statement::CRef StatementCreate_Exp(Core::Array<Core::String> &&labels,
         Core::Origin pos, Exp const *exp)
      {
         return static_cast<Statement::CRef>(
            new Statement_Exp(std::move(labels), pos, exp));
      }

      //
      // StatementCreate_Exp
      //
      Statement::CRef StatementCreate_Exp(Core::Origin pos, Exp const *exp)
      {
         return static_cast<Statement::CRef>(new Statement_Exp(pos, exp));
      }

      //
      // StatementCreate_Exp
      //
      Statement::CRef StatementCreate_Exp(Exp const *exp)
      {
         return static_cast<Statement::CRef>(new Statement_Exp(exp->pos, exp));
      }
   }
}

// EOF

