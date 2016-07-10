//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C generic-selection expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/GenSel.hpp"

#include "Core/Exception.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // CheckConstraint
      //
      static void CheckConstraint(Core::Array<GenAssoc> const &sel, Core::Origin pos)
      {
         for(auto const &assoc : sel)
            if(!assoc.type->isCTypeObject() || !assoc.type->isTypeComplete())
               throw Core::ExceptStr(pos, "expected complete object type");
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
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

         throw Core::ExceptStr(pos, "no matching generic-selection");
      }

      //
      // ExpCreate_GenSel
      //
      SR::Exp::CRef ExpCreate_GenSel(SR::Exp const *exp, SR::Exp const *def,
         Core::Array<GenAssoc> const &sel, Core::Origin pos)
      {
         CheckConstraint(sel, pos);

         return Exp_GenSel::Create(exp, def, sel, pos);
      }

      //
      // ExpCreate_GenSel
      //
      SR::Exp::CRef ExpCreate_GenSel(SR::Exp const *exp, SR::Exp const *def,
         Core::Array<GenAssoc> &&sel, Core::Origin pos)
      {
         CheckConstraint(sel, pos);

         return Exp_GenSel::Create(exp, def, std::move(sel), pos);
      }
   }
}

// EOF

