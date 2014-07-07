//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C generic-selection expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/GenSel.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// CheckConstraint
//
static void CheckConstraint(GDCC::Core::Array<GDCC::CC::GenAssoc> const &sel,
   GDCC::Core::Origin pos)
{
   for(auto const &assoc : sel)
      if(!assoc.type->isCTypeObject() || !assoc.type->isTypeComplete())
         throw GDCC::Core::ExceptStr(pos, "expected complete object type");
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_GenSel::v_getDefer
      //
      AST::Exp::CRef Exp_GenSel::v_getDefer() const
      {
         auto type = exp->getType()->getTypeQual();

         for(auto const &assoc : sel)
            if(type == assoc.type) return assoc.exp;

         if(def) return static_cast<AST::Exp::CRef>(def);

         throw Core::ExceptStr(pos, "no matching generic-selection");
      }

      //
      // ExpCreate_GenSel
      //
      AST::Exp::CRef ExpCreate_GenSel(AST::Exp const *exp, AST::Exp const *def,
         Core::Array<GenAssoc> const &sel, Core::Origin pos)
      {
         CheckConstraint(sel, pos);

         return Exp_GenSel::Create(exp, def, sel, pos);
      }

      //
      // ExpCreate_GenSel
      //
      AST::Exp::CRef ExpCreate_GenSel(AST::Exp const *exp, AST::Exp const *def,
         Core::Array<GenAssoc> &&sel, Core::Origin pos)
      {
         CheckConstraint(sel, pos);

         return Exp_GenSel::Create(exp, def, std::move(sel), pos);
      }
   }
}

// EOF

