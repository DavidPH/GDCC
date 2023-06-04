//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function preamble and prologue statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/FuncPre.hpp"

#include "CC/Factory.hpp"
#include "CC/Scope/Function.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"

#include "SR/Exp/IRExp.hpp"
#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Type.hpp"

#include "Target/CallType.hpp"
#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // MoveParam
   //
   template<typename ArgT>
   static void MoveParam(SR::GenStmntCtx const &ctx, Core::FastU paramIdx,
      Core::FastU objValue, Core::FastU objBytes)
   {
      // TODO: Replace the Create_Size call with something more direct.
      IR::Arg_Lit dstIdx{Target::GetWordBytes(), SR::Exp_IRExp::Create_Size(objValue)->getIRExp()};
      IR::Arg_Lit srcIdx{Target::GetWordBytes(), SR::Exp_IRExp::Create_Size(paramIdx)->getIRExp()};

      ctx.block.addStmnt(IR::CodeBase::Move,
         ArgT(objBytes, dstIdx), IR::Arg_LocReg(objBytes, srcIdx));
   }

   //
   // NeedSID
   //
   static bool NeedSID(SR::Function *fn, IR::CallType ctype)
   {
      if(fn->noInitDelay)
         return false;

      if(ctype != IR::CallType::ScriptI && ctype != IR::CallType::ScriptS)
         return false;

      for(auto const &st : fn->stype)
      {
         if(st == Core::STR_enter || st == Core::STR_open)
            return true;
      }

      return false;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Statement_FuncPre::v_genStmnt
   //
   void Statement_FuncPre::v_genStmnt(SR::GenStmntCtx const &ctx) const
   {
      auto ctype = Target::GetCallTypeIR(scope.fn->ctype);

      // Move parameter data to actual storage location.
      Core::FastU paramIdx = 0;

      if(Target::IsCallAutoProp(ctype))
         paramIdx += Target::GetWordBytes();

      for(auto const &obj : scope.params)
      {
         if(!obj->value) continue;

         Core::FastU objValue = obj->value->getValue().getFastU();
         Core::FastU objBytes = obj->type->getSizeBytes();

         switch(obj->type->getQualAddr().base)
         {
         case IR::AddrBase::Aut:
            MoveParam<IR::Arg_Aut>(ctx, paramIdx, objValue, objBytes);
            break;

         case IR::AddrBase::LocReg:
            if(objValue != paramIdx)
               MoveParam<IR::Arg_LocReg>(ctx, paramIdx, objValue, objBytes);
            break;

         default:
            break;
         }

         paramIdx += objBytes;
      }

      if(NeedSID(scope.fn, ctype))
         ctx.block.addStmnt(IR::CodeBase::Xcod_SID);

      if(scope.fn->labelRes)
         ctx.block.addLabel(scope.fn->labelRes);
   }

   //
   // Statement_FuncPro::v_genStmnt
   //
   void Statement_FuncPro::v_genStmnt(SR::GenStmntCtx const &ctx) const
   {
      auto ctype = Target::GetCallTypeIR(scope.fn->ctype);

      // Add label for exit point. Unless it was never generated and is
      // therefore unused.
      if(scope.fn->labelEnd)
         ctx.block.addLabel(scope.fn->labelEnd);

      // Perform return.
      if(scope.fn->retrn->isTypeVoid())
      {
         ctx.block.addStmnt(IR::CodeBase::Retn);
      }
      else if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
      {
         ctx.block.addStmnt(IR::CodeBase::Retn, IR::Arg_Stk(scope.fn->retrn->getSizeBytes()));
      }

      // Generate long jump return, if needed.
      if(scope.labelLJR)
      {
         ctx.block.addLabel(scope.labelLJR);

         if(scope.fn->retrn->isTypeVoid())
            ctx.block.addStmnt(IR::CodeBase::Rjnk);
         else
            ctx.block.setArgSize(scope.fn->retrn->getSizeBytes())
               .addStmnt(IR::CodeBase::Rjnk, 0);
      }
   }

   //
   // Factory::stCreate_FuncPre
   //
   SR::Statement::CRef Factory::stCreate_FuncPre(Core::Origin pos,
      Scope_Function &scope)
   {
      return Statement_FuncPre::Create(pos, scope);
   }

   //
   // Factory::stCreate_FuncPro
   //
   SR::Statement::CRef Factory::stCreate_FuncPro(Core::Origin pos,
      Scope_Function &scope)
   {
      return Statement_FuncPro::Create(pos, scope);
   }
}

// EOF

