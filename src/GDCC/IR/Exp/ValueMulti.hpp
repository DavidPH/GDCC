//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: multiple expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Exp__ValueMulti_H__
#define GDCC__IR__Exp__ValueMulti_H__

#include "../Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_ValueMulti
      //
      class Exp_ValueMulti final : public Exp
      {
         GDCC_CounterPreamble(GDCC::IR::Exp_ValueMulti, GDCC::IR::Exp);

      public:
         virtual String getName() const {return STR_ValueMulti;}

         GDCC::Array<Exp::CRef> const expv;


         friend Exp::CRef ExpCreate_ValueMulti(Exp::CRef const *expv,
            std::size_t expc, Origin pos);

         friend Exp::CRef ExpGetIR_ValueMulti(IArchive &in);

      protected:
         Exp_ValueMulti(Exp_ValueMulti const &) = default;
         Exp_ValueMulti(Exp::CRef const *expv_, std::size_t expc, Origin pos_) :
            Super{pos_}, expv{expv_, expv_ + expc} {}
         explicit Exp_ValueMulti(IArchive &in);

         virtual bool v_canGetValue() const;

         virtual Type v_getType() const;

         virtual Value v_getValue() const;

         virtual OArchive &v_putIR(OArchive &out) const;
      };
   }
}

#endif//GDCC__IR__Exp__ValueMulti_H__

