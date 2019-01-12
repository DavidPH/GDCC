//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C do, for, and while statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Loop_H__
#define GDCC__CC__Statement__Loop_H__

#include "../../CC/Types.hpp"

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Statement_Loop
   //
   class Statement_Loop : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_Loop, GDCC::SR::Statement);

   public:
      Scope_Local                          &scope;
      SR::Statement::CPtr             const init;
      Core::CounterRef<SR::Exp const> const cond;
      SR::Statement::CPtr             const iter;
      SR::Statement::CRef             const body;
      bool                            const post : 1;


      //
      // Create
      //
      static CRef Create(Labels &&labels, Core::Origin pos,
         Scope_Local &ctx, SR::Statement const *init, SR::Exp const *cond,
         SR::Statement const *iter, SR::Statement const *body, bool post)
      {
         return CRef(new This(std::move(labels), pos, ctx,
            init, cond, iter, body, post));
      }

   protected:
      Statement_Loop(Labels &&labels, Core::Origin pos,
         Scope_Local &ctx, SR::Statement const *init, SR::Exp const *cond,
         SR::Statement const *iter, SR::Statement const *body, bool post);

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

#endif//GDCC__CC__Statement__Loop_H__

