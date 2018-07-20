//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"

#include "SR/Exp.hpp"
#include "SR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Statement_If contructor
   //
   Statement_If::Statement_If(Labels const &labels_, Core::Origin pos_,
      SR::Exp const *cond_, SR::Statement const *bodyT_,
      SR::Statement const *bodyF_) :
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
      SR::Exp const *cond_, SR::Statement const *bodyT_,
      SR::Statement const *bodyF_) :
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
   void Statement_If::v_genStmnt(SR::GenStmntCtx const &ctx) const
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
            ctx.block.setArgSize().addStmnt(IR::Code::Jump,
               labelT = {ctx.prog, ctx.fn->genLabel()});
      }
      else
      {
         cond->genStmntStk(ctx);

         ctx.block.setArgSize().addStmnt(IR::Code::Jcnd_Nil,
            IR::Block::Stk(), labelT = {ctx.prog, ctx.fn->genLabel()});
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
            ctx.block.setArgSize().addStmnt(IR::Code::Jump, labelF);

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
   // Statement_If::v_isReturn
   //
   bool Statement_If::v_isReturn() const
   {
      return bodyF && bodyF->isReturn() && bodyT->isReturn();
   }

   //
   // StatementCreate_If
   //
   SR::Statement::CRef StatementCreate_If(
      SR::Statement::Labels const &labels, Core::Origin pos,
      SR::Exp const *cond, SR::Statement const *body)
   {
      return Statement_If::Create(
         labels, pos, ExpPromo_Cond(cond, pos), body);
   }

   //
   // StatementCreate_If
   //
   SR::Statement::CRef StatementCreate_If(SR::Statement::Labels &&labels,
      Core::Origin pos, SR::Exp const *cond, SR::Statement const *body)
   {
      return Statement_If::Create(
         std::move(labels), pos, ExpPromo_Cond(cond, pos), body);
   }

   //
   // StatementCreate_If
   //
   SR::Statement::CRef StatementCreate_If(
      SR::Statement::Labels const &labels, Core::Origin pos,
      SR::Exp const *cond, SR::Statement const *bodyT,
      SR::Statement const *bodyF)
   {
      return Statement_If::Create(
         labels, pos, ExpPromo_Cond(cond, pos), bodyT, bodyF);
   }

   //
   // StatementCreate_If
   //
   SR::Statement::CRef StatementCreate_If(SR::Statement::Labels &&labels,
      Core::Origin pos, SR::Exp const *cond, SR::Statement const *bodyT,
      SR::Statement const *bodyF)
   {
      return Statement_If::Create(
         std::move(labels), pos, ExpPromo_Cond(cond, pos), bodyT, bodyF);
   }
}

// EOF

