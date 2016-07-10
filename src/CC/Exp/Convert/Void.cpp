//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C conversion to void expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Convert/Void.hpp"

#include "Core/Exception.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_ConvertVoid::v_genStmnt
      //
      void Exp_ConvertVoid::v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst) const
      {
         if(dst.type->getQualAddr().base != IR::AddrBase::Nul)
            throw Core::ExceptStr(pos, "evaluating void to non-Nul");

         exp->genStmnt(ctx, dst);
      }

      //
      // ExpConvert_Void
      //
      SR::Exp::CRef ExpConvert_Void(SR::Type const *t, SR::Exp const *e,
         Core::Origin pos)
      {
         return Exp_ConvertVoid::Create(t, e, pos);
      }
   }
}

// EOF

