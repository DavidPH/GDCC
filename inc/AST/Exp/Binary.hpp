//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree binary expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Binary_H__
#define GDCC__AST__Exp__Binary_H__

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class Code;
   }

   namespace AST
   {
      //
      // Exp_Binary
      //
      class Exp_Binary : public Exp
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Exp_Binary, GDCC::AST::Exp);

      public:
         Exp::CRef const expL, expR;
         TypeCRef  const type;

      protected:
         Exp_Binary(Type const *t, Exp const *l, Exp const *r,
            Core::Origin pos);
         virtual ~Exp_Binary();

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };

      //
      // Exp_Pair
      //
      class Exp_Pair : public Exp_Binary
      {
         GDCC_Core_CounterPreamble(GDCC::AST::Exp_Pair, GDCC::AST::Exp_Binary);

      public:
         static Exp::CRef Create(Exp const *l, Exp const *r, Core::Origin pos)
            {return static_cast<Exp::CRef>(new Exp_Pair(l, r, pos));}

      protected:
         Exp_Pair(Exp const *l, Exp const *r, Core::Origin pos);

         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      // Returns true if only evaluating for side effects.
      bool GenStmntNul(Exp_Binary const *exp, GenStmntCtx const &ctx,
         Arg const &dst);
   }
}

#endif//GDCC__AST__Exp__Binary_H__

