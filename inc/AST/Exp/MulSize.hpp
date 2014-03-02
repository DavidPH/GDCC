//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree Size-type multiply expression.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__MulSize_H__
#define GDCC__AST__Exp__MulSize_H__

#include "../../AST/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_MulSize
      //
      class Exp_MulSize final : public Exp_Binary
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Exp_MulSize, GDCC::AST::Exp_Binary);

      public:
         friend Exp::CRef ExpCreate_MulSize(Exp const *l, Exp const *r);

      protected:
         Exp_MulSize(Exp const *l, Exp const *r, Core::Origin pos);

         virtual void v_genStmnt(IR::Block &block, Function *fn, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;
      };
   }
}

#endif//GDCC__AST__Exp__MulSize_H__

