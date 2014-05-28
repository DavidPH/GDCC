//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C return statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__Return_H__
#define GDCC__CC__Statement__Return_H__

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
      // Statement_ReturnExp
      //
      class Statement_ReturnExp : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_ReturnExp, GDCC::AST::Statement);

      public:
         Core::CounterRef<AST::Exp const> const exp;


         // Create
         static CRef Create(Labels const &labels, Core::Origin pos, AST::Exp const *exp)
            {return CRef(new This(labels, pos, exp));}

         // Create
         static CRef Create(Labels &&labels, Core::Origin pos, AST::Exp const *exp)
            {return CRef(new This(std::move(labels), pos, exp));}

         // Create
         static CRef Create(Core::Origin pos, AST::Exp const *exp)
            {return CRef(new This(pos, exp));}

      protected:
         Statement_ReturnExp(Labels const &labels, Core::Origin pos, AST::Exp const *exp);
         Statement_ReturnExp(Labels &&labels, Core::Origin pos, AST::Exp const *exp);
         Statement_ReturnExp(Core::Origin pos, AST::Exp const *exp);
         virtual ~Statement_ReturnExp();

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const {return true;}
         virtual bool v_isEffect() const;
      };

      //
      // Statement_ReturnNul
      //
      class Statement_ReturnNul : public AST::Statement
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Statement_ReturnNul, GDCC::AST::Statement);

      public:
         // Create
         static CRef Create(Labels const &labels, Core::Origin pos)
            {return CRef(new This(labels, pos));}

         // Create
         static CRef Create(Labels &&labels, Core::Origin pos)
            {return CRef(new This(std::move(labels), pos));}

         // Create
         static CRef Create(Core::Origin pos)
            {return CRef(new This(pos));}

      protected:
         Statement_ReturnNul(Labels const &labels_, Core::Origin pos_) :
            Super{labels_, pos_} {}
         Statement_ReturnNul(Labels &&labels_, Core::Origin pos_) :
            Super{std::move(labels_), pos_} {}
         explicit Statement_ReturnNul(Core::Origin pos_) : Super{pos_} {}

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx) const;

         virtual bool v_isBranch() const {return true;}
         virtual bool v_isEffect() const {return false;}
      };
   }
}

#endif//GDCC__CC__Statement__Return_H__

