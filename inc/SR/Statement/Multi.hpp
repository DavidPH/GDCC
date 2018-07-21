//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation compound statement.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Statement__Multi_H__
#define GDCC__SR__Statement__Multi_H__

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Statement_Multi
   //
   class Statement_Multi final : public Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Statement_Multi, GDCC::SR::Statement);

   public:
      static CRef Create(Labels const &labels, Core::Origin pos, Stmnts const &stmnts)
         {return CRef(new This(labels, pos, stmnts));}
      static CRef Create(Labels const &labels, Core::Origin pos, Stmnts &&stmnts)
         {return CRef(new This(labels, pos, std::move(stmnts)));}
      static CRef Create(Labels &&labels, Core::Origin pos, Stmnts const &stmnts)
         {return CRef(new This(std::move(labels), pos, stmnts));}
      static CRef Create(Labels &&labels, Core::Origin pos, Stmnts &&stmnts)
         {return CRef(new This(std::move(labels), pos, std::move(stmnts)));}
      static CRef Create(Core::Origin pos, Stmnts const &stmnts)
         {return CRef(new This(pos, stmnts));}
      static CRef Create(Core::Origin pos, Stmnts &&stmnts)
         {return CRef(new This(pos, std::move(stmnts)));}

      Stmnts const stmnts;

   protected:
      Statement_Multi(Labels const &lbl, Core::Origin pos_, Stmnts const &stmnts_) :
         Super{lbl, pos_}, stmnts{stmnts_} {}
      Statement_Multi(Labels const &lbl, Core::Origin pos_, Stmnts &&stmnts_) :
         Super{lbl, pos_}, stmnts{std::move(stmnts_)} {}
      Statement_Multi(Labels &&lbl, Core::Origin pos_, Stmnts const &stmnts_) :
         Super{std::move(lbl), pos_}, stmnts{stmnts_} {}
      Statement_Multi(Labels &&lbl, Core::Origin pos_, Stmnts &&stmnts_) :
         Super{std::move(lbl), pos_}, stmnts{std::move(stmnts_)} {}
      Statement_Multi(Core::Origin pos_, Stmnts const &stmnts_) :
         Super{pos_}, stmnts{stmnts_} {}
      Statement_Multi(Core::Origin pos_, Stmnts &&stmnts_) :
         Super{pos_}, stmnts{std::move(stmnts_)} {}

      virtual void v_genLabel(IR::Block &block) const;
      virtual void v_genStmnt(GenStmntCtx const &ctx) const;

      virtual bool v_isBranch() const;
      virtual bool v_isEffect() const;
      virtual bool v_isLabel() const;
      virtual bool v_isNoAuto() const;
      virtual bool v_isNoReturn() const;
      virtual bool v_isReturn() const;
   };
}

#endif//GDCC__SR__Statement__Multi_H__

