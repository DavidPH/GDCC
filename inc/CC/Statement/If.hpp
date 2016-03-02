//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C if statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC___Statement__If_H__
#define GDCC__CC___Statement__If_H__

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
      // Statement_If
      //
      class Statement_If : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_If, GDCC::AST::Statement);

      public:
         Core::CounterRef<AST::Exp const> const cond;
         AST::Statement::CRef             const bodyT;
         AST::Statement::CPtr             const bodyF;


         //
         // Create
         //
         static CRef Create(Labels const &labels, Core::Origin pos,
            AST::Exp const *cond, AST::Statement const *bodyT,
            AST::Statement const *bodyF = nullptr)
         {
            return CRef(new This(labels, pos, cond, bodyT, bodyF));
         }

         //
         // Create
         //
         static CRef Create(Labels &&labels, Core::Origin pos,
            AST::Exp const *cond, AST::Statement const *bodyT,
            AST::Statement const *bodyF = nullptr)
         {
            return CRef(new This(std::move(labels), pos, cond, bodyT, bodyF));
         }

      protected:
         Statement_If(Labels const &labels, Core::Origin pos,
            AST::Exp const *cond, AST::Statement const *bodyT,
            AST::Statement const *bodyF);
         Statement_If(Labels &&labels, Core::Origin pos, AST::Exp const *cond,
            AST::Statement const *bodyT, AST::Statement const *bodyF);

         virtual void v_genLabel(IR::Block &block) const;
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const;
         virtual bool v_isEffect() const;
         virtual bool v_isLabel() const;
         virtual bool v_isNoAuto() const;
         virtual bool v_isNoReturn() const;
         virtual bool v_isReturn() const;
      };
   }
}

#endif//GDCC__CC___Statement__If_H__

