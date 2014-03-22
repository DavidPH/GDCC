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

   if(exp)
      return CC::ExpPromo_Assign(fn->retrn, exp, pos);
   else
      return nullptr;
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
      Statement_ReturnExp::Statement_ReturnExp(
         Core::Array<Core::String> const &labels_, Core::Origin pos_,
         AST::Exp const *exp_) :
         Super{labels_, pos_}, exp{exp_}
      {
      }

      //
      // Statement_ReturnExp constructor
      //
      Statement_ReturnExp::Statement_ReturnExp(
         Core::Array<Core::String> &&labels_, Core::Origin pos_,
         AST::Exp const *exp_) :
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
         exp->genStmnt(ctx, AST::Arg(exp->getType(), IR::AddrBase::Stk));

         throw Core::ExceptStr(pos, "return exp stub");
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
         Core::Array<Core::String> const &labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, fn, e))
            return static_cast<AST::Statement::CRef>(
               new Statement_ReturnExp(labels, pos, exp));
         else
            return static_cast<AST::Statement::CRef>(
               new Statement_ReturnNul(labels, pos));
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, fn, e))
            return static_cast<AST::Statement::CRef>(
               new Statement_ReturnExp(std::move(labels), pos, exp));
         else
            return static_cast<AST::Statement::CRef>(
               new Statement_ReturnNul(std::move(labels), pos));
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(Core::Origin pos,
         AST::Function const *fn, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, fn, e))
            return static_cast<AST::Statement::CRef>(
               new Statement_ReturnExp(pos, exp));
         else
            return static_cast<AST::Statement::CRef>(
               new Statement_ReturnNul(pos));
      }
   }
}

// EOF

