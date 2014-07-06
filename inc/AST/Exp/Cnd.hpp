//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Conditional expression.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Cnd_H__
#define GDCC__AST__Exp__Cnd_H__

#include "../../AST/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Cnd
      //
      class Exp_Cnd : public Exp_Binary
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_Cnd, GDCC::AST::Exp_Binary);

      public:
         Exp::CRef const expC;


         // Create
         static CRef Create(Type const *t, Exp const *c, Exp const *l,
            Exp const *r, Core::Origin pos)
            {return CRef(new This(t, c, l, r, pos));}

      protected:
         Exp_Cnd(Type const *t, Exp const *c, Exp const *l,
            Exp const *r, Core::Origin pos_) :
            Super{t, l, r, pos_}, expC{c} {}

         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__AST__Exp__Cnd_H__

