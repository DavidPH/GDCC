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

#include "CC/Statement/Loop.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Local.hpp"

#include "AST/Exp.hpp"
#include "AST/Function.hpp"

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_Loop constructor
      //
      Statement_Loop::Statement_Loop(Labels const &labels_, Core::Origin pos_,
         Scope_Local &ctx, AST::Statement const *init_, AST::Exp const *cond_,
         AST::Statement const *iter_, AST::Statement const *body_, bool post_) :
         Super{labels_, pos_}, scope(ctx),
         init{init_}, cond{cond_}, iter{iter_}, body{body_}, post{post_}
      {
      }

      //
      // Statement_Loop constructor
      //
      Statement_Loop::Statement_Loop(Labels &&labels_, Core::Origin pos_,
         Scope_Local &ctx, AST::Statement const *init_, AST::Exp const *cond_,
         AST::Statement const *iter_, AST::Statement const *body_, bool post_) :
         Super{std::move(labels_), pos_}, scope(ctx),
         init{init_}, cond{cond_}, iter{iter_}, body{body_}, post{post_}
      {
      }

      //
      // Statement_Loop::v_genLabel
      //
      void Statement_Loop::v_genLabel(IR::Block &block) const
      {
         body->genLabel(block);
      }

      //
      // Statement_Loop::v_genStmnt
      //
      void Statement_Loop::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         // Labels.
         IR::Glyph labelBody  = {ctx.prog, ctx.fn->genLabel()};
         IR::Glyph labelBreak = {ctx.prog, scope.getLabelBreak()};
         IR::Glyph labelCond  = {ctx.prog, ctx.fn->genLabel()};
         IR::Glyph labelCont  = {ctx.prog, scope.getLabelContinue()};

         // Generate initializer.
         if(init) init->genStmnt(ctx);

         // If not using post-condition, jump to the condition.
         // Putting the condition at the end of the loop is more efficient.
         // If condition is known to be true, then do not bother jumping to it.
         if(!post && !(cond->isNonzero() && !cond->isEffect()))
            ctx.block.addStatementArgs(IR::Code::Jump, labelCond);

         // Generate body.
         ctx.block.addLabel(labelBody);
         body->genStmnt(ctx);

         // Generate iterator.
         ctx.block.addLabel(labelCont);
         if(iter) iter->genStmnt(ctx);

         // Generate condition.
         ctx.block.addLabel(labelCond);
         if(cond->isZero())
         {
            cond->genStmnt(ctx);
         }
         else if(cond->isNonzero())
         {
            cond->genStmnt(ctx);
            ctx.block.addStatementArgs(IR::Code::Jump, labelBody);
         }
         else
         {
            cond->genStmntStk(ctx);
            ctx.block.addStatementArgs(IR::Code::Jcnd_Tru, IR::Arg_Stk(), labelBody);
         }

         // Generate terminator.
         ctx.block.addLabel(labelBreak);
      }

      //
      // Statement_Loop::v_isBranch
      //
      bool Statement_Loop::v_isBranch() const
      {
         return body->isBranch();
      }

      //
      // Statement_Loop::v_isEffect
      //
      bool Statement_Loop::v_isEffect() const
      {
         return (init && init->isEffect()) || (cond && cond->isEffect()) ||
                (iter && iter->isEffect()) || body->isEffect();
      }

      //
      // Statement_Loop::v_isLabel
      //
      bool Statement_Loop::v_isLabel() const
      {
         return body->isLabel();
      }

      //
      // Statement_Loop::v_isNoAuto
      //
      bool Statement_Loop::v_isNoAuto() const
      {
         return (!init || init->isNoAuto()) && (!cond || cond->isNoAuto()) &&
                (!iter || iter->isNoAuto()) && body->isNoAuto();
      }

      //
      // StatementCreate_Do
      //
      AST::Statement::CRef StatementCreate_Do(
         AST::Statement::Labels const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *body, AST::Exp const *cond)
      {
         return Statement_Loop::Create(labels, pos, ctx,
            nullptr, ExpPromo_Cond(cond, pos), nullptr, body, true);
      }

      //
      // StatementCreate_Do
      //
      AST::Statement::CRef StatementCreate_Do(
         AST::Statement::Labels &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *body, AST::Exp const *cond)
      {
         return Statement_Loop::Create(std::move(labels), pos, ctx,
            nullptr, ExpPromo_Cond(cond, pos), nullptr, body, true);
      }

      //
      // StatementCreate_For
      //
      AST::Statement::CRef StatementCreate_For(
         AST::Statement::Labels const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
         AST::Statement const *iter, AST::Statement const *body)
      {
         return Statement_Loop::Create(labels, pos, ctx,
            init, ExpPromo_Cond(cond, pos), iter, body, false);
      }

      //
      // StatementCreate_For
      //
      AST::Statement::CRef StatementCreate_For(
         AST::Statement::Labels &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
         AST::Statement const *iter, AST::Statement const *body)
      {
         return Statement_Loop::Create(std::move(labels), pos, ctx,
            init, ExpPromo_Cond(cond, pos), iter, body, false);
      }

      //
      // StatementCreate_While
      //
      AST::Statement::CRef StatementCreate_While(
         AST::Statement::Labels const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body)
      {
         return Statement_Loop::Create(labels, pos, ctx,
            nullptr, ExpPromo_Cond(cond, pos), nullptr, body, false);
      }

      //
      // StatementCreate_While
      //
      AST::Statement::CRef StatementCreate_While(
         AST::Statement::Labels &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body)
      {
         return Statement_Loop::Create(std::move(labels), pos, ctx,
            nullptr, ExpPromo_Cond(cond, pos), nullptr, body, false);
      }
   }
}

// EOF

