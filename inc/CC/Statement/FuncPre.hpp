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

#ifndef GDCC__CC__Statement__FuncPre_H__
#define GDCC__CC__Statement__FuncPre_H__

#include "../../CC/Statement.hpp"

#include "../../AST/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_FuncPre
      //
      class Statement_FuncPre : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_FuncPre, GDCC::AST::Statement);

      public:
         Scope_Function &ctx;


         friend AST::Statement::CRef StatementCreate_FuncPre(Core::Origin pos,
            Scope_Function &ctx);

      protected:
         Statement_FuncPre(Core::Origin pos_, Scope_Function &ctx_) :
            Super{pos_}, ctx(ctx_) {}

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx) const;

         virtual bool v_isEffect() const {return true;}
      };
   }
}

#endif//GDCC__CC__Statement__FuncPre_H__

