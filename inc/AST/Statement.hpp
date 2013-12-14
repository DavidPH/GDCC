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

#include "../Core/Array.hpp"
#include "../Core/Counter.hpp"
#include "../Core/Origin.hpp"


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
      class Statement : public Core::Counter
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Statement, GDCC::Core::Counter);

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

         Core::Array<Core::String> const labels;
         Core::Origin              const pos;

      protected:
         Statement(Core::Array<Core::String> const &labels_, Core::Origin pos_) :
            labels{labels_}, pos{pos_} {}
         Statement(Core::Array<Core::String> &&labels_, Core::Origin pos_) :
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
      Statement::Ref StatementCreate_Empty(
         Core::Array<Core::String> const &labels, Core::Origin pos);
      Statement::Ref StatementCreate_Empty(
         Core::Array<Core::String>      &&labels, Core::Origin pos);

      Statement::Ref StatementCreate_Exp(
         Core::Array<Core::String> const &labels, Core::Origin pos, Exp *exp);
      Statement::Ref StatementCreate_Exp(
         Core::Array<Core::String>      &&labels, Core::Origin pos, Exp *exp);

      Statement::Ref StatementCreate_Multi(Core::Array<Core::String> const &labels,
         Core::Origin pos, Core::Array<Statement::Ref> const &stmnts);
      Statement::Ref StatementCreate_Multi(Core::Array<Core::String> const &labels,
         Core::Origin pos, Core::Array<Statement::Ref>      &&stmnts);
      Statement::Ref StatementCreate_Multi(Core::Array<Core::String>      &&labels,
         Core::Origin pos, Core::Array<Statement::Ref> const &stmnts);
      Statement::Ref StatementCreate_Multi(Core::Array<Core::String>      &&labels,
         Core::Origin pos, Core::Array<Statement::Ref>      &&stmnts);
   }
}

#endif//GDCC__AST__Statement_H__

