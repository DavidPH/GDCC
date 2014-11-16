//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C conversion to void expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Convert/Void.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_ConvertVoid::v_genStmnt
      //
      void Exp_ConvertVoid::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(dst.type->getQualAddr().base != IR::AddrBase::Nul)
            throw Core::ExceptStr(pos, "evaluating void to non-Nul");

         exp->genStmnt(ctx, dst);
      }

      //
      // ExpConvert_Void
      //
      AST::Exp::CRef ExpConvert_Void(AST::Type const *t, AST::Exp const *e,
         Core::Origin pos)
      {
         return Exp_ConvertVoid::Create(t, e, pos);
      }
   }
}

// EOF

