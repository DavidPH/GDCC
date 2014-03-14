//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "IR/Exp.hpp"


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
      Exp_Binary::Exp_Binary(Type const *t, Exp const *l, Exp const *r,
         Core::Origin pos_) :
         Super{pos_}, expL{l}, expR{r}, type{t}
      {
      }

      //
      // Exp_Binary destructor
      //
      Exp_Binary::~Exp_Binary()
      {
      }

      //
      // Exp_Binary::v_getType
      //
      AST::Type::CRef Exp_Binary::v_getType() const
      {
         return type;
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

      //
      // Exp_Pair constructor
      //
      Exp_Pair::Exp_Pair(Exp const *l, Exp const *r, Core::Origin pos_) :
         Super{r->getType(), l, r, pos_}
      {
      }

      //
      // Exp_Pair::v_genStmnt
      //
      void Exp_Pair::v_genStmnt(IR::Block &block, Function *fn,
         Arg const &dst) const
      {
         expL->genStmnt(block, fn);
         expR->genStmnt(block, fn, dst);
      }

      //
      // Exp_Pair::v_getIRExp
      //
      IR::Exp::CRef Exp_Pair::v_getIRExp() const
      {
         return expR->getIRExp();
      }

      //
      // GenStmntNul
      //
      bool GenStmntNul(Exp_Binary const *exp, IR::Block &block, Function *fn,
         Arg const &dst)
      {
         // If only evaluating for side-effect, just evaluate sub-expressions.
         if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
         {
            exp->expL->genStmnt(block, fn);
            exp->expR->genStmnt(block, fn);
            return true;
         }

         return false;
      }
   }
}

// EOF

