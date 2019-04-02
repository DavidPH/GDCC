//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation empty statement.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Statement__Empty_H__
#define GDCC__SR__Statement__Empty_H__

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Statement_Empty
   //
   class Statement_Empty final : public Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Statement_Empty, GDCC::SR::Statement);

   public:
      static CRef Create(Labels &&labels, Core::Origin pos)
         {return CRef(new This(std::move(labels), pos));}

   protected:
      Statement_Empty(Labels &&labels_, Core::Origin pos_) :
         Super{std::move(labels_), pos_} {}

      virtual void v_genStmnt(GenStmntCtx const &ctx) const;

      virtual bool v_isEffect() const {return false;}
      virtual bool v_isNoAuto() const {return true;}
   };
}

#endif//GDCC__SR__Statement__Empty_H__

