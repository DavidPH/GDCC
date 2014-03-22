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

#ifndef GDCC__AST__Statement__Exp_H__
#define GDCC__AST__Statement__Exp_H__

#include "../../AST/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Statement_Exp
      //
      class Statement_Exp final : public Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Statement_Exp, GDCC::AST::Statement);

      public:
         friend Statement::CRef StatementCreate_Exp(
            Core::Array<Core::String> const &labels, Core::Origin pos,
            Exp const *exp);
         friend Statement::CRef StatementCreate_Exp(
            Core::Array<Core::String> &&labels, Core::Origin pos,
            Exp const *exp);
         friend Statement::CRef StatementCreate_Exp(Core::Origin pos,
            Exp const *exp);
         friend Statement::CRef StatementCreate_Exp(Exp const *exp);

         Core::CounterRef<Exp const> const exp;

      protected:
         Statement_Exp(Core::Array<Core::String> const &labels,
            Core::Origin pos, Exp const *exp);
         Statement_Exp(Core::Array<Core::String>      &&labels,
            Core::Origin pos, Exp const *exp);
         Statement_Exp(Core::Origin pos, Exp const *exp);
         virtual ~Statement_Exp();

         virtual void v_genStmnt(GenStmntCtx const &ctx) const;

         virtual bool v_isEffect() const;
      };
   }
}

#endif//GDCC__AST__Statement__Exp_H__

