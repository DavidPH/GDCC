//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C switch statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Switch_H__
#define GDCC__CC__Statement__Switch_H__

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
      // Statement_Switch
      //
      class Statement_Switch : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_Switch, GDCC::AST::Statement);

      public:
         Scope_Case                            &scope;
         Core::CounterRef<AST::Exp const> const cond;
         AST::Statement::CRef             const body;


         // Create
         static CRef Create(Labels const &labels, Core::Origin pos,
            Scope_Case &scope, AST::Exp const *cond, AST::Statement const *body)
            {return CRef(new This(labels, pos, scope, cond, body));}

         // Create
         static CRef Create(Labels &&labels, Core::Origin pos,
            Scope_Case &scope, AST::Exp const *cond, AST::Statement const *body)
            {return CRef(new This(std::move(labels), pos, scope, cond, body));}

      protected:
         Statement_Switch(Labels const &labels, Core::Origin pos,
            Scope_Case &scope, AST::Exp const *cond, AST::Statement const *body);
         Statement_Switch(Labels &&labels, Core::Origin pos, Scope_Case &scope,
            AST::Exp const *cond, AST::Statement const *body);

         virtual void v_genLabel(IR::Block &block) const;
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const;
         virtual bool v_isEffect() const;
         virtual bool v_isLabel()  const;
      };
   }
}


#endif//GDCC__CC__Statement__Switch_H__

