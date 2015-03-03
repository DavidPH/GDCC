//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ()" stack-call expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Call.hpp"

#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Arg.hpp"
#include "IR/Block.hpp"
#include "IR/CallType.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_CallLit::v_genStmnt
      //
      void Exp_CallLit::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         IR::CallType callType = IR::GetCallTypeIR(func->getCallType());

         // Determine return word count.
         Core::FastU retWords = func->getBaseType()->isTypeVoid()
            ? 0 : func->getBaseType()->getSizeWords();

         // Evaluate expression and arguments for side effects.
         exp->genStmnt(ctx);
         for(auto const &arg : args)
            arg->genStmnt(ctx);

         // Count arguments.
         Core::FastU stkWords = 0;
         for(auto const &arg : args)
            stkWords += arg->getType()->getSizeWords();

         // Prepare IR args, preloaded with Stk for the call args.
         Core::Array<IR::Arg> irArgs{stkWords + 1, IR::Arg_Stk()};
         if(callType == IR::CallType::AsmFunc)
            irArgs[0] = IR::Arg_Lit(exp->getFunction()->valueLit);
         else
            irArgs[0] = IR::Arg_Lit(exp->getIRExp());

         for(std::size_t i = 0, e = args.size(); i != e; ++i)
            irArgs[i + 1] = IR::Arg_Lit(args[i]->getIRExp());

         // Generate call instruction.
         IR::Code code;
         switch(callType)
         {
         case IR::CallType::AsmFunc: code = IR::Code::Casm; break;
         case IR::CallType::Special: code = IR::Code::Cspe; break;
         default:
            throw Core::ExceptStr(pos, "unsupported call type");
         }
         ctx.block.addStatementArgs({code, retWords}, std::move(irArgs));

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }
   }
}

// EOF

