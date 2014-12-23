//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C goto, break, and continue statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Goto.hpp"

#include "CC/Scope/Local.hpp"

#include "Core/Exception.hpp"

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

