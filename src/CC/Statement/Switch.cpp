//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C switch statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Switch.hpp"

#include "CC/Exp/Arith.hpp"
#include "CC/Scope/Case.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/CodeSet/Cmp.hpp"
#include "IR/Glyph.hpp"

#include "SR/Exp.hpp"
#include "SR/Function.hpp"
#include "SR/Temporary.hpp"
#include "SR/Type.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   using CodePair = std::pair<IR::Code, IR::Code>;
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GenCond_Codes
   //
   static CodePair GenCond_Codes(SR::Statement const *, SR::Type const *t)
   {
      return {SR::ExpCode_ArithInteg<IR::CodeSet_CmpLT>(t),
               SR::ExpCode_ArithInteg<IR::CodeSet_CmpEQ>(t)};
   }

   //
   // GenCond_GenValue
   //
   static IR::Exp::CRef GenCond_GenValue(Statement_Switch const *stmnt,
      Scope_Case::Case const *caseData)
   {
      return IR::ExpCreate_Value(
         IR::Value_Fixed(
            caseData->value, stmnt->cond->getType()->getIRType().tFixed),
         stmnt->pos);
   }

   //
   // GenCond_BranchDefault
   //
   static void GenCond_BranchDefault(Statement_Switch const *stmnt,
      SR::GenStmntCtx const &ctx)
   {
      IR::Glyph defLabel = {ctx.prog, stmnt->scope.getLabelDefault(false)};

      // Unconditional branch.
      ctx.block.setArgSize().addStmnt(IR::Code::Jump, defLabel);
   }

   //
   // GenCond_SearchPart
   //
   static void GenCond_SearchPart(
      Statement_Switch        const *stmnt,
      SR::GenStmntCtx         const &ctx,
      CodePair                const &codes,
      SR::Temporary           const &cond,
      Scope_Case::Case const *const *begin,
      Scope_Case::Case const *const *end)
   {
      auto count = end - begin;

      if(count == 0)
      {
         GenCond_BranchDefault(stmnt, ctx);

         return;
      }

      if(count == 1)
      {
         IR::Glyph caseLabel = {ctx.prog, begin[0]->label};

         // If this case is one more than the previous and one less than the
         // next, then there is no need for further comparisons.
         if(begin[-1] && begin[+1] && begin[-1]->value + 1 == begin[+1]->value - 1)
            ctx.block.setArgSize().addStmnt(IR::Code::Jump, caseLabel);

         // Otherwise, check against value.
         else
         {
            auto caseValue = GenCond_GenValue(stmnt, begin[0]);

            ctx.block.setArgSize();

            // Compare condition to case value for equality.
            ctx.block.addStmnt(codes.second,
               IR::Block::Stk(), cond.getArg(), caseValue);

            // If true, branch to case.
            ctx.block.addStmnt(IR::Code::Jcnd_Tru, IR::Block::Stk(), caseLabel);

            // Otherwise, branch to default.
            GenCond_BranchDefault(stmnt, ctx);
         }

         return;
      }

      auto pivot = &begin[count / 2];

      IR::Glyph pivotLabel = {ctx.prog, ctx.fn->genLabel()};
      auto      pivotValue = GenCond_GenValue(stmnt, *pivot);

      ctx.block.setArgSize();

      // Compare condition against pivot.
      ctx.block.addStmnt(codes.first,
         IR::Block::Stk(), cond.getArg(), IR::Arg_Lit(cond.sizeBytes(), pivotValue));

      // Skip if false.
      ctx.block.addStmnt(IR::Code::Jcnd_Nil, IR::Block::Stk(), pivotLabel);

      // Search left half.
      GenCond_SearchPart(stmnt, ctx, codes, cond, begin, pivot);

      // Search right half.
      ctx.block.addLabel(pivotLabel);
      GenCond_SearchPart(stmnt, ctx, codes, cond, pivot, end);
   }

   //
   // GenCond_Search
   //
   static void GenCond_Search(Statement_Switch const *stmnt,
      SR::GenStmntCtx const &ctx, CodePair const &codes)
   {
      // Collect and sort cases.
      std::vector<Scope_Case::Case const *> cases;
      cases.reserve(stmnt->scope.size() + 2);

      cases.emplace_back(nullptr);
      for(auto const &c : stmnt->scope)
         cases.emplace_back(&c);
      cases.emplace_back(nullptr);

      std::sort(cases.begin() + 1, cases.end() - 1,
         [](Scope_Case::Case const *l, Scope_Case::Case const *r)
            {return l->value < r->value;});

      // Evaluate condition and store in temporary.
      stmnt->cond->genStmntStk(ctx);

      SR::Temporary tmp{ctx, stmnt->pos, stmnt->cond->getType()->getSizeWords()};
      ctx.block.addStmnt(IR::Code::Move, tmp.getArg(), tmp.getArgStk());

      // Begin binary search.
      GenCond_SearchPart(stmnt, ctx, codes, tmp, cases.data() + 1,
         cases.data() + cases.size() - 1);
   }

   //
   // GenCond_Search_Jcnd_Tab
   //
   static void GenCond_Search_Jcnd_Tab(Statement_Switch const *stmnt,
      SR::GenStmntCtx const &ctx)
   {
      Core::FastU caseSize = stmnt->cond->getType()->getSizeBytes();

      Core::Array<IR::Arg> args{stmnt->scope.size() * 2 + 1};
      auto argItr = args.begin();

      // Evaluate condition.
      stmnt->cond->genStmntStk(ctx);
      *argItr++ = IR::Arg_Stk(caseSize);

      // Determine IR type of case values.
      auto caseType = stmnt->cond->getType()->getIRType().tFixed;

      // Generate cases. Sorting is handled by the BC module, as needed.
      for(auto const &c : stmnt->scope)
      {
         *argItr++ = IR::Arg_Lit(caseSize, IR::ExpCreate_Value(
            IR::Value_Fixed{c.value, caseType}, stmnt->cond->pos));
         *argItr++ = IR::Arg_Lit(caseSize, ctx.block.getExp({ctx.prog, c.label}));
      }

      ctx.block.addStmntArgs(IR::Code::Jcnd_Tab, std::move(args));
      ctx.block.addStmnt(IR::Code::Move,
         IR::Arg_Nul(caseSize), IR::Arg_Stk(caseSize));
      GenCond_BranchDefault(stmnt, ctx);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Statement_Switch constructor
   //
   Statement_Switch::Statement_Switch(Labels const &labels_,
      Core::Origin pos_, Scope_Case &scope_, SR::Exp const *cond_,
      SR::Statement const *body_) :
      Super{labels_, pos_}, scope(scope_), cond{cond_}, body{body_}
   {
   }

   //
   // Statement_Switch constructor
   //
   Statement_Switch::Statement_Switch(Labels &&labels_, Core::Origin pos_,
      Scope_Case &scope_, SR::Exp const *cond_, SR::Statement const *body_) :
      Super{std::move(labels_), pos_}, scope(scope_), cond{cond_}, body{body_}
   {
   }

   //
   // Statement_Switch::v_genLabel
   //
   void Statement_Switch::v_genLabel(IR::Block &block) const
   {
      body->genLabel(block);
   }

   //
   // Statement_Switch::v_genStmnt
   //
   void Statement_Switch::v_genStmnt(SR::GenStmntCtx const &ctx) const
   {
      // Generate condition.
      if(Target::IsFamily_ZDACS() && cond->getType()->getSizeWords() == 1)
         GenCond_Search_Jcnd_Tab(this, ctx);
      else
         GenCond_Search(this, ctx, GenCond_Codes(this, cond->getType()));

      // Generate body.
      body->genStmnt(ctx);

      // Generate implicit default label, if needed.
      if(!scope.getDefault().defin)
         ctx.block.addLabel(scope.getDefault().label);

      // Add break label.
      ctx.block.addLabel(scope.getLabelBreak());
   }

   //
   // Statement_Switch::v_isBranch
   //
   bool Statement_Switch::v_isBranch() const
   {
      return body->isBranch();
   }

   //
   // Statement_Switch::v_isEffect
   //
   bool Statement_Switch::v_isEffect() const
   {
      return cond->isEffect() || body->isEffect();
   }

   //
   // Statement_Switch::v_isLabel
   //
   bool Statement_Switch::v_isLabel() const
   {
      return body->isLabel();
   }

   //
   // Statement_Switch::v_isNoAuto
   //
   bool Statement_Switch::v_isNoAuto() const
   {
      return cond->isNoAuto() && body->isNoAuto();
   }

   //
   // Statement_Switch::v_isNoReturn
   //
   bool Statement_Switch::v_isNoReturn() const
   {
      return body->isNoReturn();
   }

   //
   // Statement_Switch::v_isReturn
   //
   bool Statement_Switch::v_isReturn() const
   {
      // TODO: Somehow determine if all cases necessarily return.
      return false;
   }

   //
   // StatementCreate_Switch
   //
   SR::Statement::CRef StatementCreate_Switch(
      SR::Statement::Labels const &labels, Core::Origin pos,
      Scope_Case &scope, SR::Exp const *cond_, SR::Statement const *body)
   {
      auto cond = ExpPromo_Int(ExpPromo_LValue(cond_), pos);
      auto type = cond->getType();

      if(!type->isCTypeInteg())
         Core::Error(pos, "expected integer type");

      return Statement_Switch::Create(labels, pos, scope, cond, body);
   }

   //
   // StatementCreate_Switch
   //
   SR::Statement::CRef StatementCreate_Switch(
      SR::Statement::Labels &&labels, Core::Origin pos,
      Scope_Case &scope, SR::Exp const *cond_, SR::Statement const *body)
   {
      auto cond = ExpPromo_Int(ExpPromo_LValue(cond_), pos);
      auto type = cond->getType();

      if(!type->isCTypeInteg())
         Core::Error(pos, "expected integer type");

      return Statement_Switch::Create(std::move(labels), pos, scope, cond, body);
   }
}

// EOF

