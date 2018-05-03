//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "Platform/Platform.hpp"


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
         prog {&ctx.prog},
         w    {Platform::GetWordBytes()}
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
         prog {&ctx.prog},
         w    {Platform::GetWordBytes()}
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
         prog {tmp.prog},
         w    {tmp.w}
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
         w     = tmp.w;

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
         return {sizeBytes(), IR::Arg_Lit(w, expB), block->lo * w};
      }

      //
      // Temporary::getArg
      //
      IR::Arg_LocReg Temporary::getArg(Core::FastU off) const
      {
         return {w, IR::Arg_Lit(w, expB), (block->lo + off) * w};
      }

      //
      // Temporary::getArgStk
      //
      IR::Arg_Stk Temporary::getArgStk() const
      {
         return IR::Arg_Stk{sizeBytes()};
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

