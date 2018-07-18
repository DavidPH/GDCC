//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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
      friend Statement::CRef StatementCreate_Empty(
         Core::Array<Core::String> const &labels, Core::Origin pos);
      friend Statement::CRef StatementCreate_Empty(
         Core::Array<Core::String>      &&labels, Core::Origin pos);
      friend Statement::CRef StatementCreate_Empty(Core::Origin pos);

   protected:
      Statement_Empty(Core::Array<Core::String> const &labels_, Core::Origin pos_) :
         Super{labels_, pos_} {}
      Statement_Empty(Core::Array<Core::String> &&labels_, Core::Origin pos_) :
         Super{std::move(labels_), pos_} {}
      Statement_Empty(Core::Origin pos_) : Super{pos_} {}

      virtual void v_genStmnt(GenStmntCtx const &ctx) const;

      virtual bool v_isEffect() const {return false;}
      virtual bool v_isNoAuto() const {return true;}
   };
}

#endif//GDCC__SR__Statement__Empty_H__

