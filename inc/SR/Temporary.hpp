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

#ifndef GDCC__SR__Temporary_H__
#define GDCC__SR__Temporary_H__

#include "../SR/Function.hpp"
#include "../SR/GenStmnt.hpp"

#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   class Arg_LocReg;
   class Arg_Stk;
}

namespace GDCC
{
   namespace SR
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
         Temporary(Temporary &&tmp);
         ~Temporary();

         Temporary &operator = (Temporary const &) = delete;
         Temporary &operator = (Temporary &&tmp);

         void alloc(Core::FastU size);

         void free();

         IR::Arg_LocReg getArg() const;
         IR::Arg_LocReg getArg(Core::FastU off) const;
         IR::Arg_Stk getArgStk() const;

         Core::FastU size() const {return block->size;}
         Core::FastU sizeBytes() const {return block->size * w;}

      private:
         void init();

         Core::Origin                     pos;
         Function::LocalTmp::Block const *block;
         Core::CounterPtr<IR::Exp const>  expB;
         Function::Ref                    fn;
         IR::Program                     *prog;
         Core::FastU                      w;
      };
   }
}

#endif//GDCC__SR__Temporary_H__

