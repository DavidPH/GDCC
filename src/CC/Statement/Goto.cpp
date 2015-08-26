//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C goto, break, and continue statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Goto.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Local.hpp"

#include "AST/Arg.hpp"
#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_Goto constructor
      //
      Statement_Goto::Statement_Goto(Labels const &labels_, Core::Origin pos_,
         Core::String label_) :
         Super{labels_, pos_},

         label{label_}
      {
      }

      //
      // Statement_Goto constructor
      //
      Statement_Goto::Statement_Goto(Labels &&labels_, Core::Origin pos_,
         Core::String label_) :
         Super{std::move(labels_), pos_},

         label{label_}
      {
      }

      //
      // Statement_Goto::v_genStmnt
      //
      void Statement_Goto::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, IR::Glyph(ctx.prog, label));
      }

      //
      // Statement_GotoDyn constructor
      //
      Statement_GotoDyn::Statement_GotoDyn(Labels &&labels_, Core::Origin pos_,
         AST::Exp const *exp_) :
         Super{std::move(labels_), pos_},

         exp{exp_}
      {
      }

      //
      // Statement_GotoDyn destructor
      //
      Statement_GotoDyn::~Statement_GotoDyn()
      {
      }

      //
      // Statement_GotoDyn::v_genStmnt
      //
      void Statement_GotoDyn::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         if(exp->getArg().isIRArg())
         {
            ctx.block.addStatementArgs({IR::Code::Jdyn, 0}, exp->getArg().getIRArg(ctx.prog));
         }
         else
         {
            exp->genStmntStk(ctx);
            ctx.block.addStatementArgs({IR::Code::Jdyn, 0}, IR::Arg_Stk());
         }
      }

      //
      // Statement_GotoDyn::v_isNoAuto
      //
      bool Statement_GotoDyn::v_isNoAuto() const
      {
         return exp->isNoAuto();
      }

      //
      // StatementCreate_Break
      //
      AST::Statement::CRef StatementCreate_Break(
         AST::Statement::Labels const &labels, Core::Origin pos,
         Scope_Local &ctx)
      {
         if(auto label = ctx.getLabelBreak())
            return Statement_Goto::Create(labels, pos, label);

         throw Core::ExceptStr(pos, "invalid break");
      }

      //
      // StatementCreate_Break
      //
      AST::Statement::CRef StatementCreate_Break(
         AST::Statement::Labels &&labels, Core::Origin pos, Scope_Local &ctx)
      {
         if(auto label = ctx.getLabelBreak())
            return Statement_Goto::Create(std::move(labels), pos, label);

         throw Core::ExceptStr(pos, "invalid break");
      }

      //
      // StatementCreate_Continue
      //
      AST::Statement::CRef StatementCreate_Continue(
         AST::Statement::Labels const &labels, Core::Origin pos,
         Scope_Local &ctx)
      {
         if(auto label = ctx.getLabelContinue())
            return Statement_Goto::Create(labels, pos, label);

         throw Core::ExceptStr(pos, "invalid continue");
      }

      //
      // StatementCreate_Continue
      //
      AST::Statement::CRef StatementCreate_Continue(
         AST::Statement::Labels &&labels, Core::Origin pos, Scope_Local &ctx)
      {
         if(auto label = ctx.getLabelContinue())
            return Statement_Goto::Create(std::move(labels), pos, label);

         throw Core::ExceptStr(pos, "invalid continue");
      }

      //
      // StatementCreate_Goto
      //
      AST::Statement::CRef StatementCreate_Goto(AST::Statement::Labels &&labels,
          Core::Origin pos, AST::Exp const *exp_)
      {
         auto exp = ExpPromo_Assign(AST::Type::Label->getTypePointer(), exp_, pos);

         return Statement_GotoDyn::Create(std::move(labels), pos, exp);
      }

      //
      // StatementCreate_Goto
      //
      AST::Statement::CRef StatementCreate_Goto(
         AST::Statement::Labels const &labels, Core::Origin pos,
         Scope_Local &ctx, Core::String name)
      {
         return Statement_Goto::Create(labels, pos, ctx.getLabel(name));
      }

      //
      // StatementCreate_Goto
      //
      AST::Statement::CRef StatementCreate_Goto(
         AST::Statement::Labels &&labels, Core::Origin pos, Scope_Local &ctx,
         Core::String name)
      {
         return Statement_Goto::Create(std::move(labels), pos, ctx.getLabel(name));
      }
   }
}

// EOF

