//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      using OpCodePair = std::pair<IR::OpCode, IR::OpCode>;
   }
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GenCond_Codes
      //
      static OpCodePair GenCond_Codes(SR::Statement const *, SR::Type const *t)
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
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, defLabel);
      }

      //
      // GenCond_SearchPart
      //
      static void GenCond_SearchPart(
         Statement_Switch        const *stmnt,
         SR::GenStmntCtx         const &ctx,
         OpCodePair              const &ops,
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
               ctx.block.addStatementArgs({IR::Code::Jump, 0}, caseLabel);

            // Otherwise, check against value.
            else
            {
               auto caseValue = GenCond_GenValue(stmnt, begin[0]);

               // Compare condition to case value for equality.
               ctx.block.addStatementArgs(ops.second,
                  IR::Arg_Stk(), cond.getArg(), caseValue);

               // If true, branch to case.
               ctx.block.addStatementArgs({IR::Code::Jcnd_Tru, 1},
                  IR::Arg_Stk(), caseLabel);

               // Otherwise, branch to default.
               GenCond_BranchDefault(stmnt, ctx);
            }

            return;
         }

         auto pivot = &begin[count / 2];

         IR::Glyph pivotLabel = {ctx.prog, ctx.fn->genLabel()};
         auto      pivotValue = GenCond_GenValue(stmnt, *pivot);

         // Compare condition against pivot.
         ctx.block.addStatementArgs(ops.first,
            IR::Arg_Stk(), cond.getArg(), IR::Arg_Lit(pivotValue));

         // Skip if false.
         ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1},
            IR::Arg_Stk(), pivotLabel);

         // Search left half.
         GenCond_SearchPart(stmnt, ctx, ops, cond, begin, pivot);

         // Search right half.
         ctx.block.addLabel(pivotLabel);
         GenCond_SearchPart(stmnt, ctx, ops, cond, pivot, end);
      }

      //
      // GenCond_Search
      //
      static void GenCond_Search(Statement_Switch const *stmnt,
         SR::GenStmntCtx const &ctx, OpCodePair const &ops)
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
         ctx.block.addStatementArgs({IR::Code::Move_W, tmp.size()},
            tmp.getArg(), IR::Arg_Stk());

         // Begin binary search.
         GenCond_SearchPart(stmnt, ctx, ops, tmp, cases.data() + 1,
            cases.data() + cases.size() - 1);
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
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
         auto cond = ExpPromo_Int(cond_, pos);
         auto type = cond->getType();

         if(!type->isCTypeInteg())
            throw Core::ExceptStr(pos, "expected integer type");

         return Statement_Switch::Create(labels, pos, scope, cond, body);
      }

      //
      // StatementCreate_Switch
      //
      SR::Statement::CRef StatementCreate_Switch(
         SR::Statement::Labels &&labels, Core::Origin pos,
         Scope_Case &scope, SR::Exp const *cond_, SR::Statement const *body)
      {
         auto cond = ExpPromo_Int(cond_, pos);
         auto type = cond->getType();

         if(!type->isCTypeInteg())
            throw Core::ExceptStr(pos, "expected integer type");

         return Statement_Switch::Create(std::move(labels), pos, scope, cond, body);
      }
   }
}

// EOF

