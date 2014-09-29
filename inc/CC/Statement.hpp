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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Exp;
      class Function;
      class Statement;
   }

   namespace CC
   {
      class Scope;
      class Scope_Case;
      class Scope_Function;
      class Scope_Local;
   }

   namespace Core
   {
      template<typename T> class Array;
      template<typename T> class CounterRef;
      class Origin;
      class String;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      Core::CounterRef<AST::Statement const> StatementCreate_Break(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx);
      Core::CounterRef<AST::Statement const> StatementCreate_Break(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx);

      Core::CounterRef<AST::Statement const> StatementCreate_Continue(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx);
      Core::CounterRef<AST::Statement const> StatementCreate_Continue(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx);

      Core::CounterRef<AST::Statement const> StatementCreate_Decl(
         Core::Origin pos, Scope &ctx);

      Core::CounterRef<AST::Statement const> StatementCreate_Do(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *body, AST::Exp const *cond);
      Core::CounterRef<AST::Statement const> StatementCreate_Do(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *body, AST::Exp const *cond);

      Core::CounterRef<AST::Statement const> StatementCreate_For(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
         AST::Statement const *iter, AST::Statement const *body);
      Core::CounterRef<AST::Statement const> StatementCreate_For(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Statement const *init, AST::Exp const *cond,
         AST::Statement const *iter, AST::Statement const *body);

      Core::CounterRef<AST::Statement const> StatementCreate_FuncPre(
         Core::Origin pos, Scope_Function &scope);

      Core::CounterRef<AST::Statement const> StatementCreate_FuncPro(
         Core::Origin pos, Scope_Function &scope);

      Core::CounterRef<AST::Statement const> StatementCreate_Goto(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, Core::String name);
      Core::CounterRef<AST::Statement const> StatementCreate_Goto(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, Core::String name);

      Core::CounterRef<AST::Statement const> StatementCreate_If(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *body);
      Core::CounterRef<AST::Statement const> StatementCreate_If(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *body);

      Core::CounterRef<AST::Statement const> StatementCreate_If(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *bodyT,
         AST::Statement const *bodyF);
      Core::CounterRef<AST::Statement const> StatementCreate_If(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         AST::Exp const *cond, AST::Statement const *bodyT,
         AST::Statement const *bodyF);

      Core::CounterRef<AST::Statement const> StatementCreate_Return(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Function &scope, AST::Exp const *exp = nullptr);
      Core::CounterRef<AST::Statement const> StatementCreate_Return(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Function &scope, AST::Exp const *exp = nullptr);
      Core::CounterRef<AST::Statement const> StatementCreate_Return(
         Core::Origin pos, Scope_Function &scope,
         AST::Exp const *exp = nullptr);

      Core::CounterRef<AST::Statement const> StatementCreate_Switch(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Case &ctx, AST::Exp const *cond, AST::Statement const *body);
      Core::CounterRef<AST::Statement const> StatementCreate_Switch(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Case &ctx, AST::Exp const *cond, AST::Statement const *body);

      Core::CounterRef<AST::Statement const> StatementCreate_While(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body);
      Core::CounterRef<AST::Statement const> StatementCreate_While(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, AST::Exp const *cond, AST::Statement const *body);
   }
}

#endif//GDCC__CC__Statement_H__

