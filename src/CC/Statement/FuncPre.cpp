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

#include "AST/Function.hpp"
#include "AST/Type.hpp"

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
      // Statement_FuncPre::v_genStmnt
      //
      void Statement_FuncPre::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         auto ctype = IR::GetCallTypeIR(scope.fn->ctype);

         // If script, allocate automatic storage area.
         if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
            ctx.block.addStatementArgs(IR::Code::Plsa, 8192);

         // Move parameter data to actual storage location.
         // TODO
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
         if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
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

