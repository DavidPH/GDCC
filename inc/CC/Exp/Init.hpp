//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C initializer expression.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Init_H__
#define GDCC__CC__Exp__Init_H__

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      class Init;

      //
      // Exp_Init
      //
      class Exp_Init : public AST::Exp
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Init, GDCC::AST::Exp);

      public:
         std::shared_ptr<Init> const init;
         bool                  const skipZero;


         static CRef Create(std::unique_ptr<Init> &&init, bool skipZero);

         static CRef Create(std::unique_ptr<Init> &&init, bool skipZero,
            Core::Origin pos);

      protected:
         Exp_Init(std::unique_ptr<Init> &&init, bool skipZero, Core::Origin pos);
         virtual ~Exp_Init();

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isGenStmnt() const {return true;}

         virtual bool v_isIRExp() const;

         virtual bool v_isNoAuto() const;
      };
   }
}

#endif//GDCC__CC__Exp__Init_H__

