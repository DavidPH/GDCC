//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: multiple expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__Multi_H__
#define GDCC__IR__Exp__Multi_H__

#include "../../IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_Multi
      //
      class Exp_Multi final : public Exp
      {
         GDCC_Core_CounterPreamble(GDCC::IR::Exp_Multi, GDCC::IR::Exp);

      public:
         virtual Core::String getName() const {return Core::STR_Multi;}

         Core::Array<Exp::CRef> const expv;


         friend Exp::CRef ExpCreate_Multi(Exp::CRef const *expv,
            std::size_t expc, Core::Origin pos);

         friend Exp::CRef ExpGetIR_Multi(IArchive &in);

      protected:
         Exp_Multi(Exp_Multi const &) = default;
         Exp_Multi(Exp::CRef const *expv_, std::size_t expc, Core::Origin pos_) :
            Super{pos_}, expv{expv_, expv_ + expc} {}
         explicit Exp_Multi(IArchive &in);

         virtual bool v_canGetValue() const;

         virtual Type v_getType() const;

         virtual Value v_getValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__ValueMulti_H__

