//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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

#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"

#include "IR/Block.hpp"
#include "IR/CallType.hpp"
#include "IR/ScriptType.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // MoveParam
      //
      template<typename ArgT>
      static void MoveParam(AST::GenStmntCtx const &ctx, Core::FastU paramIdx,
         Core::FastU objValue, Core::FastU objWords)
      {
         IR::Arg_Lit dstIdx{AST::ExpCreate_Size(objValue)->getIRExp()};
         IR::Arg_Lit srcIdx{AST::ExpCreate_Size(paramIdx)->getIRExp()};

         ctx.block.addStatementArgs({IR::Code::Move_W, objWords},
            ArgT(dstIdx), IR::Arg_LocReg(srcIdx));
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_FuncPre::v_genStmnt
      //
      void Statement_FuncPre::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         auto ctype = IR::GetCallTypeIR(scope.fn->ctype);
         auto stype = scope.fn->stype;

         // If script, allocate automatic storage area.
         if(scope.fn->allocLoc)
            ctx.block.addStatementArgs({IR::Code::Plsa, 0}, scope.fn->allocLoc);

         // Move parameter data to actual storage location.
         Core::FastU paramIdx = 0;

         if((ctype == IR::CallType::SScriptI ||
             ctype == IR::CallType::SScriptS ||
             ctype == IR::CallType::StdCall) &&
            Platform::TargetCur == Platform::Target::ZDoom)
            ++paramIdx;

         for(auto const &obj : scope.params)
         {
            if(!obj->value) continue;

            Core::FastU objValue = obj->value->getValue().getFastU();
            Core::FastU objWords = obj->type->getSizeWords();

            switch(obj->type->getQualAddr().base)
            {
            case IR::AddrBase::Aut:
               MoveParam<IR::Arg_Aut>(ctx, paramIdx, objValue, objWords);
               break;

            case IR::AddrBase::LocReg:
               if(objValue != paramIdx)
                  MoveParam<IR::Arg_LocReg>(ctx, paramIdx, objValue, objWords);
               break;

            default:
               break;
            }

            paramIdx += objWords;
         }

         if((ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS) &&
            (stype == IR::ScriptType::Open || stype == IR::ScriptType::Enter))
            ctx.block.addStatementArgs({IR::Code::Xcod_SID, 0});

         if(scope.fn->labelRes)
            ctx.block.addLabel(scope.fn->labelRes);
      }

      //
      // Statement_FuncPro::v_genStmnt
      //
      void Statement_FuncPro::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         auto ctype = IR::GetCallTypeIR(scope.fn->ctype);

         // Add label for exit point. Unless it was never generated and is
         // therefore unused.
         if(scope.fn->labelEnd)
            ctx.block.addLabel(scope.fn->labelEnd);

         // If script, free automatic storage area.
         if(scope.fn->allocLoc)
            ctx.block.addStatementArgs({IR::Code::Plsf, 0});

         // Perform return.
         if(scope.fn->retrn->isTypeVoid())
         {
            ctx.block.addStatementArgs({IR::Code::Retn, 0});
         }
         else if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
         {
            ctx.block.addStatementArgs(
               {IR::Code::Retn, scope.fn->retrn->getSizeWords()}, IR::Arg_Stk());
         }
      }

      //
      // StatementCreate_FuncPre
      //
      AST::Statement::CRef StatementCreate_FuncPre(Core::Origin pos,
         Scope_Function &scope)
      {
         return Statement_FuncPre::Create(pos, scope);
      }

      //
      // StatementCreate_FuncPro
      //
      AST::Statement::CRef StatementCreate_FuncPro(Core::Origin pos,
         Scope_Function &scope)
      {
         return Statement_FuncPro::Create(pos, scope);
      }
   }
}

// EOF

