//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree statement base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Statement_H__
#define GDCC__AST__Statement_H__

#include "../Array.hpp"
#include "../Counter.hpp"
#include "../Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Block;
   }

   namespace AST
   {
      class Exp;
      class Function;

      //
      // Statement
      //
      class Statement : public Counter
      {
         GDCC_CounterPreambleAbstract(GDCC::AST::Statement, GDCC::Counter);

      public:
         void genLabel(IR::Block &block) const;
         void genStmnt(IR::Block &block, Function *fn) const;

         // Does this statement branch to another statement?
         bool isBranch() const {return v_isBranch();}

         // Does this statement have side effects?
         bool isEffect() const {return v_isEffect();}

         // Does this statement have or contain labels?
         bool isLabel() const {return !labels.empty() || v_isLabel();}

         // Can this statement forego codegen?
         bool isTrivial() const {return !isBranch() && !isEffect();}

         Array<String> const labels;
         Origin        const pos;

      protected:
         Statement(Array<String> const &labels_, Origin pos_) :
            labels{labels_}, pos{pos_} {}
         Statement(Array<String> &&labels_, Origin pos_) :
            labels{std::move(labels_)}, pos{pos_} {}

         virtual void v_genLabel(IR::Block &block) const;
         virtual void v_genStmnt(IR::Block &block, Function *fn) const = 0;

         virtual bool v_isBranch() const {return false;}
         virtual bool v_isEffect() const = 0;
         virtual bool v_isLabel()  const {return false;}
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      Statement::Ref StatementCreate_Empty(Array<String> const &labels, Origin pos);
      Statement::Ref StatementCreate_Empty(Array<String>      &&labels, Origin pos);

      Statement::Ref StatementCreate_Exp(Array<String> const &labels,
         Origin pos, Exp *exp);
      Statement::Ref StatementCreate_Exp(Array<String>      &&labels,
         Origin pos, Exp *exp);

      Statement::Ref StatementCreate_Multi(Array<String> const &labels,
         Origin pos, Array<Statement::Ref> const &stmnts);
      Statement::Ref StatementCreate_Multi(Array<String> const &labels,
         Origin pos, Array<Statement::Ref>      &&stmnts);
      Statement::Ref StatementCreate_Multi(Array<String>      &&labels,
         Origin pos, Array<Statement::Ref> const &stmnts);
      Statement::Ref StatementCreate_Multi(Array<String>      &&labels,
         Origin pos, Array<Statement::Ref>      &&stmnts);
   }
}

#endif//GDCC__AST__Statement_H__

