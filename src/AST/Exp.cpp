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
         Super{exp},

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
      void Exp::genStmnt(GenStmntCtx const &ctx) const
      {
         genStmnt(ctx, Arg(getType(), IR::AddrBase::Nul));
      }

      //
      // Exp::genStmnt
      //
      void Exp::genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
      {
         ctx.block.setOrigin(pos);

         // Special checks for expressions without side effects.
         if(!isEffect())
         {
            // If not evaluating for its result, then do not generate anything.
            if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
               return;

            // Try to evaluate constant expressions now.
            if(isIRExp())
            {
               GenStmnt_Move(this, ctx, dst,
                  Arg(getType(), IR::AddrBase::Lit, this));
               return;
            }
         }

         v_genStmnt(ctx, dst);
      }

      //
      // Exp::genStmntStk
      //
      void Exp::genStmntStk(GenStmntCtx const &ctx) const
      {
         genStmnt(ctx, Arg(getType(), IR::AddrBase::Stk));
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
      // Exp::getArgDup
      //
      Arg Exp::getArgDup() const
      {
         auto arg = v_getArg();
         switch(arg.type->getQualAddr().base)
         {
         case IR::AddrBase::Cpy:
         case IR::AddrBase::Lit:
         case IR::AddrBase::Nul:
            throw Core::ExceptStr(pos, "expected duplication expression");

         default:
            return arg;
         }
      }

      //
      // Exp::getArgSrc
      //
      Arg Exp::getArgSrc() const
      {
         auto arg = v_getArg();
         switch(arg.type->getQualAddr().base)
         {
         case IR::AddrBase::Cpy:
         case IR::AddrBase::Nul:
            throw Core::ExceptStr(pos, "expected source expression");

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
         {
            cacheIRExp = v_getIRExp();

            IR::Type irTypeO = getType()->getIRType();
            IR::Type irTypeI = cacheIRExp->getType();

            if(irTypeO != irTypeI)
               cacheIRExp = IR::ExpCreate_UnaryCst(irTypeO, cacheIRExp, pos);
         }

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
      // Exp::isZero
      //
      bool Exp::isZero() const
      {
         if(!isIRExp()) return false;

         auto irExp = getIRExp();

         return irExp->canGetValue() && !irExp->getValue();
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

