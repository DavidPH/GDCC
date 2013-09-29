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

#ifndef GDCC__AST__Statement__Exp_H__
#define GDCC__AST__Statement__Exp_H__

#include "../Statement.hpp"


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
         GDCC_CounterPreamble(GDCC::AST::Statement_Exp, GDCC::AST::Statement);

      public:
         friend Statement::Ref StatementCreate_Exp(Array<String> const &labels,
            Origin pos, Exp *exp);
         friend Statement::Ref StatementCreate_Exp(Array<String>      &&labels,
            Origin pos, Exp *exp);

         CounterRef<Exp> const exp;

      protected:
         Statement_Exp(Array<String> const &labels, Origin pos, Exp *exp);
         Statement_Exp(Array<String>      &&labels, Origin pos, Exp *exp);
         virtual ~Statement_Exp();

         virtual void v_genStmnt(IR::Block &block, Function *fn) const;

         virtual bool v_isEffect() const;
      };
   }
}

#endif//GDCC__AST__Statement__Exp_H__

