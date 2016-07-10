//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation temporaries handling.
//
//-----------------------------------------------------------------------------

#include "SR/Temporary.hpp"

#include "SR/Type.hpp"

#include "IR/Arg.hpp"
#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace SR
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
         Core::FastU allocSize) :
         pos  {pos_},
         fn   {ctx.fn},
         prog {&ctx.prog}
      {
         init();
         alloc(allocSize);
      }

      //
      // Temporary move constructor
      //
      Temporary::Temporary(Temporary &&tmp) :
         pos  {tmp.pos},
         block{tmp.block},
         expB {tmp.expB},
         fn   {tmp.fn},
         prog {tmp.prog}
      {
         tmp.block = nullptr;
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
      // Temporary::operator = Temporary
      //
      Temporary &Temporary::operator = (Temporary &&tmp)
      {
         if(block)
            fn->localTmp.free(block);

         pos   = tmp.pos;
         block = tmp.block;
         expB  = tmp.expB;
         fn    = tmp.fn;
         prog  = tmp.prog;

         tmp.block = nullptr;

         return *this;
      }

      //
      // Temporary::alloc
      //
      void Temporary::alloc(Core::FastU allocSize)
      {
         // Check if already allocated.
         if(block)
         {
            // If already allocated at size, just keep that.
            if(block->size == allocSize) return;

            // Otherwise, free and allocate a new temporary.
            fn->localTmp.free(block);
         }

         block = fn->localTmp.alloc(allocSize);
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
         return {IR::Arg_Lit(expB), block->lo};
      }

      //
      // Temporary::getArg
      //
      IR::Arg_LocReg Temporary::getArg(Core::FastU off) const
      {
         return {IR::Arg_Lit(expB), block->lo + off};
      }

      //
      // Temporary::init
      //
      void Temporary::init()
      {
         block = nullptr;
         expB  = IR::ExpCreate_Glyph({prog, fn->getLabelTmp()}, pos);
      }
   }
}

// EOF

