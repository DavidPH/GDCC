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

#ifndef GDCC__AST__Statement__Multi_H__
#define GDCC__AST__Statement__Multi_H__

#include "../Statement.hpp"


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
         GDCC_CounterPreamble(GDCC::AST::Statement_Multi, GDCC::AST::Statement);

      public:
         friend Statement::Ref StatementCreate_Multi(Array<String> const &labels,
            Origin pos, Array<Statement::Ref> const &stmnts);
         friend Statement::Ref StatementCreate_Multi(Array<String> const &labels,
            Origin pos, Array<Statement::Ref>      &&stmnts);
         friend Statement::Ref StatementCreate_Multi(Array<String>      &&labels,
            Origin pos, Array<Statement::Ref> const &stmnts);
         friend Statement::Ref StatementCreate_Multi(Array<String>      &&labels,
            Origin pos, Array<Statement::Ref>      &&stmnts);

         Array<Statement::Ref> const stmnts;

      protected:
         Statement_Multi(Array<String> const &labels_, Origin pos_,
            Array<Statement::Ref> const &stmnts_) : Super{labels_, pos_},
            stmnts{stmnts_} {}
         Statement_Multi(Array<String> const &labels_, Origin pos_,
            Array<Statement::Ref> &&stmnts_) : Super{labels_, pos_},
            stmnts{std::move(stmnts_)} {}
         Statement_Multi(Array<String> &&labels_, Origin pos_,
            Array<Statement::Ref> const &stmnts_) :
            Super{std::move(labels_), pos_}, stmnts{stmnts_} {}
         Statement_Multi(Array<String> &&labels_, Origin pos_,
            Array<Statement::Ref> &&stmnts_) : Super{std::move(labels_), pos_},
            stmnts{std::move(stmnts_)} {}

         virtual void v_genLabel(IR::Block &block) const;
         virtual void v_genStmnt(IR::Block &block, Function *fn) const;

         virtual bool v_isBranch() const;
         virtual bool v_isEffect() const;
         virtual bool v_isLabel()  const;
      };
   }
}

#endif//GDCC__AST__Statement__Multi_H__

