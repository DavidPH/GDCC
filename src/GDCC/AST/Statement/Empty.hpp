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

#include "../Statement.hpp"


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
         GDCC_CounterPreamble(GDCC::AST::Statement_Empty, GDCC::AST::Statement);

      public:
         friend Statement::Ref StatementCreate_Empty(
            Array<String> const &labels, Origin pos);
         friend Statement::Ref StatementCreate_Empty(
            Array<String>      &&labels, Origin pos);

      protected:
         Statement_Empty(Array<String> const &labels_, Origin pos_) :
            Super{labels_, pos_} {}
         Statement_Empty(Array<String> &&labels_, Origin pos_) :
            Super{std::move(labels_), pos_} {}

         virtual void v_genStmnt(IR::Block &block, Function *fn) const;

         virtual bool v_isEffect() const {return false;}
      };
   }
}

#endif//GDCC__AST__Statement__Empty_H__

