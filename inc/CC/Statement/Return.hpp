//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C return statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Return_H__
#define GDCC__CC__Statement__Return_H__

#include "../../CC/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_ReturnExp
      //
      class Statement_ReturnExp : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_ReturnExp, GDCC::AST::Statement);

      public:
         Core::CounterRef<AST::Exp const> const exp;


         friend AST::Statement::CRef StatementCreate_Return(
            Core::Array<Core::String> const &labels, Core::Origin pos,
            AST::Function const *fn, AST::Exp const *exp);
         friend AST::Statement::CRef StatementCreate_Return(
            Core::Array<Core::String>      &&labels, Core::Origin pos,
            AST::Function const *fn, AST::Exp const *exp);
         friend AST::Statement::CRef StatementCreate_Return(Core::Origin pos,
            AST::Function const *fn, AST::Exp const *exp);

      protected:
         Statement_ReturnExp(Core::Array<Core::String> const &labels,
            Core::Origin pos, AST::Exp const *exp);
         Statement_ReturnExp(Core::Array<Core::String> &&labels,
            Core::Origin pos, AST::Exp const *exp);
         Statement_ReturnExp(Core::Origin pos, AST::Exp const *exp);
         virtual ~Statement_ReturnExp();

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn) const;

         virtual bool v_isEffect() const;
      };

      //
      // Statement_ReturnNul
      //
      class Statement_ReturnNul : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_ReturnNul, GDCC::AST::Statement);

      public:
         friend AST::Statement::CRef StatementCreate_Return(
            Core::Array<Core::String> const &labels, Core::Origin pos,
            AST::Function const *fn, AST::Exp const *exp);
         friend AST::Statement::CRef StatementCreate_Return(
            Core::Array<Core::String>      &&labels, Core::Origin pos,
            AST::Function const *fn, AST::Exp const *exp);
         friend AST::Statement::CRef StatementCreate_Return(Core::Origin pos,
            AST::Function const *fn, AST::Exp const *exp);

      protected:
         Statement_ReturnNul(Core::Array<Core::String> const &labels_,
            Core::Origin pos_) : Super{labels_, pos_} {}
         Statement_ReturnNul(Core::Array<Core::String> &&labels_,
            Core::Origin pos_) : Super{std::move(labels_), pos_} {}
         explicit Statement_ReturnNul(Core::Origin pos_) : Super{pos_} {}

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn) const;

         virtual bool v_isBranch() const {return true;}
         virtual bool v_isEffect() const {return false;}
      };
   }
}

#endif//GDCC__CC__Statement__Return_H__

