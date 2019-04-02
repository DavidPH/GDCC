//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation compound statement.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Statement__Multi_H__
#define GDCC__SR__Statement__Multi_H__

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Statement_Multi
   //
   class Statement_Multi final : public Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Statement_Multi, GDCC::SR::Statement);

   public:
      static CRef Create(Labels &&labels, Core::Origin pos,
         Core::Array<Statement::CRef> &&sts)
         {return CRef(new This(std::move(labels), pos, std::move(sts)));}

      Core::Array<Statement::CRef> const sts;

   protected:
      Statement_Multi(Labels &&lbl, Core::Origin pos_,
         Core::Array<Statement::CRef> &&sts_) :
         Super{std::move(lbl), pos_}, sts{std::move(sts_)} {}

      virtual void v_genLabel(IR::Block &block) const;
      virtual void v_genStmnt(GenStmntCtx const &ctx) const;

      virtual bool v_isBranch() const;
      virtual bool v_isEffect() const;
      virtual bool v_isLabel() const;
      virtual bool v_isNoAuto() const;
      virtual bool v_isNoReturn() const;
      virtual bool v_isReturn() const;
   };
}

#endif//GDCC__SR__Statement__Multi_H__

