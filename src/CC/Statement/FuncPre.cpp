//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function preamble and prologue statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/FuncPre.hpp"

#include "CC/Scope/Function.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"

#include "SR/Exp.hpp"
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
      IR::Arg_Lit dstIdx{Target::GetWordBytes(), SR::ExpCreate_Size(objValue)->getIRExp()};
      IR::Arg_Lit srcIdx{Target::GetWordBytes(), SR::ExpCreate_Size(paramIdx)->getIRExp()};

      ctx.block.addStmnt(IR::Code::Move,
         ArgT(objBytes, dstIdx), IR::Arg_LocReg(objBytes, srcIdx));
   }

   //
   // NeedSID
   //
   static bool NeedSID(IR::CallType ctype, Core::Array<Core::String> const &stype)
   {
      if(ctype != IR::CallType::ScriptI && ctype != IR::CallType::ScriptS)
         return false;

      for(auto const &st : stype)
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

      if(NeedSID(ctype, scope.fn->stype))
         ctx.block.addStmnt(IR::Code::Xcod_SID);

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
         ctx.block.addStmnt(IR::Code::Retn);
      }
      else if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
      {
         ctx.block.addStmnt(IR::Code::Retn, IR::Arg_Stk(scope.fn->retrn->getSizeBytes()));
      }

      // Generate long jump return, if needed.
      if(scope.labelLJR)
      {
         ctx.block.addLabel(scope.labelLJR);

         if(scope.fn->retrn->isTypeVoid())
            ctx.block.addStmnt(IR::Code::Retn);
         else
            ctx.block.setArgSize(scope.fn->retrn->getSizeBytes()).addStmnt(IR::Code::Retn, 0);
      }
   }

   //
   // StatementCreate_FuncPre
   //
   SR::Statement::CRef StatementCreate_FuncPre(Core::Origin pos,
      Scope_Function &scope)
   {
      return Statement_FuncPre::Create(pos, scope);
   }

   //
   // StatementCreate_FuncPro
   //
   SR::Statement::CRef StatementCreate_FuncPro(Core::Origin pos,
      Scope_Function &scope)
   {
      return Statement_FuncPro::Create(pos, scope);
   }
}

// EOF

