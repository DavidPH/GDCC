//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C goto, break, and continue statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Goto_H__
#define GDCC__CC__Statement__Goto_H__

#include "../../CC/Statement.hpp"

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_Goto
      //
      class Statement_Goto : public SR::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_Goto, GDCC::SR::Statement);

      public:
         Core::String const label;


         static CRef Create(Labels const &labels, Core::Origin pos, Core::String label)
            {return CRef(new This(labels, pos, label));}
         static CRef Create(Labels &&labels, Core::Origin pos, Core::String label)
            {return CRef(new This(std::move(labels), pos, label));}

      protected:
         Statement_Goto(Labels const &labels, Core::Origin pos, Core::String label);
         Statement_Goto(Labels &&labels, Core::Origin pos, Core::String label);

         virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const {return true;}
         virtual bool v_isEffect() const {return false;}
         virtual bool v_isNoAuto() const {return true;}
      };

      //
      // Statement_GotoDyn
      //
      class Statement_GotoDyn : public SR::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_GotoDyn, GDCC::SR::Statement);

      public:
         Core::CounterRef<SR::Exp const> exp;


         static CRef Create(Labels &&labels, Core::Origin pos, SR::Exp const *exp)
            {return CRef(new This(std::move(labels), pos, exp));}

      protected:
         Statement_GotoDyn(Labels &&labels, Core::Origin pos, SR::Exp const *exp);
         virtual ~Statement_GotoDyn();

         virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const {return true;}
         virtual bool v_isEffect() const {return false;}
         virtual bool v_isNoAuto() const;
      };
   }
}

#endif//GDCC__CC__Statement__Goto_H__

