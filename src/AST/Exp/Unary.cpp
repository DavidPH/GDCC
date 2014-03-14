//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree unary expression base.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Unary.hpp"

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
      // Exp_Unary constructor
      //
      Exp_Unary::Exp_Unary(Type const *t, Exp const *e, Core::Origin pos_) :
         Super{pos_}, exp{e}, type{t}
      {
      }

      //
      // Exp_Unary Exp_Unary
      //
      Exp_Unary::~Exp_Unary()
      {
      }

      //
      // Exp_Unary::v_getType
      //
      AST::Type::CRef Exp_Unary::v_getType() const
      {
         return type;
      }

      //
      // Exp_Unary::v_isEffect
      //
      bool Exp_Unary::v_isEffect() const
      {
         return exp->isEffect();
      }

      //
      // Exp_Unary::v_isIRExp
      //
      bool Exp_Unary::v_isIRExp() const
      {
         return exp->isIRExp();
      }

      //
      // GenStmntNul
      //
      bool GenStmntNul(Exp_Unary const *exp, IR::Block &block, Function *fn,
         Arg const &dst)
      {
         // If only evaluating for side-effect, just evaluate sub-expression.
         if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
         {
            exp->exp->genStmnt(block, fn);
            return true;
         }

         return false;
      }
   }
}

// EOF

