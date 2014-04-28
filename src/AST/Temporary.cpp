//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree temporaries handling.
//
//-----------------------------------------------------------------------------

#include "AST/Temporary.hpp"

#include "AST/Type.hpp"

#include "IR/Arg.hpp"
#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Temporary constructor
      //
      Temporary::Temporary(GenStmntCtx const &ctx, Core::Origin pos_) :
         pos  {pos_},
         fn   {ctx.fn},
         prog {&ctx.prog}
      {
         init();
      }

      //
      // Temporary constructor
      //
      Temporary::Temporary(GenStmntCtx const &ctx, Core::Origin pos_,
         Core::FastU size) :
         pos  {pos_},
         fn   {ctx.fn},
         prog {&ctx.prog}
      {
         init();
         alloc(size);
      }

      //
      // Temporary destructor
      //
      Temporary::~Temporary()
      {
         if(block)
            fn->localTmp.free(block);
      }

      //
      // Temporary::alloc
      //
      void Temporary::alloc(Core::FastU size)
      {
         // Check if already allocated.
         if(block)
         {
            // If already allocated at size, just keep that.
            if(block->size == size) return;

            // Otherwise, free and allocate a new temporary.
            fn->localTmp.free(block);
         }

         block = fn->localTmp.alloc(size);
         expO  = IR::ExpCreate_ValueRoot(
            IR::Value_Fixed(block->lo, Type::Size->getIRType().tFixed), pos);
      }

      //
      // Temporary::free
      //
      void Temporary::free()
      {
         if(!block) return;

         fn->localTmp.free(block);
         block = nullptr;
      }

      //
      // Temporary::getArg
      //
      IR::Arg_LocReg Temporary::getArg() const
      {
         return {IR::Arg_Lit(expB), expO};
      }

      //
      // Temporary::getArg
      //
      IR::Arg_LocReg Temporary::getArg(Core::FastU off) const
      {
         if(!off) return {IR::Arg_Lit(expB), expO};

         return {IR::Arg_Lit(expB), IR::ExpCreate_ValueRoot(
            IR::Value_Fixed(block->lo + off, Type::Size->getIRType().tFixed), pos)};
      }

      //
      // Temporary::init
      //
      void Temporary::init()
      {
         block = nullptr;
         expB  = IR::ExpCreate_ValueGlyph({prog, fn->getLabelTmp()}, pos);
      }
   }
}

// EOF

