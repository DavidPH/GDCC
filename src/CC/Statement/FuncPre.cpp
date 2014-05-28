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
      void Statement_FuncPre::v_genStmnt(AST::GenStmntCtx const &) const
      {
         // If script, configure environment.
         // TODO

         // Move parameter data to actual storage location.
         // TODO
      }

      //
      // Statement_FuncPro::v_genStmnt
      //
      void Statement_FuncPro::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         if(scope.fn->retrn->isTypeVoid())
            ctx.block.addStatementArgs(IR::Code::Retn);
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

