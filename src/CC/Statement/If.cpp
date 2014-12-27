//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C if statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/If.hpp"

#include "CC/Exp.hpp"

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
      // Statement_If contructor
      //
      Statement_If::Statement_If(Labels const &labels_, Core::Origin pos_,
         AST::Exp const *cond_, AST::Statement const *bodyT_,
         AST::Statement const *bodyF_) :
         Super{labels_, pos_},

         cond {cond_},
         bodyT{bodyT_},
         bodyF{bodyF_}
      {
      }

      //
      // Statement_If constructor
      //
      Statement_If::Statement_If(Labels &&labels_, Core::Origin pos_,
         AST::Exp const *cond_, AST::Statement const *bodyT_,
         AST::Statement const *bodyF_) :
         Super{std::move(labels_), pos_},

         cond {cond_},
         bodyT{bodyT_},
         bodyF{bodyF_}
      {
      }

      //
      // Statement_If::v_genLabel
      //
      void Statement_If::v_genLabel(IR::Block &block) const
      {
         bodyT->genLabel(block);
         if(bodyF) bodyF->genLabel(block);
      }

      //
      // Statement_If::v_genStmnt
      //
      void Statement_If::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         bool needT = !cond->isZero() || bodyT->isLabel();
         bool needF = bodyF && (!cond->isNonzero() || bodyF->isLabel());

         if(!needT && !needF)
         {
            cond->genStmnt(ctx);
            return;
         }

         IR::Glyph labelT{ctx.prog, nullptr};

         // Generate condition.
         if(cond->isNonzero())
         {
            cond->genStmnt(ctx);
         }
         else if(cond->isZero())
         {
            cond->genStmnt(ctx);

            if(needT)
               ctx.block.addStatementArgs({IR::Code::Jump, 0},
                  labelT = {ctx.prog, ctx.fn->genLabel()});
         }
         else
         {
            cond->genStmntStk(ctx);

            ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1}, IR::Arg_Stk(),
               labelT = {ctx.prog, ctx.fn->genLabel()});
         }

         // Generate true body.
         if(needT)
            bodyT->genStmnt(ctx);

         // Generate false body.
         if(needF)
         {
            // Only branch if there is anything to branch around.
            if(!bodyF->isTrivial())
            {
               IR::Glyph labelF = {ctx.prog, ctx.fn->genLabel()};
               ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelF);

               if(labelT) ctx.block.addLabel(labelT);
               bodyF->genStmnt(ctx);
               ctx.block.addLabel(labelF);
            }
            else
            {
               if(labelT) ctx.block.addLabel(labelT);
               bodyF->genLabel(ctx.block);
            }
         }
         else
            if(labelT) ctx.block.addLabel(labelT);
      }

      //
      // Statement_If::v_isBranch
      //
      bool Statement_If::v_isBranch() const
      {
         return bodyT->isBranch() || (bodyF && bodyF->isBranch());
      }

      //
      // Statement_If::v_isEffect
      //
      bool Statement_If::v_isEffect() const
      {
         return cond->isEffect() || bodyT->isEffect() || (bodyF && bodyF->isEffect());
      }

      //
      // Statement_If::v_isLabel
      //
      bool Statement_If::v_isLabel() const
      {
         return bodyT->isLabel() || (bodyF && bodyF->isLabel());
      }

      //
      // Statement_If::v_isNoAuto
      //
      bool Statement_If::v_isNoAuto() const
      {
         return cond->isNoAuto() && bodyT->isNoAuto() && (!bodyF || bodyF->isNoAuto());
      }

      //
      // Statement_If::v_isNoReturn
      //
      bool Statement_If::v_isNoReturn() const
      {
         return bodyT->isNoReturn() && (!bodyF || bodyF->isNoReturn());
      }

      //
      // StatementCreate_If
      //
      AST::Statement::CRef StatementCreate_If(
         AST::Statement::Labels const &labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *body)
      {
         return Statement_If::Create(
            labels, pos, ExpPromo_Cond(cond, pos), body);
      }

      //
      // StatementCreate_If
      //
      AST::Statement::CRef StatementCreate_If(AST::Statement::Labels &&labels,
         Core::Origin pos, AST::Exp const *cond, AST::Statement const *body)
      {
         return Statement_If::Create(
            std::move(labels), pos, ExpPromo_Cond(cond, pos), body);
      }

      //
      // StatementCreate_If
      //
      AST::Statement::CRef StatementCreate_If(
         AST::Statement::Labels const &labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *bodyT,
         AST::Statement const *bodyF)
      {
         return Statement_If::Create(
            labels, pos, ExpPromo_Cond(cond, pos), bodyT, bodyF);
      }

      //
      // StatementCreate_If
      //
      AST::Statement::CRef StatementCreate_If(AST::Statement::Labels &&labels,
         Core::Origin pos, AST::Exp const *cond, AST::Statement const *bodyT,
         AST::Statement const *bodyF)
      {
         return Statement_If::Create(
            std::move(labels), pos, ExpPromo_Cond(cond, pos), bodyT, bodyF);
      }
   }
}

// EOF

