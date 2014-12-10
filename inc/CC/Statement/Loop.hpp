//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C do, for, and while statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Loop_H__
#define GDCC__CC__Statement__Loop_H__

#include "../../CC/Statement.hpp"

#include "../../AST/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_Loop
      //
      class Statement_Loop : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_Loop, GDCC::AST::Statement);

      public:
         Scope_Local                           &scope;
         AST::Statement::CPtr             const init;
         Core::CounterRef<AST::Exp const> const cond;
         AST::Statement::CPtr             const iter;
         AST::Statement::CRef             const body;
         bool                             const post : 1;


         //
         // Create
         //
         static CRef Create(Labels const &labels, Core::Origin pos,
            Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
            AST::Statement const *iter, AST::Statement const *body, bool post)
         {
            return CRef(new This(labels, pos, ctx,
               init, cond, iter, body, post));
         }

         //
         // Create
         //
         static CRef Create(Labels &&labels, Core::Origin pos,
            Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
            AST::Statement const *iter, AST::Statement const *body, bool post)
         {
            return CRef(new This(std::move(labels), pos, ctx,
               init, cond, iter, body, post));
         }

      protected:
         Statement_Loop(Labels const &labels, Core::Origin pos,
            Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
            AST::Statement const *iter, AST::Statement const *body, bool post);
         Statement_Loop(Labels &&labels, Core::Origin pos,
            Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
            AST::Statement const *iter, AST::Statement const *body, bool post);

         virtual void v_genLabel(IR::Block &block) const;
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const;
         virtual bool v_isEffect() const;
         virtual bool v_isLabel()  const;
         virtual bool v_isNoAuto() const;
         virtual bool v_isReturn() const;
      };
   }
}

#endif//GDCC__CC__Statement__Loop_H__

