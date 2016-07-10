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

#ifndef GDCC__CC__Exp__GenSel_H__
#define GDCC__CC__Exp__GenSel_H__

#include "../../CC/Exp.hpp"

#include "../../Core/Array.hpp"

#include "../../SR/Exp/Defer.hpp"


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
      class Exp_GenSel : public SR::Exp_Defer
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_GenSel, GDCC::SR::Exp_Defer);

      public:
         Core::Array<GenAssoc> const sel;
         SR::Exp::CPtr         const def;
         SR::Exp::CRef         const exp;


         // Create
         static CRef Create(SR::Exp const *exp, SR::Exp const *def,
            Core::Array<GenAssoc> const &sel, Core::Origin pos)
            {return CRef(new This(exp, def, sel, pos));}

         // Create
         static CRef Create(SR::Exp const *exp, SR::Exp const *def,
            Core::Array<GenAssoc> &&sel, Core::Origin pos)
            {return CRef(new This(exp, def, std::move(sel), pos));}

      protected:
         Exp_GenSel(SR::Exp const *e, SR::Exp const *d,
            Core::Array<GenAssoc> const &s, Core::Origin pos_) :
            Super{pos_}, sel{s}, def{d}, exp{e} {}

         Exp_GenSel(SR::Exp const *e, SR::Exp const *d,
            Core::Array<GenAssoc> &&s, Core::Origin pos_) :
            Super{pos_}, sel{std::move(s)}, def{d}, exp{e} {}

         virtual SR::Exp::CRef v_getDefer() const;
      };
   }
}

#endif//GDCC__CC__Exp__GenSel_H__

