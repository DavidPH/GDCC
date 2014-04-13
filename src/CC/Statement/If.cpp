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
         Core::String labelT{Core::STRNULL};

         // Generate condition.
         if(cond->isNonzero())
         {
            cond->genStmnt(ctx);
         }
         else
         {
            cond->genStmntStk(ctx);

            labelT = ctx.fn->genLabel();
            IR::Glyph glyphT{&ctx.prog, labelT};
            ctx.block.addStatementArgs(IR::Code::Cjmp_Nil, IR::Arg_Stk(),
               IR::Arg_Lit(IR::ExpCreate_ValueGlyph(glyphT, pos)));
         }

         // Generate true body.
         bodyT->genStmnt(ctx);

         // Generate false body.
         if(bodyF)
         {
            // Only branch if there is anything to branch around.
            if(!bodyF->isTrivial())
            {
               Core::String labelF{ctx.fn->genLabel()};
               IR::Glyph    glyphF{&ctx.prog, labelF};
               ctx.block.addStatementArgs(IR::Code::Jump,
                  IR::Arg_Lit(IR::ExpCreate_ValueGlyph(glyphF, pos)));

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

