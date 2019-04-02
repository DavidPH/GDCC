//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Deferred expression base.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Defer.hpp"

#include "SR/Arg.hpp"
#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Type.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Exp_Defer::getDefer
   //
   Exp::CRef Exp_Defer::getDefer() const
   {
      if(!cacheDefer)
         cacheDefer = v_getDefer();

      return static_cast<Exp::CRef>(cacheDefer);
   }

   // Exp_Defer's deferring virtuals:

   void Exp_Defer::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
      {return getDefer()->genStmnt(ctx, dst);}

   Arg Exp_Defer::v_getArg() const
      {return getDefer()->getArg();}

   Function::Ref Exp_Defer::v_getFunction() const
      {return getDefer()->getFunction();}

   IR::Exp::CRef Exp_Defer::v_getIRExp() const
      {return getDefer()->getIRExp();}

   Object::Ref Exp_Defer::v_getObject() const
      {return getDefer()->getObject();}

   Type::CRef Exp_Defer::v_getType() const
      {return getDefer()->getType();}

   bool Exp_Defer::v_isEffect() const
      {return getDefer()->isEffect();}

   bool Exp_Defer::v_isFunction() const
      {return getDefer()->isFunction();}

   bool Exp_Defer::v_isIRExp() const
      {return getDefer()->isIRExp();}

   bool Exp_Defer::v_isNoAuto() const
      {return getDefer()->isNoAuto();}

   bool Exp_Defer::v_isObject() const
      {return getDefer()->isObject();}
}

// EOF

