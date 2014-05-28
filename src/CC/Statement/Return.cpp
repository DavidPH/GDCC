//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C return statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Return.hpp"

#include "CC/Exp.hpp"

#include "AST/Arg.hpp"
#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// CheckConstraint
//
static GDCC::AST::Exp::CPtr CheckConstraint(GDCC::Core::Origin pos,
   GDCC::AST::Function const *fn, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   bool isVoid = fn->retrn->isTypeVoid();

   // Sanity check. This should have been caught by now.
   if(!isVoid && !fn->retrn->isTypeComplete())
      throw Core::ExceptStr(pos, "complete type required for return");

   if(exp)
   {
      if(isVoid)
         throw Core::ExceptStr(pos, "return expression forbidden");

      return CC::ExpPromo_Assign(fn->retrn, exp, pos);
   }
   else
   {
      if(!isVoid)
         throw Core::ExceptStr(pos, "return expression required");

      return nullptr;
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
      // Statement_ReturnExp constructor
      //
      Statement_ReturnExp::Statement_ReturnExp(Labels const &labels_,
         Core::Origin pos_, AST::Exp const *exp_) :
         Super{labels_, pos_}, exp{exp_}
      {
      }

      //
      // Statement_ReturnExp constructor
      //
      Statement_ReturnExp::Statement_ReturnExp(Labels &&labels_,
         Core::Origin pos_, AST::Exp const *exp_) :
         Super{std::move(labels_), pos_}, exp{exp_}
      {
      }

      //
      // Statement_ReturnExp constructor
      //
      Statement_ReturnExp::Statement_ReturnExp(Core::Origin pos_,
         AST::Exp const *exp_) :
         Super{pos_}, exp{exp_}
      {
      }

      //
      // Statement_ReturnExp destructor
      //
      Statement_ReturnExp::~Statement_ReturnExp()
      {
      }

      //
      // Statement_ReturnExp::v_genStmnt
      //
      void Statement_ReturnExp::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         exp->genStmntStk(ctx);

         ctx.block.setArgs({exp->getType()->getSizeWords(), IR::Arg_Stk()});
         ctx.block.addStatement(IR::Code::Retn);
      }

      //
      // Statement_ReturnExp::v_isEffect
      //
      bool Statement_ReturnExp::v_isEffect() const
      {
         return exp->isEffect();
      }

      //
      // Statement_ReturnNul::v_genStmnt
      //
      void Statement_ReturnNul::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         ctx.block.addStatementArgs(IR::Code::Retn);
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(
         AST::Statement::Labels const &labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, fn, e))
            return Statement_ReturnExp::Create(labels, pos, exp);
         else
            return Statement_ReturnNul::Create(labels, pos);
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(
         AST::Statement::Labels &&labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, fn, e))
            return Statement_ReturnExp::Create(std::move(labels), pos, exp);
         else
            return Statement_ReturnNul::Create(std::move(labels), pos);
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(Core::Origin pos,
         AST::Function const *fn, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, fn, e))
            return Statement_ReturnExp::Create(pos, exp);
         else
            return Statement_ReturnNul::Create(pos);
      }
   }
}

// EOF

