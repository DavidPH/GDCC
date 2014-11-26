//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
         for(Core::FastU i = 0; i != objWords; ++i)
         {
            IR::Arg_Lit dstIdx{AST::ExpCreate_Size(objValue + i)->getIRExp()};
            IR::Arg_Lit srcIdx{AST::ExpCreate_Size(paramIdx + i)->getIRExp()};

            ctx.block.addStatementArgs(IR::Code::Move_W,
               ArgT(dstIdx, 0), IR::Arg_LocReg(srcIdx, 0));
         }
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
            ctx.block.addStatementArgs(IR::Code::Plsa, scope.fn->allocLoc);

         // Move parameter data to actual storage location.
         Core::FastU paramIdx = 0;

         if(ctype == IR::CallType::StdCall &&
            Platform::TargetCur == Platform::Target::ZDoom)
            ++paramIdx;

         for(auto const &obj : scope.params)
         {
            if(!obj->value) continue;

            Core::FastU objValue = obj->value->getValue().getFastU();
            Core::FastU objWords = obj->type->getSizeWords();

            switch(obj->type->getQualAddr().base)
            {
            case IR::AddrBase::Loc:
               MoveParam<IR::Arg_Loc>(ctx, paramIdx, objValue, objWords);
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
            ctx.block.addStatementArgs(IR::Code::Xcod_SID);
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
            ctx.block.addStatementArgs(IR::Code::Plsf);

         // Perform return.
         if(scope.fn->retrn->isTypeVoid())
         {
            ctx.block.addStatementArgs(IR::Code::Retn);
         }
         else if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
         {
            ctx.block.setArgs({scope.fn->retrn->getSizeWords(), IR::Arg_Stk()});
            ctx.block.addStatement(IR::Code::Retn);
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

