//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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

#include "../../CC/Types.hpp"

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Statement_If
   //
   class Statement_If : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_If, GDCC::SR::Statement);

   public:
      Core::CounterRef<SR::Exp const> const cond;
      SR::Statement::CRef             const bodyT;
      SR::Statement::CPtr             const bodyF;


      //
      // Create
      //
      static CRef Create(Labels &&labels, Core::Origin pos,
         SR::Exp const *cond, SR::Statement const *bodyT,
         SR::Statement const *bodyF = nullptr)
      {
         return CRef(new This(std::move(labels), pos, cond, bodyT, bodyF));
      }

   protected:
      Statement_If(Labels &&labels, Core::Origin pos, SR::Exp const *cond,
         SR::Statement const *bodyT, SR::Statement const *bodyF);

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

#endif//GDCC__CC___Statement__If_H__

