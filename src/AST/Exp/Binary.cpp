//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree binary expression base.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Binary.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Binary constructor
      //
      Exp_Binary::Exp_Binary(Exp const *l, Exp const *r, Core::Origin pos_) :
         Super{pos_}, expL{l}, expR{r}
      {
      }

      //
      // Exp_Binary::tryGenStmntNul
      //
      bool Exp_Binary::tryGenStmntNul(IR::Block &block, Function *fn,
         Arg const &dst) const
      {
         // If only evaluating for side-effect, just evaluate sub-expressions.
         if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
         {
            expL->genStmnt(block, fn);
            expR->genStmnt(block, fn);
            return true;
         }

         return false;
      }

      //
      // Exp_Binary::v_isEffect
      //
      bool Exp_Binary::v_isEffect() const
      {
         return expL->isEffect() || expR->isEffect();
      }

      //
      // Exp_Binary::v_isIRExp
      //
      bool Exp_Binary::v_isIRExp() const
      {
         return expL->isIRExp() && expR->isIRExp();
      }
   }
}

// EOF

