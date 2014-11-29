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

#include "CC/Exp/Init.hpp"

#include "CC/Init.hpp"

#include "AST/Type.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Init constructor
      //
      Exp_Init::Exp_Init(std::unique_ptr<Init> &&init_, bool skipZero_,
         Core::Origin pos_) :
         Super{pos_},

         init    {init_.release()},
         skipZero{skipZero_}
      {
      }

      //
      // Exp_Init destructor
      //
      Exp_Init::~Exp_Init()
      {
      }

      //
      // Exp_Init::v_genStmnt
      //
      void Exp_Init::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         init->genStmnt(ctx, dst, skipZero);
      }

      //
      // Exp_Init::v_getIRExp
      //
      IR::Exp::CRef Exp_Init::v_getIRExp() const
      {
         return init->getIRExp();
      }

      //
      // Exp_Init::v_getType
      //
      AST::Type::CRef Exp_Init::v_getType() const
      {
         return init->type;
      }

      //
      // Exp_Init::v_isEffect
      //
      bool Exp_Init::v_isEffect() const
      {
         return init->isEffect();
      }

      //
      // Exp_Init::v_isIRExp
      //
      bool Exp_Init::v_isIRExp() const
      {
         return init->isIRExp();
      }

      //
      // Exp_Init::v_isNoAuto
      //
      bool Exp_Init::v_isNoAuto() const
      {
         return init->isNoAuto();
      }

      //
      // Exp_Init::Create
      //
      Exp_Init::CRef Exp_Init::Create(Init::Ptr &&init, bool skipZero)
      {
         return CRef(new This(std::move(init), skipZero, init->pos));
      }

      //
      // Exp_Init::Create
      //
      Exp_Init::CRef Exp_Init::Create(Init::Ptr &&init, bool skipZero,
         Core::Origin pos)
      {
         return CRef(new This(std::move(init), skipZero, pos));
      }
   }
}

// EOF

