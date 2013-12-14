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

#ifndef GDCC__AST__Statement__Empty_H__
#define GDCC__AST__Statement__Empty_H__

#include "../../AST/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Statement_Empty
      //
      class Statement_Empty final : public Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Statement_Empty, GDCC::AST::Statement);

      public:
         friend Statement::CRef StatementCreate_Empty(
            Core::Array<Core::String> const &labels, Core::Origin pos);
         friend Statement::CRef StatementCreate_Empty(
            Core::Array<Core::String>      &&labels, Core::Origin pos);

      protected:
         Statement_Empty(Core::Array<Core::String> const &labels_, Core::Origin pos_) :
            Super{labels_, pos_} {}
         Statement_Empty(Core::Array<Core::String> &&labels_, Core::Origin pos_) :
            Super{std::move(labels_), pos_} {}

         virtual void v_genStmnt(IR::Block &block, Function *fn) const;

         virtual bool v_isEffect() const {return false;}
      };
   }
}

#endif//GDCC__AST__Statement__Empty_H__

