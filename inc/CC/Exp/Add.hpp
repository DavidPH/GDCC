//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator +" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Add_H__
#define GDCC__CC__Exp__Add_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp/Arith.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_AddPoint
      //
      class Exp_AddPoint : public AST::Exp_Add
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_AddPoint, GDCC::AST::Exp_Add);

         GDCC_AST_Exp_BinaryCreator(Exp_AddPoint);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;
      };

      //
      // Exp_AddPointEq
      //
      class Exp_AddPointEq : public AST::Exp_Add
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_AddPointEq, GDCC::AST::Exp_Add);

      public:
         bool const post : 1;


         static CRef Create(bool post, AST::Type const *t,
            AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
            {return CRef(new This(post, t, l, r, pos));}

      protected:
         Exp_AddPointEq(bool post_, AST::Type const *t, AST::Exp const *l,
            AST::Exp const *r, Core::Origin pos_) :
            Super{t, l, r, pos_}, post{post_} {}

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual IR::Exp::CRef v_getIRExp() const;

         virtual bool v_isEffect() const {return true;}

         virtual bool v_isIRExp() const;
      };

      //
      // Exp_AddPtrRaw
      //
      class Exp_AddPtrRaw : public AST::Exp_Add
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_AddPtrRaw, GDCC::AST::Exp_Add);

         GDCC_AST_Exp_BinaryCreator(Exp_AddPtrRaw);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;
      };

      //
      // Exp_AddStrEntInt
      //
      class Exp_AddStrEntInt : public AST::Exp_Binary
      {
         GDCC_Core_CounterPreamble(
            GDCC::CC::Exp_AddStrEntInt, GDCC::AST::Exp_Binary);

         GDCC_AST_Exp_BinaryCreator(Exp_AddStrEntInt);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual bool v_isIRExp() const {return false;}
      };
   }
}

#endif//GDCC__CC__Exp__Add_H__

