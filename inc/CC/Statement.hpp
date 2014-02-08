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
      class FunctionScope;
      class Scope;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      AST::Statement::CRef StatementCreate_Decl(Core::Origin pos, Scope *ctx);

      AST::Statement::CRef StatementCreate_FuncPre(Core::Origin pos,
         FunctionScope const *fn);

      AST::Statement::CRef StatementCreate_Return(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *exp = nullptr);
      AST::Statement::CRef StatementCreate_Return(
         Core::Array<Core::String>      &&labels, Core::Origin pos,
         AST::Function const *fn, AST::Exp const *exp = nullptr);
      AST::Statement::CRef StatementCreate_Return(Core::Origin pos,
         AST::Function const *fn, AST::Exp const *exp = nullptr);
   }
}

#endif//GDCC__CC__Statement_H__

