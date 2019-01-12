//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation compound statement.
//
//-----------------------------------------------------------------------------

#include "SR/Statement/Multi.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Statement_Multi::v_genLabel
   //
   void Statement_Multi::v_genLabel(IR::Block &block) const
   {
      for(auto const &st : sts)
         st->genLabel(block);
   }

   //
   // Statement_Multi::v_genStmnt
   //
   void Statement_Multi::v_genStmnt(GenStmntCtx const &ctx) const
   {
      for(auto const &st : sts)
         st->genStmnt(ctx);
   }

   //
   // Statement_Multi::v_isBranch
   //
   bool Statement_Multi::v_isBranch() const
   {
      for(auto const &st : sts)
         if(st->isBranch()) return true;

      return false;
   }

   //
   // Statement_Multi::v_isEffect
   //
   bool Statement_Multi::v_isEffect() const
   {
      for(auto const &st : sts)
         if(st->isEffect()) return true;

      return false;
   }

   //
   // Statement_Multi::v_isLabel
   //
   bool Statement_Multi::v_isLabel() const
   {
      for(auto const &st : sts)
         if(st->isLabel()) return true;

      return false;
   }

   //
   // Statement_Multi::v_isNoAuto
   //
   bool Statement_Multi::v_isNoAuto() const
   {
      for(auto const &st : sts)
         if(!st->isNoAuto()) return false;

      return true;
   }

   //
   // Statement_Multi::v_isNoReturn
   //
   bool Statement_Multi::v_isNoReturn() const
   {
      for(auto const &st : sts)
         if(!st->isNoReturn()) return false;

      return true;
   }

   //
   // Statement_Multi::v_isReturn
   //
   bool Statement_Multi::v_isReturn() const
   {
      for(auto const &st : sts)
         if(st->isReturn()) return true;

      return false;
   }
}

// EOF

