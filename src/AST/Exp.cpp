//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression base.
//
//-----------------------------------------------------------------------------

#include "AST/Exp.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "IR/Exp.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp constructor
      //
      Exp::Exp(Core::Origin pos_) : pos{pos_}
      {
      }

      //
      // Exp destructor
      //
      Exp::~Exp()
      {
      }

      //
      // Exp::genStmnt
      //
      void Exp::genStmnt(IR::Block &block, Function *fn) const
      {
         genStmnt(block, fn, Arg(getType(), IR::AddrBase::Nul));
      }

      //
      // Exp::genStmnt
      //
      void Exp::genStmnt(IR::Block &block, Function *fn, Arg const &dst) const
      {
         if(isIRExp() && !isEffect())
         {
            genStmntMove(block, fn, dst, Arg(getType(), IR::AddrBase::Lit, this));
            return;
         }

         v_genStmnt(block, fn, dst);
      }

      //
      // Exp::getArg
      //
      Arg Exp::getArg() const
      {
         return v_getArg();
      }

      //
      // Exp::getIRExp
      //
      IR::Exp::CRef Exp::getIRExp() const
      {
         if(!cacheIRExp)
            cacheIRExp = v_getIRExp();

         return static_cast<IR::Exp::CRef>(cacheIRExp);
      }

      //
      // Exp::getType
      //
      Type::CRef Exp::getType() const
      {
         return v_getType();
      }

      //
      // Exp::isEffect
      //
      bool Exp::isEffect() const
      {
         return v_isEffect();
      }

      //
      // Exp::isIRExp
      //
      bool Exp::isIRExp() const
      {
         return v_isIRExp();
      }

      //
      // Exp::v_getArg
      //
      Arg Exp::v_getArg() const
      {
         return {getType(), IR::AddrBase::Lit, this};
      }

      //
      // Exp::v_getIRExp
      //
      IR::Exp::CRef Exp::v_getIRExp() const
      {
         std::cerr << "ERROR: " << pos << ": expected constant\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

