//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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

#include "../../CC/Types.hpp"

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Statement_Switch
   //
   class Statement_Switch : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_Switch, GDCC::SR::Statement);

   public:
      Scope_Case                           &scope;
      Core::CounterRef<SR::Exp const> const cond;
      SR::Statement::CRef             const body;


      // Create
      static CRef Create(Labels &&labels, Core::Origin pos,
         Scope_Case &scope, SR::Exp const *cond, SR::Statement const *body)
         {return CRef(new This(std::move(labels), pos, scope, cond, body));}

   protected:
      Statement_Switch(Labels &&labels, Core::Origin pos, Scope_Case &scope,
         SR::Exp const *cond, SR::Statement const *body);

      virtual void v_genLabel(IR::Block &block) const;
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

      virtual bool v_isBranch() const;
      virtual bool v_isEffect() const;
      virtual bool v_isLabel() const;
      virtual bool v_isNoAuto() const;
      virtual bool v_isNoReturn() const;
      virtual bool v_isReturn() const;
   };
}


#endif//GDCC__CC__Statement__Switch_H__

