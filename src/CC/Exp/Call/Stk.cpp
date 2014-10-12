//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ()" stack-call expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Call.hpp"

#include "AST/Arg.hpp"
#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "IR/Block.hpp"
#include "IR/CallType.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_CallStk::v_genStmnt
      //
      void Exp_CallStk::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         IR::CallType callType  = IR::GetCallTypeIR(func->getCallType());
         Core::FastU  callWords = func->getCallWords();
         std::size_t  irWords   = 0;
         Core::FastU  stkWords  = 0;
         Core::FastU  vaWords   = 0;

         // Count arguments.
         for(auto const &arg : args)
            stkWords += arg->getType()->getSizeWords();

         // Is this a ZDACS variadic call?
         if(stkWords > func->getCallWords() &&
            callType == IR::CallType::StdCall &&
            Platform::TargetCur == Platform::Target::ZDoom)
         {
            vaWords  = stkWords - callWords;
            stkWords = callWords;
         }

         // Propagate stack pointer.
         if(callType == IR::CallType::StdCall &&
            Platform::TargetCur == Platform::Target::ZDoom)
         {
            ctx.block.addStatementArgs(IR::Code::Pltn,
               IR::Arg_Stk(), ctx.fn->localArs + vaWords);

            ++stkWords;
         }

         // Evaluate arguments.
         if(vaWords)
         {
            auto argItr    = args.begin();
            auto argEnd    = args.end();
            auto argEndStk = argItr + func->getParameters()->size();

            // Pass-by-Stk arguments.
            for(; argItr != argEndStk; ++argItr)
               (*argItr)->genStmntStk(ctx);

            // Pass-by-Loc arguments.
            Core::FastU locItr = ctx.fn->localArs + vaWords;
            for(; argItr != argEnd; ++argItr)
            {
               locItr -= (*argItr)->getType()->getSizeWords();

               auto locType = (*argItr)->getType()
                  ->getTypeQual({{IR::AddrBase::Loc, Core::STR_}})
                  ->getTypePointer();

               auto locExp = AST::ExpCreate_IRExp(IR::ExpCreate_Value(
                  IR::Value_Point(locItr, IR::ArgBase::Loc, Core::STR_,
                     locType->getIRType().tPoint), pos), locType, pos);

               (*argItr)->genStmnt(ctx, {locType->getBaseType(), locExp});
            }
         }
         else for(auto const &arg : args)
            arg->genStmntStk(ctx);

         if(callType != IR::CallType::AsmFunc)
            irWords += stkWords + 2;
         else
         {
            // TODO: const params
            irWords += 1;
         }

         // Prepare IR args, preloaded with Stk for the call args.
         Core::Array<IR::Arg> irArgs{irWords, IR::Arg_Stk()};

         // Second IR arg is return words, except for Casm.
         if(callType != IR::CallType::AsmFunc)
         {
            irArgs[1] = IR::Arg_Lit(func->getBaseType()->isTypeVoid()
               ? AST::ExpCreate_Size(0)->getIRExp()
               : func->getBaseType()->getSizeWordsVM()->getIRExp());
         }

         // Prepare function's address.
         if(exp->isIRExp())
         {
            exp->genStmnt(ctx);
            irArgs[0] = IR::Arg_Lit(exp->getIRExp());
         }
         else
         {
            exp->genStmntStk(ctx);
            irArgs[0] = IR::Arg_Stk();
         }

         // Generate call instruction.
         IR::Code code;
         switch(callType)
         {
         case IR::CallType::AsmFunc: code = IR::Code::Casm; break;
         case IR::CallType::Native:  code = IR::Code::Cnat; break;
         case IR::CallType::Special: code = IR::Code::Cspe; break;
         default:                    code = IR::Code::Call; break;
         }
         ctx.block.addStatementArgs(code, std::move(irArgs));

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }
   }
}

// EOF

