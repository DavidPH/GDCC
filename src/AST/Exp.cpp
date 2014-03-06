//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp copy constructor
      //
      Exp::Exp(Exp const &exp) :
         pos{exp.pos},

         cacheIRExp{exp.cacheIRExp}
      {
      }

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
         block.setOrigin(pos);

         // Special checks for expressions without side effects.
         if(!isEffect())
         {
            // If not evaluating for its result, then do not generate anything.
            if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
               return;

            // Try to evaluate constant expressions now.
            if(isIRExp())
            {
               genStmntMove(block, fn, dst,
                  Arg(getType(), IR::AddrBase::Lit, this));
               return;
            }
         }

         v_genStmnt(block, fn, dst);
      }

      //
      // Exp::genStmntStk
      //
      void Exp::genStmntStk(IR::Block &block, Function *fn) const
      {
         genStmnt(block, fn, Arg(getType(), IR::AddrBase::Stk));
      }

      //
      // Exp::getArg
      //
      Arg Exp::getArg() const
      {
         return v_getArg();
      }

      //
      // Exp::getArgDst
      //
      Arg Exp::getArgDst() const
      {
         auto arg = v_getArg();
         switch(arg.type->getQualAddr().base)
         {
         case IR::AddrBase::Cpy:
         case IR::AddrBase::Lit:
            throw Core::ExceptStr(pos, "expected destination expression");

         default:
            return arg;
         }
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
      // Exp::setRefer
      //
      void Exp::setRefer() const
      {
         v_setRefer();
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
         throw Core::ExceptStr(pos, "expected constant expression");
      }

      //
      // Exp::v_setRefer
      //
      void Exp::v_setRefer() const
      {
      }
   }
}

// EOF

