//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation statement base.
//
//-----------------------------------------------------------------------------

#include "SR/Statement.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Statement::genLabel
   //
   void Statement::genLabel(IR::Block &block) const
   {
      for(auto const &s : labels) block.addLabel(s);
      v_genLabel(block);
   }

   //
   // Statement::genStmnt
   //
   void Statement::genStmnt(GenStmntCtx const &ctx) const
   {
      ctx.block.setOrigin(pos);

      for(auto const &s : labels) ctx.block.addLabel(s);

      if(isTrivial())
         v_genLabel(ctx.block);
      else
         v_genStmnt(ctx);
   }

   //
   // Statement::v_genLabel
   //
   void Statement::v_genLabel(IR::Block &) const
   {
   }
}

// EOF

