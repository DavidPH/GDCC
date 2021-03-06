//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C generic-selection expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/GenSel.hpp"

#include "CC/Factory.hpp"

#include "Core/Exception.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // CheckConstraint
   //
   static void CheckConstraint(Core::Array<GenAssoc> const &sel, Core::Origin pos)
   {
      for(auto const &assoc : sel)
         if(!assoc.type->isCTypeObject() || !assoc.type->isTypeComplete())
            Core::Error(pos, "expected complete object type");
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_GenSel::v_getDefer
   //
   SR::Exp::CRef Exp_GenSel::v_getDefer() const
   {
      auto type = exp->getType()->getTypeQual();

      for(auto const &assoc : sel)
         if(type == assoc.type) return assoc.exp;

      if(def) return static_cast<SR::Exp::CRef>(def);

      Core::Error(pos, "no matching generic-selection");
   }

   //
   // Factory::expCreate_GenSel
   //
   SR::Exp::CRef Factory::expCreate_GenSel(SR::Exp const *exp, SR::Exp const *def,
      Core::Array<GenAssoc> &&sel, Core::Origin pos)
   {
      CheckConstraint(sel, pos);

      return Exp_GenSel::Create(exp, def, std::move(sel), pos);
   }
}

// EOF

