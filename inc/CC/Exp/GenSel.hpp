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

#ifndef GDCC__CC__Exp__GenSel_H__
#define GDCC__CC__Exp__GenSel_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp/Defer.hpp"

#include "../../Core/Array.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_GenSel
      //
      class Exp_GenSel : public AST::Exp_Defer
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_GenSel, GDCC::AST::Exp_Defer);

      public:
         Core::Array<GenAssoc> const sel;
         AST::Exp::CPtr        const def;
         AST::Exp::CRef        const exp;


         // Create
         static CRef Create(AST::Exp const *exp, AST::Exp const *def,
            Core::Array<GenAssoc> const &sel, Core::Origin pos)
            {return CRef(new This(exp, def, sel, pos));}

         // Create
         static CRef Create(AST::Exp const *exp, AST::Exp const *def,
            Core::Array<GenAssoc> &&sel, Core::Origin pos)
            {return CRef(new This(exp, def, std::move(sel), pos));}

      protected:
         Exp_GenSel(AST::Exp const *e, AST::Exp const *d,
            Core::Array<GenAssoc> const &s, Core::Origin pos_) :
            Super{pos_}, sel{s}, def{d}, exp{e} {}

         Exp_GenSel(AST::Exp const *e, AST::Exp const *d,
            Core::Array<GenAssoc> &&s, Core::Origin pos_) :
            Super{pos_}, sel{std::move(s)}, def{d}, exp{e} {}

         virtual AST::Exp::CRef v_getDefer() const;
      };
   }
}

#endif//GDCC__CC__Exp__GenSel_H__

