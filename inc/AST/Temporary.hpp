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

#ifndef GDCC__AST__Temporary_H__
#define GDCC__AST__Temporary_H__

#include "../AST/Function.hpp"
#include "../AST/GenStmnt.hpp"

#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      struct Arg_LocReg;
   }

   namespace AST
   {
      //
      // Temporary
      //
      class Temporary
      {
      public:
         Temporary(GenStmntCtx const &ctx, Core::Origin pos);
         Temporary(GenStmntCtx const &ctx, Core::Origin pos, Core::FastU size);
         Temporary(Temporary const &) = delete;
         Temporary(Temporary &&) = delete;
         ~Temporary();

         Temporary &operator = (Temporary const &) = delete;
         Temporary &operator = (Temporary &&) = delete;

         void alloc(Core::FastU size);

         void free();

         IR::Arg_LocReg getArg() const;
         IR::Arg_LocReg getArg(Core::FastU off) const;

         Core::FastU size() const {return block->size;}

      private:
         void init();

         Core::Origin                     pos;
         Function::LocalTmp::Block const *block;
         Core::CounterPtr<IR::Exp const>  expB;
         Core::CounterPtr<IR::Exp const>  expO;
         Function::Ref                    fn;
         IR::Program                     *prog;
      };
   }
}

#endif//GDCC__AST__Temporary_H__

