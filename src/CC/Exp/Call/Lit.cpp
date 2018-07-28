//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ()" literal-call expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Call.hpp"

#include "Core/Exception.hpp"

#include "IR/Arg.hpp"
#include "IR/Block.hpp"
#include "IR/CallType.hpp"

#include "Platform/Platform.hpp"

#include "SR/Function.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_CallLit::v_genStmnt
   //
   void Exp_CallLit::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      IR::CallType callType = IR::GetCallTypeIR(func->getCallType());

      // Evaluate expression and arguments for side effects.
      exp->genStmnt(ctx);
      for(auto const &arg : args)
         arg->genStmnt(ctx);

      // Prepare IR args.
      Core::Array<IR::Arg> irArgs{args.size() + 2, IR::Arg()};

      irArgs[0] = IR::Arg_Stk(func->getBaseType()->isTypeVoid()
         ? 0 : func->getBaseType()->getSizeBytes());

      if(callType == IR::CallType::AsmFunc)
         irArgs[1] = IR::Arg_Lit(Platform::GetWordBytes(), exp->getFunction()->valueLit);
      else
         irArgs[1] = IR::Arg_Lit(Platform::GetWordBytes(), exp->getIRExp());

      for(std::size_t i = 0, e = args.size(); i != e; ++i)
         irArgs[i + 2] = IR::Arg_Lit(args[i]->getType()->getSizeBytes(), args[i]->getIRExp());

      // Generate call instruction.
      IR::Code code;
      switch(callType)
      {
      case IR::CallType::AsmFunc: code = IR::Code::Casm; break;
      case IR::CallType::Special: code = IR::Code::Cspe; break;
      default:
         Core::Error(pos, "unsupported call type");
      }
      ctx.block.addStmntArgs(code, std::move(irArgs));

      // Move to destination.
      GenStmnt_MovePart(this, ctx, dst, false, true);
   }
}

// EOF

