//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression statement.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Statement__Exp_H__
#define GDCC__SR__Statement__Exp_H__

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Statement_Exp
   //
   class Statement_Exp final : public Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Statement_Exp, GDCC::SR::Statement);

   public:
      Core::CounterRef<Exp const> const exp;


      static CRef Create(Labels &&labels, Core::Origin pos, Exp const *exp)
         {return CRef(new This(std::move(labels), pos, exp));}

   protected:
      Statement_Exp(Labels &&labels_, Core::Origin pos_, Exp const *exp_) :
         Super{std::move(labels_), pos_}, exp{exp_} {}
      virtual ~Statement_Exp();

      virtual void v_genStmnt(GenStmntCtx const &ctx) const;

      virtual bool v_isEffect() const;
      virtual bool v_isNoAuto() const;
   };
}

#endif//GDCC__SR__Statement__Exp_H__

