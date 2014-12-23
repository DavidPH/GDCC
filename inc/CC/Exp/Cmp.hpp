//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C relational and equality expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Cmp_H__
#define GDCC__CC__Exp__Cmp_H__

#include "../../CC/Exp/Arith.hpp"

#include "../../AST/Exp/Cmp.hpp"
#include "../../AST/ExpCode.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_CmpArith
      //
      // Uses a base type and code set to generalize basic arithmetic
      // comparator expression creation.
      //
      template<typename Base, typename Codes>
      AST::Exp::CRef ExpCreate_CmpArith(AST::Type const *bt, AST::Type const *t,
         AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         IR::OpCode op;

         // Floating types.
         if(t->isCTypeFloat())
            op = AST::ExpCode_ArithFloat<Codes>(t);

         // Fixed-point types.
         else if(t->isCTypeFixed())
            op = AST::ExpCode_ArithFixed<Codes>(t);

         // Integer types.
         else if(t->isCTypeInteg())
            op = AST::ExpCode_ArithInteg<Codes>(t);

         // Pointer types.
         else if(t->isTypePointer())
            op = AST::ExpCode_ArithPoint<Codes>(t);

         // ???
         else
            throw Core::ExceptStr(pos, "unsupported arithmetic type");

         return AST::Exp_Arith<Base>::Create(op, bt, l, r, pos);
      }
   }
}

#endif//GDCC__CC__Exp__Cmp_H__

