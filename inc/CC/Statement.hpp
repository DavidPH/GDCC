//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement_H__
#define GDCC__CC__Statement_H__

#include "../AST/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Exp;
      class Function;
   }

   namespace CC
   {
      class Scope;
      class Scope_Function;
      class Scope_Local;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      AST::Statement::CRef StatementCreate_Break(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx);
      AST::Statement::CRef StatementCreate_Break(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx);

      AST::Statement::CRef StatementCreate_Continue(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx);
      AST::Statement::CRef StatementCreate_Continue(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx);

      AST::Statement::CRef StatementCreate_Decl(Core::Origin pos, Scope &ctx);

      AST::Statement::CRef StatementCreate_Do(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *body, AST::Exp const *cond);
      AST::Statement::CRef StatementCreate_Do(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *body, AST::Exp const *cond);

      AST::Statement::CRef StatementCreate_For(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
         AST::Statement const *iter, AST::Statement const *body);
      AST::Statement::CRef StatementCreate_For(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
         AST::Statement const *iter, AST::Statement const *body);

      AST::Statement::CRef StatementCreate_FuncPre(Core::Origin pos,
         Scope_Function &fn);

      AST::Statement::CRef StatementCreate_Goto(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, Core::String name);
      AST::Statement::CRef StatementCreate_Goto(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, Core::String name);

      AST::Statement::CRef StatementCreate_If(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *body);
      AST::Statement::CRef StatementCreate_If(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *body);

      AST::Statement::CRef StatementCreate_If(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *bodyT,
         AST::Statement const *bodyF);
      AST::Statement::CRef StatementCreate_If(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *bodyT,
         AST::Statement const *bodyF);

      AST::Statement::CRef StatementCreate_Return(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *exp = nullptr);
      AST::Statement::CRef StatementCreate_Return(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *exp = nullptr);
      AST::Statement::CRef StatementCreate_Return(Core::Origin pos,
         AST::Function const *fn, AST::Exp const *exp = nullptr);

      AST::Statement::CRef StatementCreate_Switch(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body);
      AST::Statement::CRef StatementCreate_Switch(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body);

      AST::Statement::CRef StatementCreate_While(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body);
      AST::Statement::CRef StatementCreate_While(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body);
   }
}

#endif//GDCC__CC__Statement_H__

