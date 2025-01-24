//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ()" stack-call expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Call.hpp"

#include "CC/Scope/Function.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"

#include "SR/Arg.hpp"
#include "SR/Function.hpp"
#include "SR/Type.hpp"

#include "Target/CallType.hpp"
#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // IsAddrPre
   //
   static bool IsAddrPre(IR::CallType callType)
   {
      switch(callType)
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
         return true;

      default:
         return false;
      }
   }

   //
   // IsLongJump
   //
   static bool IsLongJump(IR::CallType callType)
   {
      switch(callType)
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
      case IR::CallType::StdCall:
         return true;

      default:
         return false;
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_CallStk::v_genStmnt
   //
   void Exp_CallStk::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      IR::CallType callType  = Target::GetCallTypeIR(func->getCallType());
      Core::FastU  callWords = func->getCallWords();
      Core::FastU  retBytes;
      Core::FastU  stkWords  = 0;
      Core::FastU  vaWords   = 0;
      Core::FastU  wordBytes = Target::GetWordBytes();

      bool addrPre = IsAddrPre(callType);

      // Determine return word count.
      retBytes = func->getBaseType()->isTypeVoid()
         ? 0 : func->getBaseType()->getSizeBytes();

      GenStmnt_MoveDstPre(this, ctx, dst);

      // Evaluate address before arguments?
      if(addrPre)
         exp->genStmntStk(ctx);

      // Count arguments.
      for(auto const &arg : args)
         stkWords += arg->getType()->getSizeWords();

      // Is this a variadic call?
      if(stkWords > func->getCallWords() && Target::IsCallVaria(callType))
      {
         vaWords  = stkWords - callWords;
         stkWords = callWords;
      }

      // Propagate stack pointer.
      if(Target::IsCallAutoProp(callType))
      {
         Core::FastU autWords = ctx.fn->localAut + vaWords
            + Target::GetCallAutoAdd(callType);

         // Ensure auto pointer is unique for longjmp checks.
         // TODO: Only ensure this for functions which use setjmp.
         if(!autWords)
            autWords = 1;

         ctx.block.setArgSize().addStmnt(IR::CodeBase::Pltn,
            IR::Block::Stk(), autWords * wordBytes);

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

         // Pass-by-Aut arguments.
         for(; argItr != argEnd; ++argItr)
            (*argItr)->genStmntStk(ctx);
         Core::FastU autItr =
            (ctx.fn->localAut + Target::GetCallAutoAdd(callType)) * wordBytes;
         for(; argItr-- != argEndStk;)
         {
            auto autBytes = (*argItr)->getType()->getSizeBytes();

            auto autType = (*argItr)->getType()
               ->getTypeQual({{IR::AddrBase::Aut, Core::STR_}})
               ->getTypePointer()->getIRType().tPoint;

            IR::Value_Point autVal =
               {autItr, IR::ArgBase::Aut, Core::STR_, std::move(autType)};

            auto autExp = IR::ExpCreate_Value(std::move(autVal), pos);

            ctx.block.addStmnt(IR::CodeBase::Move,
               IR::Arg_Aut(autBytes, IR::Arg_Lit(wordBytes, autExp)),
               IR::Arg_Stk(autBytes));

            autItr += autBytes;
         }
      }
      else for(auto const &arg : args)
         arg->genStmntStk(ctx);

      // Calculate IR arg count.
      std::size_t irWords = stkWords + 2;

      if(callType == IR::CallType::SScriptI ||
         callType == IR::CallType::SScriptS)
      {
         ++irWords;
      }

      // Prepare IR args, preloaded with Stk for the call args.
      Core::Array<IR::Arg> irArgs{irWords, IR::Arg_Stk(wordBytes)};

      irArgs[0] = IR::Arg_Stk(retBytes);

      // Prepare function's address.
      if(addrPre)
      {
         irArgs[1] = IR::Arg_Stk(wordBytes);
      }
      else if(exp->isIRExp())
      {
         exp->genStmnt(ctx);
         irArgs[1] = IR::Arg_Lit(wordBytes, exp->getIRExp());
      }
      else
      {
         exp->genStmntStk(ctx);
         irArgs[1] = IR::Arg_Stk(wordBytes);
      }

      // For synchronous script calls, third IR arg is return flag.
      if(callType == IR::CallType::SScriptI ||
         callType == IR::CallType::SScriptS)
      {
         irArgs[2] = IR::Arg_Aut(wordBytes, IR::Arg_Lit(wordBytes,
            ctx.block.getExp(ctx.fn->localAut + vaWords)));
      }

      // Generate call instruction.
      IR::Code code;
      switch(callType)
      {
      case IR::CallType::AsmFunc:  code = IR::CodeBase::Casm;    break;
      case IR::CallType::Native:   code = IR::CodeBase::Cnat;    break;
      case IR::CallType::SScriptI: code = IR::CodeBase::Cscr_IS; break;
      case IR::CallType::SScriptS: code = IR::CodeBase::Cscr_SS; break;
      case IR::CallType::ScriptI:  code = IR::CodeBase::Cscr_IA; break;
      case IR::CallType::ScriptS:  code = IR::CodeBase::Cscr_SA; break;
      case IR::CallType::Special:  code = IR::CodeBase::Cspe;    break;
      case IR::CallType::StdCall:  code = IR::CodeBase::Call;    break;
      case IR::CallType::StkCall:  code = IR::CodeBase::Call;    break;
      default:
         Core::Error(pos, "unsupported call type");
      }
      ctx.block.addStmntArgs(code, std::move(irArgs));

      // Long jump propagation.
      if(IsLongJump(callType))
         ctx.block.setArgSize().addStmnt(IR::CodeBase::Jfar_Pro,
            IR::Glyph(ctx.prog, scope.fn.getLabelLJR()), IR::Arg_Stk(retBytes));

      // Move to destination.
      GenStmnt_MoveDstSuf(this, ctx, dst);
   }
}

// EOF

