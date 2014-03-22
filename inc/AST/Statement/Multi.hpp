//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree compound statement.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Statement__Multi_H__
#define GDCC__AST__Statement__Multi_H__

#include "../../AST/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Statement_Multi
      //
      class Statement_Multi final : public Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Statement_Multi, GDCC::AST::Statement);

      public:
         friend Statement::CRef StatementCreate_Multi(
            Core::Array<Core::String> const &labels, Core::Origin pos,
            Core::Array<Statement::CRef> const &stmnts);
         friend Statement::CRef StatementCreate_Multi(
            Core::Array<Core::String> const &labels, Core::Origin pos,
            Core::Array<Statement::CRef> &&stmnts);
         friend Statement::CRef StatementCreate_Multi(
            Core::Array<Core::String> &&labels, Core::Origin pos,
            Core::Array<Statement::CRef> const &stmnts);
         friend Statement::CRef StatementCreate_Multi(
            Core::Array<Core::String> &&labels, Core::Origin pos,
            Core::Array<Statement::CRef> &&stmnts);
         friend Statement::CRef StatementCreate_Multi(Core::Origin pos,
            Core::Array<Statement::CRef> const &stmnts);
         friend Statement::CRef StatementCreate_Multi(Core::Origin pos,
            Core::Array<Statement::CRef> &&stmnts);

         Core::Array<Statement::CRef> const stmnts;

      protected:
         Statement_Multi(Core::Array<Core::String> const &labels_,
            Core::Origin pos_, Core::Array<Statement::CRef> const &stmnts_) :
            Super{labels_, pos_}, stmnts{stmnts_} {}
         Statement_Multi(Core::Array<Core::String> const &labels_,
            Core::Origin pos_, Core::Array<Statement::CRef> &&stmnts_) :
            Super{labels_, pos_}, stmnts{std::move(stmnts_)} {}
         Statement_Multi(Core::Array<Core::String> &&labels_, Core::Origin pos_,
            Core::Array<Statement::CRef> const &stmnts_) :
            Super{std::move(labels_), pos_}, stmnts{stmnts_} {}
         Statement_Multi(Core::Array<Core::String> &&labels_, Core::Origin pos_,
            Core::Array<Statement::CRef> &&stmnts_) :
            Super{std::move(labels_), pos_}, stmnts{std::move(stmnts_)} {}
         Statement_Multi(Core::Origin pos_,
            Core::Array<Statement::CRef> const &stmnts_) :
            Super{pos_}, stmnts{stmnts_} {}
         Statement_Multi(Core::Origin pos_,
            Core::Array<Statement::CRef> &&stmnts_) :
            Super{pos_}, stmnts{std::move(stmnts_)} {}

         virtual void v_genLabel(IR::Block &block) const;
         virtual void v_genStmnt(GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const;
         virtual bool v_isEffect() const;
         virtual bool v_isLabel()  const;
      };
   }
}

#endif//GDCC__AST__Statement__Multi_H__

