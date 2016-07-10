//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
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
      class Token;
   }

   namespace SR
   {
      class Exp;
      class Function;
      class Statement;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      Core::CounterRef<SR::Statement const> StatementCreate_Asm(
         Core::Array<Core::String> &&labels, Core::Origin pos,
         Core::Array<Core::Token> &&tokens);

      Core::CounterRef<SR::Statement const> StatementCreate_Break(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx);
      Core::CounterRef<SR::Statement const> StatementCreate_Break(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx);

      Core::CounterRef<SR::Statement const> StatementCreate_Continue(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx);
      Core::CounterRef<SR::Statement const> StatementCreate_Continue(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx);

      Core::CounterRef<SR::Statement const> StatementCreate_Decl(
         Core::Origin pos, Scope &ctx);

      Core::CounterRef<SR::Statement const> StatementCreate_Do(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, SR::Statement const *body, SR::Exp const *cond);
      Core::CounterRef<SR::Statement const> StatementCreate_Do(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, SR::Statement const *body, SR::Exp const *cond);

      Core::CounterRef<SR::Statement const> StatementCreate_For(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, SR::Statement const *init, SR::Exp const *cond,
         SR::Statement const *iter, SR::Statement const *body);
      Core::CounterRef<SR::Statement const> StatementCreate_For(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, SR::Statement const *init, SR::Exp const *cond,
         SR::Statement const *iter, SR::Statement const *body);

      Core::CounterRef<SR::Statement const> StatementCreate_FuncPre(
         Core::Origin pos, Scope_Function &scope);

      Core::CounterRef<SR::Statement const> StatementCreate_FuncPro(
         Core::Origin pos, Scope_Function &scope);

      Core::CounterRef<SR::Statement const> StatementCreate_Goto(
         Core::Array<Core::String> &&labels, Core::Origin pos,
         SR::Exp const *exp);

      Core::CounterRef<SR::Statement const> StatementCreate_Goto(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, Core::String name);
      Core::CounterRef<SR::Statement const> StatementCreate_Goto(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, Core::String name);

      Core::CounterRef<SR::Statement const> StatementCreate_If(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         SR::Exp const *cond, SR::Statement const *body);
      Core::CounterRef<SR::Statement const> StatementCreate_If(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         SR::Exp const *cond, SR::Statement const *body);

      Core::CounterRef<SR::Statement const> StatementCreate_If(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         SR::Exp const *cond, SR::Statement const *bodyT,
         SR::Statement const *bodyF);
      Core::CounterRef<SR::Statement const> StatementCreate_If(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         SR::Exp const *cond, SR::Statement const *bodyT,
         SR::Statement const *bodyF);

      Core::CounterRef<SR::Statement const> StatementCreate_Return(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Function &scope, SR::Exp const *exp = nullptr);
      Core::CounterRef<SR::Statement const> StatementCreate_Return(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Function &scope, SR::Exp const *exp = nullptr);
      Core::CounterRef<SR::Statement const> StatementCreate_Return(
         Core::Origin pos, Scope_Function &scope,
         SR::Exp const *exp = nullptr);

      Core::CounterRef<SR::Statement const> StatementCreate_Switch(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Case &ctx, SR::Exp const *cond, SR::Statement const *body);
      Core::CounterRef<SR::Statement const> StatementCreate_Switch(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Case &ctx, SR::Exp const *cond, SR::Statement const *body);

      Core::CounterRef<SR::Statement const> StatementCreate_While(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Scope_Local &ctx, SR::Exp const *cond, SR::Statement const *body);
      Core::CounterRef<SR::Statement const> StatementCreate_While(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         Scope_Local &ctx, SR::Exp const *cond, SR::Statement const *body);
   }
}

#endif//GDCC__CC__Statement_H__

