//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "../../AST/Statement.hpp"


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
      class Statement_Goto : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_Goto, GDCC::AST::Statement);

      public:
         Core::String const label;


         static CRef Create(Labels const &labels, Core::Origin pos, Core::String label)
            {return CRef(new This(labels, pos, label));}
         static CRef Create(Labels &&labels, Core::Origin pos, Core::String label)
            {return CRef(new This(std::move(labels), pos, label));}

      protected:
         Statement_Goto(Labels const &labels, Core::Origin pos, Core::String label);
         Statement_Goto(Labels &&labels, Core::Origin pos, Core::String label);

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const {return true;}
         virtual bool v_isEffect() const {return false;}
      };
   }
}

#endif//GDCC__CC__Statement__Goto_H__

