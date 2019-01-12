//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C return statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Return_H__
#define GDCC__CC__Statement__Return_H__

#include "../../CC/Types.hpp"

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Statement_ReturnExp
   //
   class Statement_ReturnExp : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_ReturnExp, GDCC::SR::Statement);

   public:
      Core::CounterRef<SR::Exp const> const exp;
      Scope_Function                        &scope;


      // Create
      static CRef Create(Labels &&labels, Core::Origin pos,
         SR::Exp const *exp, Scope_Function &scope)
         {return CRef(new This(std::move(labels), pos, exp, scope));}

   protected:
      Statement_ReturnExp(Labels &&labels, Core::Origin pos,
         SR::Exp const *exp, Scope_Function &scope);
      virtual ~Statement_ReturnExp();

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

      virtual bool v_isBranch() const {return true;}
      virtual bool v_isEffect() const;
      virtual bool v_isNoAuto() const;
      virtual bool v_isNoReturn() const {return false;}
      virtual bool v_isReturn() const {return true;}
   };

   //
   // Statement_ReturnNul
   //
   class Statement_ReturnNul : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_ReturnNul, GDCC::SR::Statement);

   public:
      Scope_Function &scope;


      // Create
      static CRef Create(Labels &&labels, Core::Origin pos, Scope_Function &scope)
         {return CRef(new This(std::move(labels), pos, scope));}

   protected:
      Statement_ReturnNul(Labels &&labels_, Core::Origin pos_, Scope_Function &scope_) :
         Super{std::move(labels_), pos_}, scope(scope_) {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

      virtual bool v_isBranch() const {return true;}
      virtual bool v_isEffect() const {return false;}
      virtual bool v_isNoAuto() const {return true;}
      virtual bool v_isNoReturn() const {return false;}
      virtual bool v_isReturn() const {return true;}
   };
}

#endif//GDCC__CC__Statement__Return_H__

