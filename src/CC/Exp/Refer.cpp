//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator &" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Refer.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Refer constructor
      //
      Exp_Refer::Exp_Refer(AST::Exp const *e, Core::Origin pos_) :
         Super{e->getType()->getTypePointer(), e, pos_}
      {
      }

      //
      // Exp_Refer::v_genStmnt
      //
      void Exp_Refer::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         auto arg = exp->getArg();
         if(!arg.data) throw Core::ExceptStr(pos, "expected arg data");
         arg.data->genStmnt(ctx, dst);
      }

      //
      // Exp_Refer::v_getIRExp
      //
      IR::Exp::CRef Exp_Refer::v_getIRExp() const
      {
         auto arg = exp->getArg();
         if(!arg.data) throw Core::ExceptStr(pos, "expected arg data");
         return arg.data->getIRExp();
      }

      //
      // Exp_Refer::v_isIRExp
      //
      bool Exp_Refer::v_isIRExp() const
      {
         auto arg = exp->getArg();
         if(!arg.data) return false;
         return arg.data->isIRExp();
      }

      //
      // ExpCreate_Refer
      //
      AST::Exp::CRef ExpCreate_Refer(AST::Exp const *e, Core::Origin pos)
      {
         auto type = e->getType();

         if(type->isCTypeFunction())
            return static_cast<AST::Exp::CRef>(new Exp_Refer(e, pos));

         if(!IsLValue(e))
            throw Core::ExceptStr(pos, "expected function or lvalue");

         if(type->isTypeBitfield())
            throw Core::ExceptStr(pos, "cannot get address of bitfield");

         // TODO: Check if e is an object declared as register/noPtr.

         e->setRefer();

         return static_cast<AST::Exp::CRef>(new Exp_Refer(e, pos));
      }
   }
}

// EOF

