//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C goto, break, and continue statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Goto.hpp"

#include "CC/Factory.hpp"
#include "CC/Scope/Local.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"

#include "SR/Arg.hpp"
#include "SR/Exp.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
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
   void Statement_Goto::v_genStmnt(SR::GenStmntCtx const &ctx) const
   {
      ctx.block.setArgSize().addStmnt(IR::CodeBase::Jump, IR::Glyph(ctx.prog, label));
   }

   //
   // Statement_GotoDyn constructor
   //
   Statement_GotoDyn::Statement_GotoDyn(Labels &&labels_, Core::Origin pos_,
      SR::Exp const *exp_) :
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
   void Statement_GotoDyn::v_genStmnt(SR::GenStmntCtx const &ctx) const
   {
      if(exp->getArg().isIRArg())
      {
         ctx.block.addStmnt(IR::CodeBase::Jdyn, exp->getArg().getIRArg(ctx.prog));
      }
      else
      {
         exp->genStmntStk(ctx);
         ctx.block.setArgSize().addStmnt(IR::CodeBase::Jdyn, IR::Block::Stk());
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
   // Factory::stCreate_Break
   //
   SR::Statement::CRef Factory::stCreate_Break(
      SR::Statement::Labels &&labels, Core::Origin pos, Scope_Local &ctx)
   {
      if(auto label = ctx.getLabelBreak())
         return Statement_Goto::Create(std::move(labels), pos, label);

      Core::Error(pos, "invalid break");
   }

   //
   // Factory::stCreate_Continue
   //
   SR::Statement::CRef Factory::stCreate_Continue(
      SR::Statement::Labels &&labels, Core::Origin pos, Scope_Local &ctx)
   {
      if(auto label = ctx.getLabelContinue())
         return Statement_Goto::Create(std::move(labels), pos, label);

      Core::Error(pos, "invalid continue");
   }

   //
   // Factory::stCreate_Goto
   //
   SR::Statement::CRef Factory::stCreate_Goto(SR::Statement::Labels &&labels,
         Core::Origin pos, SR::Exp const *exp_)
   {
      auto exp = expPromo_Assign(SR::Type::Label->getTypePointer(), exp_, pos);

      return Statement_GotoDyn::Create(std::move(labels), pos, exp);
   }

   //
   // Factory::stCreate_Goto
   //
   SR::Statement::CRef Factory::stCreate_Goto(
      SR::Statement::Labels &&labels, Core::Origin pos, Scope_Local &ctx,
      Core::String name)
   {
      return Statement_Goto::Create(std::move(labels), pos, ctx.getLabel(name));
   }
}

// EOF

