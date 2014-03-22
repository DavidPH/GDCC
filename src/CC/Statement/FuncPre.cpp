//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function preamble statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/FuncPre.hpp"


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
      // StatementCreate_FuncPre
      //
      AST::Statement::CRef StatementCreate_FuncPre(Core::Origin pos,
         FunctionScope const *ctx)
      {
         return static_cast<AST::Statement::CRef>(
            new Statement_FuncPre(pos, ctx));
      }
   }
}

// EOF

