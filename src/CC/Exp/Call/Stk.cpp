//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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

#include "AST/Arg.hpp"
#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/CallType.hpp"
#include "IR/Glyph.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
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
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
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
         Core::FastU  retWords;
         Core::FastU  stkWords  = 0;
         Core::FastU  vaWords   = 0;

         bool addrPre = IsAddrPre(callType);

         // Determine return word count.
         retWords = func->getBaseType()->isTypeVoid()
            ? 0 : func->getBaseType()->getSizeWords();

         // Evaluate address before arguments?
         if(addrPre)
            exp->genStmntStk(ctx);

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
            Core::FastU autWords = ctx.fn->localAut + vaWords;

            // Ensure auto pointer is unique for longjmp checks.
            // TODO: Only ensure this for functions which use setjmp.
            if(!autWords)
               autWords = 1;

            ctx.block.addStatementArgs({IR::Code::Pltn, 0}, IR::Arg_Stk(), autWords);

            ++stkWords;
         }
         else if(callType == IR::CallType::SScriptI ||
            callType == IR::CallType::SScriptS)
         {
            ctx.block.addStatementArgs({IR::Code::Pltn, 0},
               IR::Arg_Stk(), ctx.fn->localAut + vaWords + 1);

            ++stkWords;
            ++irWords;
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
            Core::FastU autItr = ctx.fn->localAut;
            for(; argItr-- != argEndStk;)
            {
               auto w = (*argItr)->getType()->getSizeWords();

               auto autType = (*argItr)->getType()
                  ->getTypeQual({{IR::AddrBase::Aut}})
                  ->getTypePointer()->getIRType().tPoint;

               IR::Value_Point autVal =
                  {autItr, IR::ArgBase::Aut, Core::STR_, std::move(autType)};

               auto autExp = IR::ExpCreate_Value(std::move(autVal), pos);

               ctx.block.addStatementArgs({IR::Code::Move_W, w},
                  IR::Arg_Aut(IR::Arg_Lit(autExp)), IR::Arg_Stk());

               autItr += w;
            }
         }
         else for(auto const &arg : args)
            arg->genStmntStk(ctx);

         if(callType != IR::CallType::AsmFunc)
            irWords += stkWords + 1;
         else
            irWords += 1;

         // Prepare IR args, preloaded with Stk for the call args.
         Core::Array<IR::Arg> irArgs{irWords, IR::Arg_Stk()};

         // For synchronous script calls, second IR arg is return flag.
         if(callType == IR::CallType::SScriptI ||
            callType == IR::CallType::SScriptS)
         {
            irArgs[1] = IR::Arg_Aut(IR::Arg_Lit(
               AST::ExpCreate_Size(ctx.fn->localAut + vaWords)->getIRExp()));
         }

         // Prepare function's address.
         if(addrPre)
            irArgs[0] = IR::Arg_Stk();
         else if(exp->isIRExp())
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
         case IR::CallType::AsmFunc:  code = IR::Code::Casm;    break;
         case IR::CallType::Native:   code = IR::Code::Cnat;    break;
         case IR::CallType::SScriptI: code = IR::Code::Cscr_IS; break;
         case IR::CallType::SScriptS: code = IR::Code::Cscr_SS; break;
         case IR::CallType::ScriptI:  code = IR::Code::Cscr_IA; break;
         case IR::CallType::ScriptS:  code = IR::Code::Cscr_SA; break;
         case IR::CallType::Special:  code = IR::Code::Cspe;    break;
         case IR::CallType::StdCall:  code = IR::Code::Call;    break;
         case IR::CallType::StkCall:  code = IR::Code::Call;    break;
         default:
            throw Core::ExceptStr(pos, "unsupported call type");
         }
         ctx.block.addStatementArgs({code, retWords}, std::move(irArgs));

         // Long jump propagation.
         if(IsLongJump(callType))
            ctx.block.addStatementArgs({IR::Code::Jfar, retWords},
               IR::Glyph(ctx.prog, scope.fn.getLabelLJR()));

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }
   }
}

// EOF

